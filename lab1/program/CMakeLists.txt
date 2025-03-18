cmake_minimum_required(VERSION 3.31)

project(ir_lab_1)

set(CMAKE_C_STANDARD 23)

add_executable(ir_lab_1
    src/main.c
    src/data_structures/string.c
    src/data_structures/string.h
    src/information_retrieval/data_structures.c
    src/information_retrieval/data_structures.h
)
