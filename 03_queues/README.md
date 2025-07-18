# [WIP]FreeRTOS Pico Program 03: Queues 

## Table of Content
[Overivew](#overview)<br>
[Objective](#objective)<br>
[Building](#building)<br>
[Executing](#executing)<br>
[Useful](#useful)<br>
[Learnings](#learnings)<br>

## Overview
A queue (FIFO) could be used as a form of IPC between different tasks. I want to have a couple of examples showcasing how different tasks can utilize a queue for their own purposes. Queues in FreeRTOS are thread safe so it should be fine for multiple tasks(threads) to interact with a queue. 
 
## Objective
- [] Executable that shows tasks utilizing a mutex to interact with a shared resource (OLED).
- [] Mailbox shared between tasks in a producer(s) and consumer example.
- [] Queue set utilized in FreeRTOS.

## Setup
Modify the FreeRTOSConfig.h:
- Update the configMAX_PRIORITES
- Enable Time Slicing
    - update configUSE_TIME_SLICING = 1
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

```
