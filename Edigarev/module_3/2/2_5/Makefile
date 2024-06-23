CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = counter

all: $(TARGET)

$(TARGET): counter.c
	$(CC) $(CFLAGS) -o $(TARGET) counter.c

clean:
	rm -f $(TARGET) counter.txt
