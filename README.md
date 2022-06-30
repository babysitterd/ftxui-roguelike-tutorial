# ftxui-roguelike-tutorial
r/roguelikedev summer tutorial 2022 in FTXUI

Build steps (Windows/Linux/macOS)
```
mkdir build && cd build
cmake ..
cmake --build .
```

Webassembly build steps
```
mkdir build_emscripten && cd build_emscripten
emcmake cmake ..
make -j
./run_webassembly.py
(visit localhost:8000)
```
