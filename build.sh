#!/bin/sh

set -e

if [ "$(uname)" == "Darwin" ]; then
    OpenGL="-framework OpenGL -I ../stb"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    OpenGL="-lGL"
else
    if [ "$(expr substr $(uname -s) 1 5)" == "MINGW" ]; then
        ./build.cmd
    fi
    exit 0
fi

# Create target directory
mkdir -p target

# Build
gcc src/*.c -o target/debug -Og -ggdb3 -D DEBUG \
-std=c17 -Wall -Wextra -pedantic -Wno-unused-parameter \
-lglfw -lGLEW $OpenGL

# Run
./target/debug
