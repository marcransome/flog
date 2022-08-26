include(CTest)

find_package(PkgConfig)
pkg_check_modules(CMOCKA REQUIRED cmocka)
include(add_cmocka_test)

add_cmocka_test(config)
