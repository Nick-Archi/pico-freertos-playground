cmake_minimum_required(VERSION 3.22)

set(PICO_PLATFORM "rp2350-arm-s" CACHE STRING "Targeting pico 2350 specifically the arm core")

set(SOURCE_DIR "src/")
set(EXEC1_NAME "proj1")
set(EXEC2_NAME "proj2")
set(EXEC3_NAME "sh1106")

# initialize the SDK based on PICO_SDK_PATH
# note: this happens before the project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/Community-Supported-Ports/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of proj

add_executable(${EXEC1_NAME} 
    ${SOURCE_DIR}mutex_simple.c
    ${FREERTOS_SRC_FILES}
)

add_executable(${EXEC2_NAME} 
    ${SOURCE_DIR}mutex_typecasting.c
    ${FREERTOS_SRC_FILES}
)

add_executable(${EXEC3_NAME} 
    ${SOURCE_DIR}mutex_sh1106_interaction.c
    ${FREERTOS_SRC_FILES}
)

# add pico_stdlib library which aggregates commonly used features
target_link_libraries(${EXEC1_NAME} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4
)

# add pico_stdlib library which aggregates commonly used features
target_link_libraries(${EXEC2_NAME} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4
)

# edits for sbm integration
target_link_libraries(${EXEC3_NAME} 
    pico_stdlib 
    hardware_spi
    FreeRTOS-Kernel-Heap4
    sbm
)

target_include_directories(${EXEC1_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${EXEC2_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# edits for sbm integration
target_include_directories(${EXEC3_NAME} PUBLIC /usr/local/include/sbm)
target_include_directories(${EXEC3_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# edit for sbm integration
target_link_directories(${EXEC3_NAME} PUBLIC /usr/local/lib)

# tell compiler to use the USB output and disable UART output
pico_enable_stdio_usb(${EXEC1_NAME} 1)
pico_enable_stdio_uart(${EXEC1_NAME} 0)

pico_enable_stdio_usb(${EXEC2_NAME} 1)
pico_enable_stdio_uart(${EXEC2_NAME} 0)

pico_enable_stdio_usb(${EXEC3_NAME} 1)
pico_enable_stdio_uart(${EXEC3_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${EXEC1_NAME})
pico_add_extra_outputs(${EXEC2_NAME})
pico_add_extra_outputs(${EXEC3_NAME})
