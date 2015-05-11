# ┌──────────────────────────────────────────────────────────────────┐
# │  Projects Settings                                               │
# └──────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.0)
project(FluidEmscripten)
add_definitions("-std=c++11")

# ┌──────────────────────────────────────────────────────────────────┐
# │  Executable                                                      │
# └──────────────────────────────────────────────────────────────────┘
file(GLOB_RECURSE source_files src/*.cpp)
add_executable(FluidEmscripten.html ${source_files})

# trick cmake and emscripten
list(APPEND CMAKE_CXX_FLAGS " -s USE_SDL=2")
