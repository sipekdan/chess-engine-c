cmake_minimum_required(VERSION 3.27) # 3.29
project(chess C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-O3)

# add_executable(chess main.c chess.c chess.h test_perft.c)
# add_executable(chess main.c test_perft.c chess.h)

# add_executable(chess main.c test_perft.c chess.c chess.h)

include_directories(${CMAKE_SOURCE_DIR}/)

add_executable(chess examples/example_02.c test_perft.c chess.h)