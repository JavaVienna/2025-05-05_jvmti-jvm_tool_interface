set(CMAKE_C_STANDARD 23)

find_package(JNI REQUIRED)
include_directories(
        "~/.sdkman/candidates/java/current/include/"
        "~/.sdkman/candidates/java/current/include/linux/"
        ${JNI_INCLUDE_DIRS}
)

add_library(agent-11 SHARED agent.c io_kay_Main.h)
target_link_libraries(agent-11 ${JNI_LIBRARIES})
target_include_directories(agent-11 PUBLIC .)