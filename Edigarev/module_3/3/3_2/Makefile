CC = gcc
CFLAGS = -Wall
LDFLAGS = -lrt

all: client server

client: client.o
	$(CC) $(CFLAGS) -o $@ client.o $(LDFLAGS)

server: server.o
	$(CC) $(CFLAGS) -o $@ server.o $(LDFLAGS)

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f client server *.o
