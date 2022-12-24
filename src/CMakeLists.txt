set(target flog)

add_executable(flog main.c flog.c flog.h config.c config.h defs.h utils.c utils.h)

target_link_libraries(${target} PRIVATE ${POPT_LINK_LIBRARIES})
target_include_directories(${target} PRIVATE ${POPT_INCLUDE_DIRS})
target_compile_options(${target} PRIVATE ${POPT_CFLAGS})

install(TARGETS flog DESTINATION bin)
