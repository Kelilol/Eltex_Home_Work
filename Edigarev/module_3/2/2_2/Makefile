# Имя компилируемого файла
TARGET = random_numbers

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Исходные файлы
SOURCES = random_numbers.c

# Правило по умолчанию: компиляция программы
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Правило для очистки сгенерированных файлов
clean:
	rm -f $(TARGET) *.o

# Правило для повторной компиляции
rebuild: clean all

# Устанавливаем правила для .PHONY целей
.PHONY: all clean rebuild
