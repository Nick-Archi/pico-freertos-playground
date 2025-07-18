# FreeRTOS Pico Program 02: Mutexes 

## Table of Content
[Overivew](#overview)<br>
[Objective](#objective)<br>
[Building](#building)<br>
[Executing](#executing)<br>
[Useful](#useful)<br>
[Learnings](#learnings)<br>

## Overview
A mutex allows tasks to share a resource, effectively limiting the usage of that resource to just 1 task. Once the task is done, it gives up the mutex for another task to then take. In this project, I want to showcase some simple examples of using a mutex as well as an example showcasing the usage of the sbm library I've been working on to interact with an OLED as the shared resource.
 
## Objective
- [] Executable that spins up tasks to utilize a mutex to interact with a shared resource (OLED).

## Setup
Modify the FreeRTOSConfig.h:
- Update the configMAX_PRIORITES
- Enable Time Slicing
    - update configUSE_TIME_SLICING = 1
- Enable Mutual Exclusion
    - configUSE_MUTEXES = 1
- Enable indefinite waits
    - INCLUDE_vTaskSuspend = 1

## Building

Navigate to build dir and execute cmake & make
```
	mkdir build
	cd build
	cmake .. && make
	
	# For generating build logs for debugging...
	# cmake --trace-expand .. 2>&1 | tee cmake_build_output.txt && make 2>&1 | tee make_build_output.txt
```

Flash the built .uf2 file onto the Raspberry Pi Pico
```
	# connect the pico2 via USB
	# Drag & drop the .uf2 file into the location of where the Pico2 was connected
	OR
	$ cp <filename>.uf2 <dir_location>/RP2350
```

## Useful
- View logs from the Kernel to see USB connections
```
	$ watch -n 1 "sudo dmesg" | tail -n 20"
```
- Open Minicom to see serial 
```
	$ watch -n 1 "sudo dmesg" | tail -n 20"
```

## Learnings
```
- [] Had to figure out an approach to showcasing 2 different numbers (eg, 2 and 999) properly on the OLED without it getting overwritten. Used sprinttf to modify a char* buffer.
- [] Sending a generic void* that would then be typecasted into the struct I created, makes it easier to pass information to generic task functions.
```
