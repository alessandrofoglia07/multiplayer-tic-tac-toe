cmake_minimum_required(VERSION 3.22)
project(multiplayer_tic_tac_toe C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_executable(server src/server.c
        include/server.h
        src/protocol.c
        include/protocol.h
        src/utils.c
        include/utils.h)
add_executable(game src/client.c
        include/client.h
        src/protocol.c
        include/protocol.h
        src/utils.c
        include/utils.h)

target_link_libraries(server pthread)