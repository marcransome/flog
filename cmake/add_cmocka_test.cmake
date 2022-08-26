function(add_cmocka_test unit)
    set(test_target test_${unit})

    add_executable(${test_target} test_${unit}.c ${CMAKE_SOURCE_DIR}/src/${unit}.c)

    target_link_libraries(${test_target} ${CMOCKA_LINK_LIBRARIES})
    target_include_directories(${test_target} PUBLIC ${CMOCKA_INCLUDE_DIRS})

    target_include_directories(${test_target} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_compile_options(${test_target} PUBLIC ${CMOCKA_CFLAGS_OTHER})
    target_compile_definitions(${test_target} PRIVATE UNIT_TESTING)

    add_test(NAME ${test_target} COMMAND ${test_target})
endfunction()
