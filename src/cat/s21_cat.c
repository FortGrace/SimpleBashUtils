#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} opt;

static struct option long_option[] = {
    {"number-nonblank", no_argument, 0, 'b'},
    {"e", no_argument, 0, 'e'},
    {"number", no_argument, 0, 'n'},
    {"squeeze-blank", no_argument, 0, 's'},
    {"v", no_argument, 0, 'v'},
    {"V", no_argument, 0, 'V'},
    {"T", no_argument, 0, 'T'},
    {"t", no_argument, 0, 't'},
    {0, 0, 0, 0},
};

void parser(int argc, char* argv[], opt* options);
void reader(int argc, char* argv[], opt* options);
void opt_s(char sym, int* prev);
void opt_b(char sym, int* new_row, int* str_count);
void opt_n(char sym, int* new_row, int* str_count);
void opt_e(char sym);
void opt_t(char* sym);
void opt_v(char* sym);

int main(int argc, char* argv[]) {
    opt options = {0};
    parser(argc, argv, &options);
    reader(argc, argv, &options);

    return 0;
}

void parser(int argc, char* argv[], opt* options) {
    int opt;
    int option_index;
    while ((opt = getopt_long(argc, argv, "+benstvTE", long_option,
                              &option_index)) != -1) {
        switch (opt) {
            case 0:
                break;
            case 'b':
                options->b = 1;
                break;
            case 'e':
                options->e = 1;
                options->v = 1;
                break;
            case 'n':
                options->n = 1;
                break;
            case 's':
                options->s = 1;
                break;
            case 't':
                options->t = 1;
                options->v = 1;
                break;
            case 'v':
                options->v = 1;
                break;
            case 'T':
                options->t = 1;
                break;
            case 'E':
                options->e = 1;
                break;
            default:
                break;
        }
    }
    if (options->b == 1) options->n = 0;
}

void reader(int argc, char* argv[], opt* options) {
    int currentFile = optind;
    int str_count = 1;
    int new_row = 1;
    int prev = 0;
    char sym = 0;
    FILE* fp = NULL;

    while (currentFile < argc) {
        fp = fopen(argv[currentFile], "r");
        if (fp == NULL) {
            printf("%s: No such file or directory\n", argv[currentFile]);
            currentFile++;
        } else {
            while ((sym = fgetc(fp)) != EOF) {
                if (options->b) opt_b(sym, &new_row, &str_count);
                if (options->e) opt_e(sym);
                if (options->v) opt_v(&sym);
                if (options->n) opt_n(sym, &new_row, &str_count);
                if (options->s) {
                    opt_s(sym, &prev);
                    if (prev >= 3) continue;
                }
                if (options->t) opt_t(&sym);
                printf("%c", sym);
            }
            fclose(fp);
            currentFile++;
        }
    }
}
void opt_s(char sym, int* prev) {
    if (sym != '\n')
        *prev = 0;
    else
        *prev += 1;
}

void opt_b(char sym, int* new_row, int* str_count) {
    if (*new_row != 0) {
        if (sym != '\n') {
            printf("%6d\t", (*str_count)++);
            *new_row = 0;
        }
    }
    if (sym == '\n') *new_row = 1;
}

void opt_n(char sym, int* new_row, int* str_count) {
    if (*new_row != 0) {
        printf("%6d\t", (*str_count)++);
        *new_row = 0;
    }
    if (sym == '\n') *new_row = 1;
}

void opt_e(char sym) {
    if (sym == '\n') printf("%c", '$');
}

void opt_t(char* sym) {
    if (*sym == '\t') {
        printf("%c", '^');
        *sym = 'I';
    }
}

void opt_v(char* sym) {
    if (!isprint(*sym) && *sym != '\n' && *sym != '\t') {
        printf("^");
        if (*sym == 127)
            *sym -= 64;
        else
            *sym += 64;
    }
}
