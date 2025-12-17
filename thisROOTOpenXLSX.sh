SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:SCRIPT_DIR/external/OpenXLSX/OpenXLSX/
export CPATH=$CPATH:$SCRIPT_DIR/external/OpenXLSX/OpenXLSX/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:SCRIPT_DIR/external/OpenXLSX/gnu-make-crutch
export CPATH=$CPATH:$SCRIPT_DIR/external/OpenXLSX/gnu-make-crutch
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SCRIPT_DIR
export CPATH=$CPATH:$SCRIPT_DIR
