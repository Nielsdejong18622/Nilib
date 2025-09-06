@echo off

echo "Number of Lines:"
dir /s /b /a-d | find /c /v ""

cmake -B build\debug -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake -B build\release -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

cmake --build build\debug --parallel
cmake --build build\release --parallel