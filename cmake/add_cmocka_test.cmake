function(add_cmocka_test unit)
    set(test_target test_${unit})

    add_executable(${test_target} test_${unit}.c ${CMAKE_SOURCE_DIR}/src/${unit}.c)

    if (ENABLE_COVERAGE)
        target_link_options(${test_target} PRIVATE --coverage)
        target_compile_options(${test_target} PRIVATE -fprofile-arcs PRIVATE -ftest-coverage)
    endif()

    target_link_libraries(${test_target} PRIVATE ${CMOCKA_LINK_LIBRARIES} PRIVATE ${POPT_LINK_LIBRARIES})
    target_include_directories(${test_target} PRIVATE ${CMAKE_SOURCE_DIR}/src PRIVATE ${CMOCKA_INCLUDE_DIRS} PRIVATE ${POPT_INCLUDE_DIRS})

    target_compile_options(${test_target} PRIVATE ${CMOCKA_CFLAGS} PRIVATE ${POPT_CFLAGS} PRIVATE -O0)
    target_compile_definitions(${test_target} PRIVATE UNIT_TESTING)

    add_test(NAME ${test_target} COMMAND ${test_target})
endfunction()
