# FreeRTOS Pico Program 00

- Program adapted from the pico-examples/freertos repo.
- This program can executed to ensure that the environment is setup properly (environment variables set properly, etc)

# Pico SDK Project Setup Steps
# Build Project via Linux Terminal

Navigate to build dir and execute cmake & make
```
	mkdir build
	cd build
	cmake .. && make
	
	# For generating build logs for debugging...
	# cmake --trace-expand .. 2>&1 | tee cmake_build_output.txt && make 2>&1 | tee make_build_output.txt
```
# Project Learnings

