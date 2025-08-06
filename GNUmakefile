all: libuuid.a

libuuid.a: libuuid.o
	ar -q $@ $<

libuuid.o: *.h
libuuid.o: libuuid.c
	gcc -c $< -o $@

test: LDLIBS:=libuuid.a
test: test.c
	./test
