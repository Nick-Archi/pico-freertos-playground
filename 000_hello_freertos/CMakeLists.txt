cmake_minimum_required(VERSION 3.22)

set(PICO_PLATFORM "rp2350-arm-s" CACHE STRING "Targeting pico 2350 specifically the arm core")

set(SOURCE_DIR "src/")
set(EXEC1_NAME "hello_freertos")

# initialize the SDK based on PICO_SDK_PATH
# note: this happens before the project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of proj

# GLOB RECURSE SEARCH FOR FREERTOS SOURCE FILES
file(GLOB FREERTOS_SRC_FILES $ENV{FREERTOS_KERNEL_PATH}/*.c)

# find other *.c files necessary for FreeRTOS Kernel building
file(GLOB NON_SECURE_SRC_FILES $ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/Community-Supported-Ports/GCC/RP2350_ARM_NTZ/non_secure/*.c)
file(GLOB HEAP_SRC_FILES $ENV{FREERTOS_KERNEL_PATH}/portable/MemMang/*.c)

list(APPEND FREERTOS_SRC_FILES ${NON_SECURE_SRC_FILES} ${HEAP_SRC_FILES})

# remove heap_*.c files so only 1 remains...
set(FILES_TO_REMOVE "heap_1.c" "heap_2.c" "heap_3.c" "heap_5.c")
foreach(file_to_remove IN LISTS FILES_TO_REMOVE)
	foreach(file IN LISTS FREERTOS_SRC_FILES)
		get_filename_component(filename ${file} NAME)
		
		if(filename STREQUAL file_to_remove)
			list(REMOVE_ITEM FREERTOS_SRC_FILES ${file})
		endif()
	endforeach()
endforeach()

message("FREERTOS_SRC_FILES = " ${FREERTOS_SRC_FILES})

add_executable(${EXEC1_NAME} 
    ${SOURCE_DIR}hello_freertos.c
    ${FREERTOS_SRC_FILES}
)

# add pico_stdlib library which aggregates commonly used features
target_link_libraries(${EXEC1_NAME} 
    pico_stdlib 
    pico_multicore
    hardware_adc
    pico_async_context_freertos
    hardware_exception
)

target_include_directories(${EXEC1_NAME} PRIVATE $ENV{FREERTOS_KERNEL_PATH}/include/)
target_include_directories(${EXEC1_NAME} PRIVATE $ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/Community-Supported-Ports/GCC/RP2350_ARM_NTZ/non_secure/)
target_include_directories(${EXEC1_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# tell compiler to use the USB output and disable UART output
pico_enable_stdio_usb(${EXEC1_NAME} 1)
pico_enable_stdio_uart(${EXEC1_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${EXEC1_NAME})
