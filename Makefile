CXX = `root-config --cxx`
#CXXFLAGS = `root-config --cflags` -fPIC -g -Wall -DOPENXLSX_STATIC_DEFINE -fvisibility=hidden -fvisibility-inlines-hidden
CXXFLAGS = `root-config --cflags` -fPIC -g -Wall -DOPENXLSX_STATIC_DEFINE -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS = -Wl,-z,now -Wl,-z,relro -Wl,--no-as-needed

INCLUDES = -I/home/terawatt/Programs/OpenXLSX/OpenXLSX -I/home/terawatt/Programs/OpenXLSX/OpenXLSX/headers -I/home/terawatt/Programs/OpenXLSX/OpenXLSX/external/pugixml -I/home/terawatt/Programs/OpenXLSX/OpenXLSX/external/zippy
ROOTLIBS = `root-config --glibs` -lMathMore
SHARED = -shared

PROGRAM = ROOTOpenXLSXLib.so

# Наши исходники
ROOT_SRCS = ROOTOpenXLSXLib.cxx ROOTOpenXLSXLib.cpp

# Исходники OpenXLSX (все .cpp файлы в директории OpenXLSX и ее поддиректориях)
OPENXLSX_DIR = /home/terawatt/Programs/OpenXLSX/OpenXLSX
OPENXLSX_SRCS = $(shell find $(OPENXLSX_DIR) -name "*.cpp" -type f)

# Все исходники вместе
ALL_SRCS = $(ROOT_SRCS) $(OPENXLSX_SRCS)

all: $(PROGRAM)

ROOTOpenXLSXLib.cxx: ROOTOpenXLSXLib_LinkDef.h
	@echo "Generating dictionary ..."
	@rootcling -f ROOTOpenXLSXLib.cxx -rml $(PROGRAM) -rmf ROOTOpenXLSXLib.rootmap ROOTOpenXLSXLib.hh OpenXLSX.hpp ROOTOpenXLSXLib_LinkDef.h

$(PROGRAM): $(ALL_SRCS)
	@echo "Building $(PROGRAM) from sources ..."
	@rm -f $(PROGRAM)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(VISIBILITY_FLAGS) $(SHARED) $(LDFLAGS) \
           -o $@ $^ $(ROOTLIBS) -lzip -lz -ldl
	@echo "✅ Done!"

clean:
	@rm -rf core *.so *.rootmap *.cxx *.pcm
