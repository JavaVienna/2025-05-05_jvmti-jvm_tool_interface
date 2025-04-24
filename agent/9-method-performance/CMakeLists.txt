set(CMAKE_C_STANDARD 23)

include_directories(
        "~/.sdkman/candidates/java/current/include/"
        "~/.sdkman/candidates/java/current/include/linux/"
)

add_library(agent-9 SHARED agent.c)
