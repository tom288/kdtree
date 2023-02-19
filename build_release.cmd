@echo off

set exe_name=release.exe
set flags=-Ofast -static -static-libgcc -static-libstdc++ -Wl,-subsystem,windows
echo BUILDING RELEASE...
call common.cmd

if %errorlevel% == 0 (
    : Do not print launching
    copy >NUL "%glew%\bin\Release\x64\glew32.dll" %target_path%
    : Do not launch
    echo CLOSING...
) else ( pause )
