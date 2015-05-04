# ┌──────────────────────────────────────────────────────────────────┐
# │  Projects Settings                                               │
# └──────────────────────────────────────────────────────────────────┘

cmake_minimum_required(VERSION 3.0)
project(FluidEmscripten)

add_definitions("-std=c++11")
file(GLOB_RECURSE source_files src/*.cpp)
add_executable(FluidEmscripten.html ${source_files})
