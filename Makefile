BASH_MARKER_START = ROOTOpenXLSXLib_config_begin
BASH_MARKER_END = ROOTOpenXLSXLib_config_end

CXX = `root-config --cxx`
#CXXFLAGS = `root-config --cflags` -fPIC -g -Wall -DOPENXLSX_STATIC_DEFINE -fvisibility=hidden -fvisibility-inlines-hidden
CXXFLAGS = `root-config --cflags` -fPIC -g -Wall -DOPENXLSX_STATIC_DEFINE -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS = -Wl,-z,now -Wl,-z,relro -Wl,--no-as-needed

#INCLUDES = -I external/OpenXLSX/OpenXLSX -I external/OpenXLSX/OpenXLSX/headers -I external/OpenXLSX/OpenXLSX/external/pugixml -I external/OpenXLSX/OpenXLSX/external/zippy
ROOTLIBS = `root-config --glibs` 
SHARED = -shared

PROGRAM = ROOTOpenXLSXLib.so

# Наши исходники
ROOT_SRCS = ROOTOpenXLSXLib.cxx ROOTOpenXLSXLib.cpp
MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
# Исходники OpenXLSX (все .cpp файлы в директории OpenXLSX и ее поддиректориях)
OPENXLSX_DIR = $(MAKEFILE_DIR)external/OpenXLSX/OpenXLSX/
OPENXLSX_SRCS = $(shell find $(OPENXLSX_DIR) -name "*.cpp" -type f)
OPENXLSX_Headers = $(shell find $(MAKEFILE_DIR)external/ -name "*.hpp" -type f)
INCLUDES=-I $(MAKEFILE_DIR)external/OpenXLSX/OpenXLSX -I $(MAKEFILE_DIR)external/OpenXLSX/OpenXLSX/headers -I $(MAKEFILE_DIR)external/OpenXLSX/OpenXLSX/external/pugixml -I $(MAKEFILE_DIR)external/OpenXLSX/OpenXLSX/external/zippy -I $(MAKEFILE_DIR)external/OpenXLSX/gnu-make-crutch
# Все исходники вместе
ALL_SRCS = $(ROOT_SRCS) $(OPENXLSX_SRCS)


all: $(PROGRAM)

ROOTOpenXLSXLib.cxx: ROOTOpenXLSXLib_LinkDef.h
	@if [ -d .git ] && [ -f .gitmodules ]; then \
		if [ ! -f external/OpenXLSX/CMakeLists.txt ]; then \
			echo "Initializing git submodules..."; \
			git submodule update --init --recursive; \
		fi; \
	fi
	@echo "Generating dictionary ..."
	@rootcling -f ROOTOpenXLSXLib.cxx -rml $(PROGRAM) -rmf ROOTOpenXLSXLib.rootmap  ROOTOpenXLSXLib.hh ROOTOpenXLSXLib_LinkDef.h 

$(PROGRAM): $(ALL_SRCS)
	@echo "Building $(PROGRAM) from sources ..."
	@rm -f $(PROGRAM)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(VISIBILITY_FLAGS) $(SHARED) $(LDFLAGS) \
           -o $@ $^ $(ROOTLIBS) -lzip -lz -ldl
	@echo "Done"

clean:
	@rm -rf core *.so *.rootmap *.cxx *.pcm
install:
	@echo "Updating ~/.bashrc..."
	@if ! grep -q $(BASH_MARKER_START) ~/.bashrc 2>/dev/null; then \
		echo "#$(BASH_MARKER_START)" >> ~/.bashrc; \
		echo 'source '`pwd`/thisROOTOpenXLSX.sh >> ~/.bashrc; \
		echo "#$(BASH_MARKER_END)" >> ~/.bashrc; \
		echo "Added library paths to ~/.bashrc"; \
	else \
		echo "Library paths already in ~/.bashrc"; \
	fi
uninstall:
	@echo "Starting uninstall..."
	@echo "Cleaning ~/.bashrc (backup created)..."
	@if [ -f ~/.bashrc ]; then \
		cp ~/.bashrc ~/.bashrc.backup.$$(date +%Y%m%d_%H%M%S); \
		sed -i '\|$(BASH_MARKER_START)|,\|$(BASH_MARKER_END)|d' ~/.bashrc; \
		sed -i '/^$$/d' ~/.bashrc; \
		echo "Removed library configuration from ~/.bashrc"; \
		echo "Backup saved as ~/.bashrc.backup.*"; \
	else \
		echo "~/.bashrc not found, skipping..."; \
	fi
	@echo "Uninstall complete!"
