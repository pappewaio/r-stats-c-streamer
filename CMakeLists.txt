cmake_minimum_required(VERSION 3.0)
project(stat_r_in_c)
include_directories("./src")
file(GLOB Sources "src/*.c")
add_executable(stat_r_in_c ${Sources})

find_package(Rmath 1.0.0 REQUIRED)
target_link_libraries(stat_r_in_c Rmath::Rmath)
