CC= gcc -std=c11
CFLAGS= -Wall -Werror -Wextra

OS= $(shell uname)

ifeq ($(OS), Darwin)
CFLAGS= -Wall -Werror -Wextra
endif

ifeq ($(OS), Linux)
CFLAGS= -Wall -Werror -Wextra -D_GNU_SOURCE
endif

all: s21_grep

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

test: s21_grep
	chmod +x test.sh
	sh test.sh

style:
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c

clean:
	rm -rf s21_grep

rebuild: clean all
