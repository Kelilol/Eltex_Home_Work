# Название исполняемого файла
TARGET = contact_manager

# Компилятор и флаги компиляции
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Исходные файлы
SRCS = main.c contact_list.c file_io.c

# Объектные файлы
OBJS = $(SRCS:.c=.o)

# Заголовочные файлы
DEPS = contact_list.h file_io.h

# Правило по умолчанию
all: $(TARGET)

# Правило сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Правило сборки объектных файлов
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило очистки
clean:
	rm -f $(TARGET) $(OBJS)

# Правило запуска программы
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

