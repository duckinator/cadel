# TODO: Don't rely on `find`?
SOURCE_FILES := $(shell find src -name '*.c')

all: cadel

cadel: ${SOURCE_FILES}
	clang -Wall ${SOURCE_FILES} -lxcb -o $@

clean:
	find -name '*.o' -delete
	rm -f cadel
