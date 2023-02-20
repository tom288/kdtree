@echo off

set exe_name=debug.exe
set flags=-Og -D DEBUG
echo BUILDING...
call common.cmd

if %errorlevel% == 0 (
    echo LAUNCHING...
    robocopy >NUL "%glew%/bin/Release/x64/" %target_path% glew32.dll
    "%exe_path%"
    echo CLOSING...
) else ( pause )
