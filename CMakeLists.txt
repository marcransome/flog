cmake_minimum_required(VERSION 3.22)
project(flog C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if (NOT UNIT_TESTING)
    add_subdirectory(src)
else()
    add_subdirectory(test)
endif()
