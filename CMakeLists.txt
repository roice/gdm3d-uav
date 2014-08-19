cmake_minimum_required (VERSION 2.8)
set (PRJ_NAME rtt-stm32f10x)
project (${PRJ_NAME})

####################### For Users to modify ##########################
# Adjust MCU Density in use, see CMSIS file "stm32f10x.h"
#STM32F103RBT (128KB FLASH, 20KB RAM) --> STM32F10X_MD
#set (MCU_DENSITY STM32F10X_MD)
#STM32F103RET (512KB FLASH, 64KB RAM) --> STM32F10X_HD
set (MCU_DENSITY STM32F10X_HD)
# select the build type for this lib project (release recommanded)
#set (CMAKE_BUILD_TYPE Release)
set (CMAKE_BUILD_TYPE Debug)
############################## End ###################################

###################### Don't touch if possible #######################
# set the linker script
set (LDSCRIPT "${PROJECT_SOURCE_DIR}/link/stm32_rom.ld")
# enable assembly language for "context_gcc.S" and "startup_stm32f10x_md.s" files
enable_language (ASM)
# include the tool chain file for stm32f10x
include (${PROJECT_SOURCE_DIR}/TC-stm32f10x.cmake)
###################### cmake user debug info ########################
message(STATUS "C_FLAGS=${CMAKE_C_FLAGS}")
message(STATUS "Linker_flags=${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "project_source_dir=${PROJECT_SOURCE_DIR}")
message(STATUS "project_binary_dir=${PROJECT_BINARY_DIR}")

#++++++++++++++++ INCLUDES ++++++++++++++++++++++
#==== User application related includes ===========
# path of the header files to be included
include_directories ("${PROJECT_SOURCE_DIR}/driver")
include_directories ("${PROJECT_SOURCE_DIR}/inc")
#===== STM32 Std Periph related includes ===========
# location of system_stm32f10x.c .h and stm32f10x.h
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/bsp/stm32f10x/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup)
# location of device support
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/bsp/stm32f10x/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x)
# location of stm32 core support
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/CMSIS/Include)
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/CMSIS/RTOS)
# headers of stm32f10x peripheral lib
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/bsp/stm32f10x/Libraries/STM32F10x_StdPeriph_Driver/inc)
#===== RT-Thread related includes =================
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/CMSIS/Include)
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/drivers/include)
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/finsh)
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/init)
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/include)
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/libcpu/arm/common)
include_directories (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/libcpu/arm/cortex-m3)

#++++++++++++++++ SOURCES +++++++++++++++++++++++
# source Users
aux_source_directory (${PROJECT_SOURCE_DIR}/src SrcUser)
# select startup script accoring to MCU_DENSITY
if (${MCU_DENSITY} STREQUAL STM32F10X_LD)
    set (SrcStartup ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/bsp/stm32f10x/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_ld.s)
elseif (${MCU_DENSITY} STREQUAL STM32F10X_MD)
    set (SrcStartup ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/bsp/stm32f10x/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.s)
elseif (${MCU_DENSITY} STREQUAL STM32F10X_HD)
    set (SrcStartup ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/bsp/stm32f10x/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_hd.s)
endif ()
# source drivers Users
aux_source_directory (${PROJECT_SOURCE_DIR}/driver SrcDriver)
# source CORTEX_M3 (libcpu)
set (SrcCortexM3 ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/libcpu/arm/cortex-m3/cpuport.c ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/libcpu/arm/cortex-m3/context_gcc.S ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/libcpu/arm/common/backtrace.c ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/libcpu/arm/common/div0.c ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/libcpu/arm/common/showmem.c)
# source CMSIS (using #define RT_USING_CMSIS_OS)
set (SrcCMSIS ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/CMSIS/RTOS/cmsis_rtthread.c)
# sources of stm32f10x peripheral lib
aux_source_directory (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/bsp/stm32f10x/Libraries/STM32F10x_StdPeriph_Driver/src SrcStm32f10xperiph)
set (SrcStm32f10xperiph ${SrcStm32f10xperiph} ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/bsp/stm32f10x/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c)
# source RTT kernel
aux_source_directory (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/src/ SrcRTTKernel)
# source RTT device drivers
set (SrcRTTDriver ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/drivers/serial/serial.c ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/drivers/src/completion.c ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/drivers/src/dataqueue.c ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/drivers/src/pipe.c ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/drivers/src/ringbuffer.c)
# source RTT component common
set (SrcRTTCPCommon ${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/init/components.c)
# source RTT component finsh
aux_source_directory (${PROJECT_SOURCE_DIR}/../RT-Thread_1.2.0/components/finsh SrcRTTCPFinsh)

#++++++++++++++++++ Compile +++++++++++++++++++++++
add_executable (${CMAKE_PROJECT_NAME}.elf ${SrcUser} ${SrcStartup} ${SrcDriver} ${SrcStm32f10xperiph} ${SrcCortexM3} ${SrcCMSIS} ${SrcRTTKernel} ${SrcRTTDriver} ${SrcRTTCPCommon} ${SrcRTTCPFinsh})
################# Convert elf to hex and bin ##########
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.hex ALL DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)

ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin ALL DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
