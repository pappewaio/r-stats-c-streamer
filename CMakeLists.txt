cmake_minimum_required(VERSION 3.0)
project(stat_r_in_c)
add_executable(stat_r_in_c source.c)

find_package(Rmath 1.0.0 REQUIRED)
target_link_libraries(stat_r_in_c Rmath::Rmath)
