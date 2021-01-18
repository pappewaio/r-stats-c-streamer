cmake_minimum_required(VERSION 3.0)
project(r-stats-c-streamer)
include_directories("./src")
file(GLOB Sources "src/*.c")
add_executable(r-stats-c-streamer ${Sources})

find_package(Rmath 1.0.0 REQUIRED)
target_link_libraries(r-stats-c-streamer Rmath::Rmath)
