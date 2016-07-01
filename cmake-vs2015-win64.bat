@echo off
set BUILD_DIR=build-win64
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%
cmake ../ -G "Visual Studio 14 2015 Win64"
cd ..
pause
