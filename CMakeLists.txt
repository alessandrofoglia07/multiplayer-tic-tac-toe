cmake_minimum_required(VERSION 3.22)
project(multiplayer_tic_tac_toe C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(server src/server.c
        src/protocol.c
        include/protocol.h
        include/server.h
        src/utils.c
        include/utils.h)
add_executable(game src/client.c
        src/protocol.c
        include/protocol.h
        include/client.h
        src/utils.c
        include/utils.h)

target_link_libraries(server pthread)