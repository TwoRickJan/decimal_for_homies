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

TEST_SRCS = tests/test_arith.c \
            tests/test_compare.c \
            tests/test_convert.c \
            tests/test_math.c \
            tests/test_normalize.c \
            tests/test_round.c

TEST_BINS = $(TEST_SRCS:.c=)

all: s21_decimal.a

s21_decimal.a: $(OBJ)
	ar rcs $@ $^
	ranlib $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests/%: tests/%.c s21_decimal.a
	$(CC) $(CFLAGS) $< s21_decimal.a $(CHECK_LIBS) -o $@

test: s21_decimal.a $(TEST_BINS)
	@for bin in $(TEST_BINS); do \
		echo "\n=== Running $$bin ==="; \
		./$$bin || exit 1; \
	done
	@echo "\nAll tests passed!"

gcov_report: $(SRC) $(TEST_SRCS) s21_decimal.a
	@for src in $(TEST_SRCS); do \
		bin=$${src%.c}; \
		echo "Building $$bin with coverage..."; \
		$(CC) $(CFLAGS) $(GCOV_FLAGS) $$src s21_decimal.a $(GCOV_LIBS) -o $$bin; \
		echo "Running $$bin..."; \
		./$$bin || exit 1; \
	done
	lcov --capture --directory . --output-file coverage.info --no-external
	genhtml coverage.info --output-directory gcov_report
	rm -f tests/*.gcda tests/*.gcno *.gcda *.gcno

style:
	clang-format -n *.c *.h helpers/*.c tests/*.c

style_fix:
	clang-format -i *.c *.h helpers/*.c tests/*.c

clean:
	rm -f $(OBJ) $(TEST_BINS) s21_decimal.a
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -rf gcov_report
	rm -f helpers/*.o

install_libs:
	sudo apt install check lcov -y

.PHONY: all test gcov_report style style_fix clean install_libs
