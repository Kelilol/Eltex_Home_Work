CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = main.c permissions.c
OBJ = $(SRC:.c=.o)
DEPS = permissions.h

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) Phonebook
