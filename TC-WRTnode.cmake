# the name of the target operating system, tell cmake we are doing cross compiling
set (CMAKE_SYSTEM_NAME Linux)
#include (CMakeForceCompiler)
set (CMAKE_SYSTEM_PROCESSOR mipsel)

# set the compilers to use for C and C++, and the archive tools, linker and link script(important)
#set (CPREF mipsel-openwrt-linux)
#CMAKE_FORCE_C_COMPILER (mipsel-openwrt-linux-gcc GNU)
#CMAKE_FORCE_CXX_COMPILER (mipsel-openwrt-linux-c++ GNU)
#set (AS ${CPREF}-gcc)
#set (AR ${CPREF}-ar)
#set (LD ${CPREF}-ld)
#set (NM ${CPREF}-nm)
#set (OBJCOPY ${CPREF}-objcopy)
#set (OBJDUMP ${CPREF}-objdump)
#set (READELF ${CPREF}-readelf)

set (CMAKE_C_COMPILER mipsel-openwrt-linux-gcc)
set (CMAKE_CXX_COMPILER mipsel-openwrt-linux-g++)

set (CMAKE_FIND_ROOT_PATH ${STAGING_DIR})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

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

# include staging_dir/target-architecture-uclibc-libcver/usr/include
# to search for the header files for OpenWRT
# Note: STAGING_DIR is the root path of OpenWRT toolchain
#include_directories(${STAGING_DIR}/target-mipsel_r2_uClibc-0.9.33.2/usr/include)
include_directories (${STAGING_DIR}/toolchain-mipsel_r2_gcc-4.6-linaro_uClibc-0.9.33.2/include)

# add staging_dir/target-architecture-uclibc-libcver/usr/lib
# to search for the shared objects for OpenWRT
#link_directories(${STAGING_DIR}/target-mipsel_r2_uClibc-0.9.33.2/usr/lib)
link_directories (${STAGING_DIR}/toolchain-mipsel_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib)
