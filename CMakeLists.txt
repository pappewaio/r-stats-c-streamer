cmake_minimum_required(VERSION 3.0)
project(qnorm_stdin)
add_executable(qnorm_stdin source.c)

find_package(Rmath 1.0.0 REQUIRED)
target_link_libraries(qnorm_stdin Rmath::Rmath)
