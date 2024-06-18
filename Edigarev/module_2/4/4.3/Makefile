# Имя исполняемого файла
TARGET = contact_tree

# Компилятор и флаги компиляции
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Список исходных файлов
SRCS = main.c

# Список объектных файлов (вычисляется заменой .c на .o)
OBJS = $(SRCS:.c=.o)

# Правило по умолчанию - сборка всего проекта
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Правило для сборки объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для очистки сборочных файлов
clean:
	rm -f $(TARGET) $(OBJS)

# Правило для повторной сборки проекта
rebuild: clean all

# Дополнительные полезные правила
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run

