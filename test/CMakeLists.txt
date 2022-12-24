include(CTest)

pkg_check_modules(CMOCKA REQUIRED cmocka>=1.1.5)

include(add_cmocka_test)

add_cmocka_test(config)
