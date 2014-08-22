# the name of the target operating system, tell cmake we are doing cross compiling
set (CMAKE_SYSTEM_NAME Generic)
include (CMakeForceCompiler)

# set the compilers to use for C and C++, and the archive tools, linker and link script(important)
CMAKE_FORCE_C_COMPILER (gcc GNU)
CMAKE_FORCE_CXX_COMPILER (g++ GNU)

# set assembly and c language compile flags
set (CMAKE_C_FLAGS "-D _REENTRANT -D __STDC_FORMAT_MACROS -lpthread -Wall")
set (CMAKE_CXX_FLAGS "-D _REENTRANT -D __STDC_FORMAT_MACROS -lpthread -Wall")
# debug version compile flags
# -gstabs generate debug info as stabs format, gdb debug info excluded
# -gstabs+ generate debug info as stabs format, gdb debug info included
# -ggdb generate debug info for gdb
# -glevel generate debug info according to the level requested(default is 2)
set (CMAKE_C_FLAGS_DEBUG "-O0 -g -gstabs+")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -gstabs+")
# release version compile flags
set (CMAKE_C_FLAGS_RELEASE "-O3")
set (CMAKE_CXX_FLAGS_RELEASE "-O3")
