set (CMAKE_SYSTEM_PROCESSOR "arm"              CACHE STRING "")
set (MACHINE                "zynqmp_r5"        CACHE STRING "")
set (PROJECT_VENDOR         "xlnx"             CACHE STRING "")
set (CROSS_PREFIX           "armr5-none-eabi-" CACHE STRING "")
set (CMAKE_C_FLAGS          "-mfloat-abi=soft -mcpu=cortex-r5" CACHE STRING "")

include (cross-freertos-gcc)

