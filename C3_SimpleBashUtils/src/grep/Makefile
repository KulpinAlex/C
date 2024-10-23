CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

OS = $(shell uname)
ifeq ($(OS), Linux)
	CFLAGS_TEST = `pkg-config --cflags --libs check`
else
	CFLAGS_TEST = -lm -lcheck -lpthread
endif

.PHONY: s21_grep

all: build

build: s21_grep

rebuild: clean build

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

style:
	cp ../../materials/linters/.clang-format ../.
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm -rf ../.clang-format
	
check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

clean:
	rm -rf result ./s21_grep .clang-format ../clang-format ./s21_grep_test 
	rm -rf s21_test log.txt s21_original grep.txt s21_grep.txt

leaks: s21_grep
ifeq ($(OS), Linux)
	valgrind --leak-check=full ./s21_grep -e txt Makefile
else
	leaks -atExit -- ./s21_grep -e txt Makefile
endif

test: test_grep clean


test_grep: s21_grep
	$(CC) $(CFLAGS) s21_grep_test.c -o s21_grep_test $(CFLAGS_TEST)
	./grep_test.sh
	./s21_grep_test