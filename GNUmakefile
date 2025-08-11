CFLAGS:=-Wall -Wextra -Wpedantic

all: libuuid.a

libuuid.a: libuuid.o
	ar -q $@ $<

libuuid.o: *.h
libuuid.o: libuuid.c

test_uuid: LDLIBS:=libuuid.a
test_uuid: libuuid.a

test: test_uuid
	./test_uuid

install: test
	install -v uuid.h ${HOME}/.local/include/uuid
	install -v libuuid.a ${HOME}/.local/lib

clean:
	$(RM) *.o *.a *~

distclean:
	git clean -xdf
