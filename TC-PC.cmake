# the name of the target operating system, tell cmake we are doing cross compiling
set (CMAKE_SYSTEM_NAME Generic)
include (CMakeForceCompiler)

# set the compilers to use for C and C++, and the archive tools, linker and link script(important)
set (CPREF arm-none-eabi)
CMAKE_FORCE_C_COMPILER (arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER (arm-none-eabi-c++ GNU)
set (AS ${CPREF}-gcc)
set (AR ${CPREF}-ar)
set (LD ${CPREF}-ld)
set (NM ${CPREF}-nm)
set (OBJCOPY ${CPREF}-objcopy)
set (OBJDUMP ${CPREF}-objdump)
set (READELF ${CPREF}-readelf)

# here is the target environment is located
#set (CMAKE_FIND_ROOT_PATH /home/roice/Programs/stm32-tools/gcc-arm-embedded/gcc-arm-none-eabi-4_8-2014q2)
# adjust the default behaviour of the FIND_XXX() commands:  
# search headers and libraries in the target environment, search   
# programs in the host environment  
#set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)  
#set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)  
#set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set the type of MCU core
set (MCU_CORE cortex-m3)
set (MCFLAGS "-mcpu=${MCU_CORE} -mthumb -ffunction-sections -fdata-sections -D${MCU_DENSITY} -DVECT_TAB_FLASH -DUSE_STDPERIPH_DRIVER")

# set assembly and c language compile flags
set (CMAKE_ASM_FLAGS "-c ${MCFLAGS} -x assembler-with-cpp")
set (CMAKE_C_FLAGS "${MCFLAGS} -Wall")  
# debug version compile flags
# -gstabs generate debug info as stabs format, gdb debug info excluded
# -gstabs+ generate debug info as stabs format, gdb debug info included
# -ggdb generate debug info for gdb
# -glevel generate debug info according to the level requested(default is 2)
set (CMAKE_ASM_FLAGS_DEBUG "-g -gstabs+")
set (CMAKE_C_FLAGS_DEBUG "-O0 -g -gstabs+")
#set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -gstabs+")
# release version compile flags
set (CMAKE_C_FLAGS_RELEASE "-O3")
#set (CMAKE_CXX_FLAGS_RELEASE "-O3")
set (CMAKE_ASM_FLAGS_RELEASE "")
# set linker flags
set (CMAKE_EXE_LINKER_FLAGS "${MCFLAGS} -Wl,--gc-sections,-Map=${PRJ_NAME}.map,-cref,-u,Reset_Handler -T ${LDSCRIPT}")

set (CMAKE_MODULE_LINKER_FLAGS "")
set (CMAKE_SHARED_LINKER_FLAGS "")
# remove -rdynamic properties
set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
