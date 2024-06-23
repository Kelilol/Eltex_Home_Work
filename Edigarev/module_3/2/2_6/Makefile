CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = counter
SOURCES = counter.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) counter.txt
