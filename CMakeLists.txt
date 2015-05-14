# ┌──────────────────────────────────────────────────────────────────┐
# │  Projects Settings                                               │
# └──────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.0)
project(FluidScripten)
add_definitions("-std=c++11")

# ┌──────────────────────────────────────────────────────────────────┐
# │  Executable                                                      │
# └──────────────────────────────────────────────────────────────────┘
file(GLOB_RECURSE source_files src/*.cpp)
add_executable(FluidScripten.js ${source_files})

# emscripten specific
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_FILESYSTEM=1 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_FILESYSTEM=1 ")
