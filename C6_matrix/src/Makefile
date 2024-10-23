CC = gcc
CFLAGS = -c -Wall -Wextra -Werror -std=c11

ADD_LIBS = -lcheck
RM := -rm -rf
MK := -mkdir -p

PROJECT_NAME = s21_matrix
SRC_DIR = .
OBJ_DIR = ./objs
TESTS_DIR = ./tests
REPORT_DIR = ./report
GCOV = -fprofile-arcs -ftest-coverage

OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	OPEN_CMD = open
	ADD_LIBS +=
	BREW := ${HOME}/homebrew
endif

ifeq ($(OS), Linux)
	OPEN_CMD = xdg-open
	ADD_LIBS += -lm -lpthread -lrt -lsubunit
	BREW = .
endif

PATH := $(BREW)/bin:$(PATH)

SOURCE = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCE:%.c=%.o)))
TESTS = $(wildcard $(TESTS_DIR)/tests_$(PROJECT_NAME).check)
TESTSC = $(addprefix $(TESTS_DIR)/, $(notdir $(TESTS:%.check=%.c)))

include $(wildcard $(OBJ_DIR)/*.d)

.PHONY: default all clean test $(PROJECT_NAME).a check gcov_report valgrind

default: all

all: clean $(PROJECT_NAME).a

$(PROJECT_NAME).a: $(OBJECTS)
	ar rcs $(PROJECT_NAME).a $(OBJECTS)
	ranlib $(PROJECT_NAME).a

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MK) $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -MD $< -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(GCOV) -c -MD $< -o $@

$(TESTS_DIR)/%.c: $(TESTS_DIR)/%.check
	-checkmk clean_mode=1 $< > $@

test: $(PROJECT_NAME).a $(TESTSC)
	$(CC) -o $(TESTS_DIR)/$@_$(PROJECT_NAME) $(TESTSC) $(PROJECT_NAME).a $(ADD_LIBS)
	$(TESTS_DIR)/$@_$(PROJECT_NAME)

check:
	@echo "----- style check -----"
	clang-format -style=google -n *.c *.h
	@echo "----- check done ------"

valgrind: test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./$(TESTS_DIR)/test_$(PROJECT_NAME)

gcov_report: $(SOURCE:%.c=%.o) $(TESTSC)
	@$(MK) $(OBJ_DIR)
	$(CC) $(TESTSC) -c -o $(TESTSC:%.c=%.o)
	$(CC) $(SOURCE:%.c=%.o) $(TESTSC:%.c=%.o) $(GCOV) $(ADD_LIBS) -o test_cov
	./test_cov
	lcov -t "tests_$(PROJECT_NAME)" -o tests_$(PROJECT_NAME).info -c -d .
	genhtml -o $(REPORT_DIR) tests_$(PROJECT_NAME).info
	-$(OPEN_CMD) $(REPORT_DIR)/index-sort-f.html

clean:
	$(RM) $(PROJECT_NAME).a $(OBJ_DIR)/*.d $(TESTS_DIR)/tests_$(PROJECT_NAME).c $(SRC_DIR)/*.d
	$(RM) $(OBJECTS) $(SOURCE:%.c=%.o) $(TESTS_DIR)/tests_$(PROJECT_NAME).o
	$(RM) $(REPORT_DIR) RESULT_VALGRIND.txt ./*.gcno ./*.gcda ./tests_$(PROJECT_NAME).info
	$(RM) test_cov ./$(TESTS_DIR)/test_$(PROJECT_NAME) ./a.out objs

rebuild: clean all

check_clang:
	clang-format-14 -style=google -i *.c *.h ./tests/*.check
