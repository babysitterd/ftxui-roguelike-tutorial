#!/bin/sh

# rm -rf build_emscripten
mkdir -p build_emscripten && cd build_emscripten
conan install --build=missing ..
emcmake cmake ..
cmake --build .

cp index.html bin/
cp run_webassembly.py bin/
cd bin
python3 ./run_webassembly.py
