set libs=..
set glfw=%libs%\glfw-3.3.8.bin.WIN64
set glew=%libs%\glew-2.1.0
set cglm=%libs%\cglm
set target_path=target
set exe_path=%target_path%\%exe_name%

if exist custom.cmd (
    call custom.cmd
)

if not exist %target_path% (
    mkdir %target_path%
)

gcc -std=c17 -Wall -Wextra -Wno-unused-parameter ^
src/main.c -o %exe_path% ^
-I "%glfw%\include" ^
-I "%glew%\include" ^
-I "%cglm%\include" ^
-L "%glfw%\lib-mingw-w64" ^
-L "%glew%\lib\Release\x64" ^
-lglfw3 -lgdi32 -lopengl32 -lglew32 %flags%
