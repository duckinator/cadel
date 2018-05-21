# TODO: Don't rely on `find`?
SOURCE_FILES := $(shell find src -name '*.c')

CFLAGS := -std=c11 -pedantic-errors -Wall -Wextra -Wpedantic -Wunreachable-code -Wbad-function-cast -Werror=implicit-function-declaration -D_XOPEN_SOURCE

all: cadel

cadel: ${SOURCE_FILES}
	clang ${CFLAGS}  ${SOURCE_FILES} -lxcb -o $@

clean:
	find -name '*.o' -delete
	rm -f cadel
