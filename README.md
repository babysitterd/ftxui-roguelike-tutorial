# ftxui-roguelike-tutorial
r/roguelikedev summer tutorial 2022 in FTXUI

Build steps (Windows/Linux/macOS)
```
mkdir build && cd build
conan install --build=missing ..
cmake ..
cmake --build .
```

To run a Webassembly version just
```
docker compose up
(visit localhost:8000)
```
