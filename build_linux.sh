#!/bin/sh

# Create target directory
mkdir -p target

# Build
gcc src/*.c -o target/debug -Og -D DEBUG \
-std=c17 -Wall -Wextra -Wno-unused-parameter \
-lglfw -lGL -lGLEW

# Run
./target/debug
