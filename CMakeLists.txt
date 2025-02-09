cmake_minimum_required(VERSION 3.29)
project(chess C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-O3)

# add_executable(chess main.c chess.c chess.h test_perft.c)
# add_executable(chess main.c test_perft.c chess_header_only.h)

add_executable(chess main.c test_perft.c chess.c chess.h)
