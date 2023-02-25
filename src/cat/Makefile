CC=gcc
CFLAGS= -Wall -Werror -Wextra
OS := $(shell uname)

ifeq ($(OS), Darwin)
CFLAGS= -Wall -Werror -Wextra
TEST= ./test_mac.sh
endif

ifeq ($(OS), Linux)
CFLAGS= -Wall -Werror -Wextra -D_GNU_SOURCE
TEST= ./test_linux.sh
endif

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

test: s21_cat
	chmod +x $(TEST)
	sh $(TEST)

style:
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c

clean:
	rm -rf s21_cat

rebuild: clean all
