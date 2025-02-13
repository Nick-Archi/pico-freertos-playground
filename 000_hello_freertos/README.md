# FreeRTOS Pico Program 000: Beginning of Something

This was my 1st attempt at trying to build the FreeRTOS for the Raspberry Pi Pico2 (RP2350) without an IDE. I struggled a bit as I didn't find that many resourcesand heavily relied on CMake documentation to debug things (shoutout execute_process(COMMAND read -p "DEBUG") and message()). 

It took me a couple of days but it works...

# Table of Content
[Overivew](#overview)<br>
[Objective](#objective)<br>
[Building](#building)<br>
[Executing](#executing)<br>
[Useful](#useful)<br>

## Overview
- Program adapted from the [pico-examples/hello_freertos](https://github.com/raspberrypi/pico-examples/freertos).
- This program can executed to ensure that the environment is setup properly (environment variables set properly, etc)
- By viewing the CMakeLists.txt, you should get an idea of how the project is setup but if it's not obvious then the [Pico SDK](https://github.com/raspberrypi/pico-sdk) and [FreeRTOS-Kernel](https://github.com/FreeRTOS/FreeRTOS/) are downloaded from their respective repos and set as environment variables. 

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

## Useful 
- View logs from the Kernel to see USB connections
```
	$ watch -n 1 "sudo dmesg" | tail -n 20"
```
