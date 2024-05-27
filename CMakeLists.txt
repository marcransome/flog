cmake_minimum_required(VERSION 3.22)
project(flog C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

option(UNIT_TESTING "Build unit test targets" OFF)
option(ENABLE_COVERAGE "Build with coverage" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(POPT REQUIRED popt>=1.19)

add_subdirectory(src bin)

if (UNIT_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
