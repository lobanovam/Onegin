#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
size_t getline (char **s, size_t *n, FILE *stream);
int my_strcmp (const char *s1, const char *s2);
int isAlpha(const int ch);
void sortLines(char **arr, int lines_count);
void printArr(char **arr, int lines_count, FILE *write);
void readFile(char ***arr, size_t *lines_count_pt, size_t *arr_size_pt, FILE *read);

const int defaultBuffSize = 10;

int main() {
    FILE *read = NULL;
    FILE *write = NULL;
    size_t lines_count = 0;
    size_t arr_size = defaultBuffSize;

    if ((read = fopen("onegin.txt", "r")) == NULL) {
        printf("cant't open file\n");
        return 0;
    }

    char** arr = NULL;

    printf("trying to calloc with pointer %p\n", arr);
    arr = (char**)(calloc(arr_size, sizeof(char*)));
    printf("calloc done with pointer %p\n\n", arr );

    readFile(&arr, &lines_count, &arr_size, read);

    fclose(read);
    printf("\nread file is closed. total lines (not empty) %d, pointer %p\n", lines_count, arr);

    sortLines(arr, lines_count);
    printf("sort completed\n");

    if ((write = fopen("result.txt", "w")) == NULL) {
        printf("cant't open file\n");
        return 0;
    }
    printArr(arr, lines_count, write);

    free(arr);
    fclose(write);
    printf("write file is closed. Job is done\n");


    return 0;
}

size_t getline (char **s, size_t *n, FILE *fp) {
    if (*n == 0) {
        *n = defaultBuffSize;
    }
    if (*s == NULL) {
        *s = (char*)(calloc(*n, sizeof(char)));
    }
    int ch = 0;
    size_t i = 0;
    while ((ch = fgetc(fp))!= '\n' && ch != EOF) {
        (*s)[i] = ch;
        i++;

        if (i == *n) {
            *n *= 2;
            *s= (char*)(realloc(*s, *n * sizeof(char)));
            (*s)[i] = '\0';

        }
    }
    (*s)[i] = '\0';
    *n = i;
    return *n;
}

int my_strcmp (const char *s1, const char *s2) {
    assert(s1 != nullptr);
    assert(s2 != nullptr);

    while (!isAlpha(*s1) && *s1 != '\0') {
                s1++;
        }
    while (!isAlpha(*s2) && *s2 != '\0') {
                s2++;
        }

    while (*s1 == *s2 && *s1 != '\0' ) {
            s1++;
            s2++;
            while (!isAlpha(*s1) && *s1 != '\0') {
                s1++;
            }
            while (!isAlpha(*s2) && *s2 != '\0') {
                s2++;
            }
        }

return (*s1 - *s2);
}

int isAlpha(const int ch) {
    if (ch >= -64 && ch <= -1 || ch == 72 || isalpha(ch)) {
        return 1;
    } else {
        return 0;
    }
}

void sortLines(char **arr, int lines_count) {

    for (int i = 0; i < lines_count; i++) {
        for (int j = i + 1; j < lines_count; j++) {
            if (my_strcmp(arr[i], arr[j]) > 0) {
                char* str = arr[i];
                arr[i] = arr[j];
                arr[j] = str;
            }
         }
    }
}

void printArr(char **arr, int lines_count, FILE *write) {

    for (int i = 0; i < lines_count; i++) {
        fputs(arr[i], write);
        fputs("\n", write);
    }
}

void readFile(char ***arr_pt, size_t *lines_count_pt, size_t *arr_size_pt, FILE *read) {

    while (!feof(read)) {

        size_t n = defaultBuffSize;
        (*arr_pt)[*lines_count_pt] = (char*)(calloc(n, sizeof(char)));

        n = getline(&(*arr_pt)[*lines_count_pt], &n, read);

        if ((*arr_pt)[*lines_count_pt][0] == '\0')
            continue;

        (*lines_count_pt)++;

        if (*lines_count_pt == *arr_size_pt) {
            (*arr_size_pt) *= 2;
            printf("\ntrying to realloc with pointer %p\n", *arr_pt);
            *arr_pt = (char**)(realloc(*arr_pt, *arr_size_pt * sizeof(char*)));
            printf("realloc done with pointer %p\n\n", *arr_pt );
        }
    }
}









