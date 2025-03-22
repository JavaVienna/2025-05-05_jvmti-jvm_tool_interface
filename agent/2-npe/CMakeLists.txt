cmake_minimum_required(VERSION 3.25)
project(agent C)

set(CMAKE_C_STANDARD 23)
include_directories(
        "~/.sdkman/candidates/java/current/include/"
        "~/.sdkman/candidates/java/current/include/linux/"
)

add_library(agent SHARED agent.c)
