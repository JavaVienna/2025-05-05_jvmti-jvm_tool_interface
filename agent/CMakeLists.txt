cmake_minimum_required(VERSION 3.25)
project(agent C)
set(CMAKE_C_STANDARD 23)

add_subdirectory(1-method-entry)
add_subdirectory(2-npe)
add_subdirectory(3-jvmquake)
add_subdirectory(4-stacktraces)
add_subdirectory(5-track-objects)
