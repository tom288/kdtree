#!/bin/sh

# Create target directory
mkdir -p target

# Build
gcc src/main.c -o target/debug -Og -D DEBUG \
-std=c17 -Wall -Wextra -Wno-unused-parameter \
-framework OpenGL -lglfw -lGLEW

# Run
./target/debug
