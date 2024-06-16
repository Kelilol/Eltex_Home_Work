CC=gcc
CFLAGS=-Wall -pedantic -std=c99

all: main

libs/libadd.so: add.c
	$(CC) $(CFLAGS) -fPIC -shared -o libs/libadd.so add.c

libs/libsub.so: sub.c
	$(CC) $(CFLAGS) -fPIC -shared -o libs/libsub.so sub.c

libs/libmul.so: mul.c
	$(CC) $(CFLAGS) -fPIC -shared -o libs/libmul.so mul.c

libs/libdiv.so: div.c
	$(CC) $(CFLAGS) -fPIC -shared -o libs/libdiv.so div.c

main: main.c libs/libadd.so libs/libsub.so libs/libmul.so libs/libdiv.so
	$(CC) $(CFLAGS) -o main main.c -ldl -Llibs -ladd -lsub -lmul -ldiv

rebuild: clean all

clean:
	rm -f main libs/libadd.so libs/libsub.so libs/libmul.so libs/libdiv.so
