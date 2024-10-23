CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

OS = $(shell uname)
ifeq ($(OS), Linux)
	CFLAGS_TEST = `pkg-config --cflags --libs check`
else
	CFLAGS_TEST = -lm -lcheck -lpthread
endif

.PHONY: s21_cat

all: build

build: s21_cat

rebuild: clean build

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

style:
	cp ../../materials/linters/.clang-format ../.
	clang-format -n *.c *.h
	rm -rf ../.clang-format

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

clean:
	rm -rf result ./s21_cat ./s21_cat_test s21_original s21_test

leaks: s21_cat
ifeq ($(OS), Linux)
	valgrind --leak-check=full ./s21_cat -benst *.txt cat_test.sh
else
	leaks -atExit -- ./s21_cat -benst *.txt
endif

test: test_cat clean

test_cat: s21_cat
	$(CC) $(CFLAGS) s21_cat_test.c -o s21_cat_test $(CFLAGS_TEST)
	./cat_test.sh
	./s21_cat_test
