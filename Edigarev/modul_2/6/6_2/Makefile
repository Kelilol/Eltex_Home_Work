# Имя динамической библиотеки
LIBRARY = libcontacts.so

# Компилятор и флаги компиляции
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -shared

# Список исходных файлов библиотеки
LIB_SRCS = contacts_list.c

# Список объектных файлов библиотеки (вычисляется заменой .c на .o)
LIB_OBJS = $(LIB_SRCS:.c=.o)

# Исходные файлы основного приложения
APP_SRCS = main.c

# Список объектных файлов основного приложения (вычисляется заменой .c на .o)
APP_OBJS = $(APP_SRCS:.c=.o)

# Имя исполняемого файла
TARGET = contacts_app

# Правило по умолчанию - сборка всего проекта
all: $(TARGET)

# Правило для сборки динамической библиотеки
$(LIBRARY): $(LIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Правило для сборки исполняемого файла
$(TARGET): $(APP_OBJS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJS) -L. -lcontacts

# Правило для сборки объектных файлов библиотеки
%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Правило для очистки сборочных файлов
clean:
	rm -f $(TARGET) $(LIBRARY) $(LIB_OBJS) $(APP_OBJS)

# Правило для повторной сборки проекта
rebuild: clean all

# Дополнительные полезные правила
run: $(TARGET)
	LD_LIBRARY_PATH=. ./$(TARGET)

.PHONY: all clean rebuild run
