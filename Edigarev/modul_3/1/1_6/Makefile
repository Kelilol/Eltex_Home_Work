CC=gcc
CFLAGS=-Wall -pedantic -std=c99

file_manager: file_manager.o
	$(CC) $(CFLAGS) -o file_manager file_manager.o

file_manager.o: file_manager.c
	$(CC) $(CFLAGS) -c file_manager.c

clean:
	rm -f file_manager.o file_manager
