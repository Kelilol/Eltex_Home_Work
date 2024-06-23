all: sender receiver

sender: sender.o
	gcc -o sender sender.o

receiver: receiver.o
	gcc -o receiver receiver.o

sender.o: sender.c
	gcc -c sender.c

receiver.o: receiver.c
	gcc -c receiver.c

clean:
	rm -f sender receiver sender.o receiver.o

