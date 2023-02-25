cmake_minimum_required(VERSION 3.22)
project(flog C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(POPT REQUIRED popt>=1.19)

if (NOT UNIT_TESTING)
    message(STATUS "Building program target")
    add_subdirectory(src)
else()
    message(STATUS "Building unit test targets")
    enable_testing()
    add_subdirectory(test)
endif()
