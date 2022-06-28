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
 
add_executable(ftxui-roguelike-tutorial src/main.cpp)
target_include_directories(ftxui-roguelike-tutorial PRIVATE src)
 
target_link_libraries(ftxui-roguelike-tutorial
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)
