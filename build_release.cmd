@echo off

set libs=C:\Git\C
set glfw=%libs%\glfw-3.3.8.bin.WIN64
set glew=%libs%\glew-2.1.0
set target_path=target
set exe_path=%target_path%\release.exe

echo BUILDING RELEASE...

gcc -std=c17 -march=native -Ofast ^
-Wall -Wextra -Wno-unused-parameter ^
-static -static-libgcc -static-libstdc++ -Wl,-subsystem,windows ^
-o %exe_path% ^
src/main.c ^
-I "src" ^
-I "%glfw%\include" ^
-I "%glew%\include" ^
-L "%glfw%\lib-mingw-w64" ^
-L "%glew%\lib\Release\x64" ^
-lglfw3 -lgdi32 -lopengl32 -lglew32

if %errorlevel% == 0 (
    : Do not print launching
    copy >NUL "%glew%\bin\Release\x64\glew32.dll" %target_path%
    : Do not launch
    echo CLOSING...
) else ( pause )
