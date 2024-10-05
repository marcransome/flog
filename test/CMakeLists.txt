pkg_check_modules(CMOCKA REQUIRED cmocka>=1.1.7)

include(add_cmocka_test)

add_cmocka_test(config)
add_cmocka_test(common)
