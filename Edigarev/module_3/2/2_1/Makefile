# Название исполняемого файла
TARGET = square_areas

# Компилятор и флаги компиляции
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Исходные файлы
SRCS = main.c

# Правило по умолчанию
all: $(TARGET)

# Правило сборки исполняемого файла
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Правило очистки
clean:
	rm -f $(TARGET)

# Правило запуска программы
run: $(TARGET)
	./$(TARGET) 3 4 5 6 7 8

.PHONY: all clean run
