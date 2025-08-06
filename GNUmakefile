all: libuuid.a

libuuid.a: libuuid.o
	ar -q $@ $<

libuuid.o: *.h
libuuid.o: libuuid.c
	gcc -c $< -o $@

test_uuid: LDLIBS:=libuuid.a
test_uuid: libuuid.a

test: test_uuid
	./test_uuid

install: test
	cp uuid.h ${HOME}/.local/include
	cp libuuid.a ${HOME}/.local/lib
