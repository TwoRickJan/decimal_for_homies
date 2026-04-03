# =========================== ПЕРЕМЕННЫЕ ===========================
TARGET = s21_decimal
CC = gcc
CFLAGS = -std=c11 
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
OS = $(shell uname -s)

# =========================== БИБЛИОТЕКИ ===========================
ifeq ($(OS), Linux)
    CHECK_LIBS = -lcheck -lm -lsubunit -lpthread
    GCOV_LIBS = -lcheck -lm -lsubunit -lpthread -lgcov
else
    CHECK_LIBS = -lcheck -lm
    GCOV_LIBS = -lcheck -lm -lgcov
endif

# =========================== ФАЙЛЫ ===========================
SRC = s21_decimal.c
OBJ = $(SRC:.c=.o)
TEST_SRC = tests/test_s21_decimal.c
TEST_OBJ = $(TEST_SRC:.c=.o)

# =========================== ЦЕЛИ ===========================
all: s21_decimal.a

# Статическая библиотека
s21_decimal.a: $(OBJ)
	ar rcs $@ $^
	ranlib $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка и запуск тестов
test: $(TEST_OBJ) s21_decimal.a
	$(CC) $(CFLAGS) $(TEST_OBJ) s21_decimal.a $(CHECK_LIBS) -o test_s21_decimal
	./test_s21_decimal
	rm -f test_s21_decimal

# Отчет о покрытии
gcov_report: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC) $(TEST_SRC) $(GCOV_LIBS) -o test_gcov
	./test_gcov
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report
	rm -f *.gcda *.gcno *.gcov test_gcov coverage.info

# Проверка стиля
style:
	clang-format -n *.c *.h tests/*.c tests/*.h

# Исправление стиля
style_fix:
	clang-format -i *.c *.h tests/*.c tests/*.h

# Очистка
clean:
	rm -f $(OBJ) $(TEST_OBJ) s21_decimal.a test_s21_decimal test_gcov
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -rf gcov_report

# Установка зависимостей (Linux)
install_libs:
	sudo apt install check lcov -y

# Удаление зависимостей
remove_libs:
	sudo apt remove check lcov -y

# =========================== PHONY ===========================
.PHONY: all test gcov_report style style_fix clean install_libs remove_libs