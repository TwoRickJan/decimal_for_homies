TARGET = s21_decimal
CC = gcc
CFLAGS = -std=c11
GCOV_FLAGS = --coverage -fprofile-arcs -ftest-coverage
OS = $(shell uname -s)

ifeq ($(OS), Linux)
    CHECK_LIBS = -lcheck -lm -lsubunit -lpthread
    GCOV_LIBS = -lcheck -lm -lsubunit -lpthread -lgcov
else
    CHECK_LIBS = -lcheck -lm
    GCOV_LIBS = -lcheck -lm -lgcov
endif

SRC = s21_decimal_arith.c \
      s21_decimal_compare.c \
      s21_decimal_convert.c \
      s21_decimal_round.c \
      helpers/s21_decimal_core.c \
      helpers/s21_decimal_math.c \
      helpers/s21_decimal_normalize.c
OBJ = $(SRC:.c=.o)
TEST_SRC = tests/test_s21_decimal.c
TEST_OBJ = $(TEST_SRC:.c=.o)

all: s21_decimal.a

s21_decimal.a: $(OBJ)
	ar rcs $@ $^
	ranlib $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_OBJ) s21_decimal.a
	$(CC) $(CFLAGS) $(TEST_OBJ) s21_decimal.a $(CHECK_LIBS) -o test_s21_decimal
	./test_s21_decimal

gcov_report: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC) $(TEST_SRC) $(GCOV_LIBS) -o test_gcov
	./test_gcov
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report
	rm -f *.gcda *.gcno *.gcov test_gcov coverage.info

style:
	clang-format -n *.c *.h helpers/*.c tests/*.c

style_fix:
	clang-format -i *.c *.h helpers/*.c tests/*.c

clean:
	rm -f $(OBJ) $(TEST_OBJ) s21_decimal.a test_s21_decimal test_gcov
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -rf gcov_report
	rm -f helpers/*.o

install_libs:
	sudo apt install check lcov -y

.PHONY: all test gcov_report style style_fix clean install_libs
