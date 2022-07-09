cmake_minimum_required (VERSION 3.11)

set(CMAKE_CXX_STANDARD 20)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v3.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(ftxui-roguelike-tutorial
  LANGUAGES CXX
  VERSION 1.0.0
)

set(ROGUELIKE_SOURCES
  src/Actor.cpp
  src/Entity.cpp
  src/Map.cpp
  src/Tile.cpp
  src/World.cpp
)

add_executable(roguelike-tutorial
  src/main.cpp
  ${ROGUELIKE_SOURCES}
)

target_include_directories(roguelike-tutorial PRIVATE src)

target_link_libraries(roguelike-tutorial
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

if (EMSCRIPTEN)
  string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS")
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s ASYNCIFY")
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s PROXY_TO_PTHREAD")

  foreach(file "index.html" "run_webassembly.py")
    configure_file("src/${file}" ${file})
  endforeach(file)
else()
  enable_testing()
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup()

  add_executable(roguelike-tests
    test/main.cpp
    test/example.cpp
    ${ROGUELIKE_SOURCES}
  )

  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  include(CTest)
  include(Catch)

  target_link_libraries(roguelike-tests
    PRIVATE ${CONAN_LIBS}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
  )

  target_include_directories(roguelike-tests PRIVATE src)

  catch_discover_tests(roguelike-tests)
endif()
