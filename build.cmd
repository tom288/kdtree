@echo off

set exe_name=debug.exe
set flags=-Og -D DEBUG
echo BUILDING...
call common.cmd

if %errorlevel% == 0 (
    echo LAUNCHING...
    copy >NUL "%glew%\bin\Release\x64\glew32.dll" %target_path%
    %exe_path%
    echo CLOSING...
) else ( pause )
