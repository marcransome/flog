add_executable(flog main.c flog.c flog.h config.c config.h defs.h utils.c utils.h)

install(TARGETS flog DESTINATION bin)
