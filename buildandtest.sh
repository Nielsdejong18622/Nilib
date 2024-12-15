cmake -B build -G "MinGW Makefiles"
cmake --build build
cd build 
ctest
cd ..