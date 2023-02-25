#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buff_size 999999

struct flags {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;
    int o;
};

int parser(int argc, char *argv[], struct flags *_flags, char *pattern);
void grep_main(struct flags *_flags, int argc, char **argv, char *pattern);
int save_patt_file(char *pattern, char *temp);
void grep_file(struct flags *_flags, char *pattern, char *file);
void filework(struct flags *_flags, FILE *fp, regex_t reg, char *file);

int main(int argc, char *argv[]) {
    struct flags _flags;
    int returnResult = 0;
    char pattern[buff_size] = {0};
    memset(&_flags, 0, sizeof(_flags));
    returnResult = parser(argc, argv, &_flags, pattern);
    if ((argc >= 3) && (returnResult != 1)) {
        grep_main(&_flags, argc, argv, pattern);
    }
    return returnResult;
}

int parser(int argc, char *argv[], struct flags *_flags, char *pattern) {
    int d = 0, result = 0;
    while ((d = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
        if (d == 'e') {
            _flags->e = 1;
            snprintf(pattern, buff_size, "%s", optarg);
        } else if (d == 'i') {
            _flags->i = 1;
        } else if (d == 'v') {
            _flags->v = 1;
        } else if (d == 'c') {
            _flags->c = 1;
        } else if (d == 'l') {
            _flags->l = 1;
        } else if (d == 'n') {
            _flags->n = 1;
        } else if (d == 'h') {
            _flags->h = 1;
        } else if (d == 's') {
            _flags->s = 1;
        } else if (d == 'f') {
            _flags->f = 1;
            snprintf(pattern, buff_size, "%s", optarg);
        } else if (d == 'o') {
            _flags->o = 1;
        } else {
            result = 1;
        }
    }
    return result;
}

void grep_main(struct flags *_flags, int argc, char **argv, char *pattern) {
    char patternL[buff_size] = {0};
    int cols = 0;

    if (!_flags->f && !_flags->e) {
        snprintf(patternL, buff_size, "%s", argv[optind++]);
    }
    if (_flags->f) cols = save_patt_file(patternL, pattern);
    if (!_flags->f && _flags->e) {
        snprintf(patternL, buff_size, "%s", pattern);
    }

    if (cols != -1) {
        int file_c = (argc - optind > 1) ? 1 : 0;
        for (int i = optind; i < argc; i++) {
            if (file_c && !_flags->h && !_flags->l) printf("%s:", argv[i]);
            grep_file(_flags, patternL, argv[i]);
        }
    }
}

int save_patt_file(char *pattern, char *temp) {
    FILE *fp;
    fp = fopen(temp, "r");
    int i = 0;

    if (fp == NULL) {
        i = -1;
    } else {
        int ch;
        while ((ch = fgetc(fp)) != EOF) {
            if (ch == 13 || ch == 10) pattern[i++] = '|';
            if (ch != 13 && ch != 10) pattern[i++] = ch;
        }

        if (pattern[i - 1] == '|') pattern[i - 1] = '\0';
        fclose(fp);
    }
    return (i == -1) ? -1 : (i + 1);
}

void grep_file(struct flags *_flags, char *pattern, char *file) {
    int cflags = (_flags->i) ? REG_ICASE | REG_EXTENDED : REG_EXTENDED;
    regex_t reg;
    FILE *fp;
    fp = fopen(file, "r");

    if (fp == NULL) {
    } else {
        regcomp(&reg, pattern, cflags);
        filework(_flags, fp, reg, file);
        regfree(&reg);
        fclose(fp);
    }
}

void filework(struct flags *_flags, FILE *fp, regex_t reg, char *file) {
    char text[buff_size] = {0};
    regmatch_t pmatch[1];
    int line_matches = 0, nline = 1;

    while (fgets(text, buff_size - 1, fp) != NULL) {
        int status = regexec(&reg, text, 1, pmatch, 0);
        int match = 0;

        if (status == 0 && !_flags->v) match = 1;
        if (status == REG_NOMATCH && _flags->v) match = 1;
        if (match && !_flags->l && !_flags->c && _flags->n)
            printf("%d:", nline);
        if (match && !_flags->l && !_flags->c && !_flags->o) printf("%s", text);

        if (_flags->o && match) {
            for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
                printf("%c", text[i]);
            }
            printf("\n");
        }
        line_matches += match;
        nline++;
    }

    if (_flags->l && line_matches > 0) printf("%s\n", file);
    if (_flags->c && !_flags->l) printf("%d\n", line_matches);
}
