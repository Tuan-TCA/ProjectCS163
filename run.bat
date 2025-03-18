rmdir /s /q build
cmake -G "MinGW Makefiles" -S . -B build
cd build
cmake --build .
main.exe
