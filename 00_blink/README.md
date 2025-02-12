# FreeRTOS Pico Program 00: Blinking

## Table of Content
[Overivew](#overview)<br>
[Objective](#objective)<br>
[Building](#building)<br>
[Executing](#executing)<br>
[Useful Commands](#useful commands)<br>

## Overview
This project is meant to be a simpler starting point for anyone wanting to start developing applications with FreeRTOS in an IDEless environment.

[Notes]
- Adapted the simple blink program from LearnEmbeddedSystems Youtube Channel 
- Adapted the 000 Program's CMakeLists.txt and removed uncessary source files from being built from the FreeRTOS Kernel
 
## Objective
This project can be downloaded and executed to ensure that your build environment is setup properly. i.e.) The FreeRTOS-Kernel & Pico SDK paths established.

## Building
Navigate to build dir and execute cmake & make
```
	mkdir build
	cd build
	cmake .. && make
	
	# For generating build logs for debugging...
	# cmake --trace-expand .. 2>&1 | tee cmake_build_output.txt && make 2>&1 | tee make_build_output.txt
```

## Executing
Flash the built .uf2 file onto the Raspberry Pi Pico
```
	# connect the pico2 via USB
	# Drag & drop the .uf2 file into the location of where the Pico2 was connected
```

## Useful Commands 
- View logs from the Kernel to see USB connections
```
	$ watch -n 1 "sudo dmesg" | tail -n 20"
```
