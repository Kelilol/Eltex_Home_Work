CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = main.c phonebook.c
OBJ = $(SRC:.c=.o)
DEPS = phonebook.h contact.h sociallink.h email.h phonenumber.h 

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) Phonebook
