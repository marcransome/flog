cmake_minimum_required(VERSION 3.22)
project(flog C)

set(CMAKE_C_STANDARD 17)

add_executable(flog main.c flog.c flog.h config.c config.h defs.h utils.c utils.h)
