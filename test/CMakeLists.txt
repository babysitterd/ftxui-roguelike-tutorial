# --- Fetch Catch2 --------------------------------------------------------------
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

FetchContent_MakeAvailable(Catch2)
# ------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(roguelike-tests example.cpp)
target_link_libraries(roguelike-tests PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(roguelike-tests)
