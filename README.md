# ftxui-roguelike-tutorial
r/roguelikedev summer tutorial 2022 in FTXUI

Build steps (Windows/Linux/macOS)
```
mkdir build && cd build
conan install ..
cmake ..
cmake --build .
```

Webassembly build steps
```
mkdir build_emscripten && cd build_emscripten
emcmake cmake ..
cmake --build .
./run_webassembly.py
(visit localhost:8000)
```
