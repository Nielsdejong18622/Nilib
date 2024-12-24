@echo off

echo "Number of Lines:"
dir /s /b /a-d | find /c /v ""


cmake -B build -G "MinGW Makefiles"
cmake --build build
cd build 
ctest
cd ..