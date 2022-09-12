#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <assert.h>
#include <stdbool.h>
int my_strcmp (const char *s1, const char *s2);
int inverse_strcmp(const char *s1, const char *s2);
int isAlpha(const int ch);
void sortLines(char **arr, int lines_count, bool inverted);
void printArr(char **arr, int lines_count, FILE *write);
void connectPointers(char ***pointers_pt, char *buff, size_t *lines_count, size_t *pointersSize, int ch_count);
void printBuff (char *buff, int ch_count, FILE *write);


const size_t defaultSize = 10;

int main() {

    size_t pointersSize = defaultSize;
    struct stat file;
    stat("onegin.txt", &file);
    size_t buffSize = file.st_size;
    printf("buffSize is %ld\n", buffSize);

    FILE *read = NULL;
    FILE *write = NULL;

    if ((read = fopen("onegin.txt", "r")) == NULL) {
        printf("cant't open file\n");
        return 0;
    }

    char *buff = nullptr;
    char **pointers = nullptr;

    buff = (char*)(calloc(buffSize, sizeof(char)));
    printf("buff calloc done with pointer %p\n", buff);

    pointers = (char**)(calloc(pointersSize, sizeof(char*)));
    printf("pointers calloc done with pointer %p\n", pointers);

    size_t ch_count = fread(buff, sizeof(char), buffSize, read);
    fclose(read);

    size_t lines_count = 0;

    connectPointers(&pointers, buff, &lines_count, &pointersSize, ch_count);

    printf("lines_count is %ld\n", lines_count);

    if ((write = fopen("result.txt", "w")) == NULL) {
        printf("cant't open file\n");
        return 0;
    }

    bool inverted = false;
    printf("sorting normally\n");
    sortLines(pointers, lines_count, inverted);
    printArr(pointers, lines_count, write);
    printf("normal sort completed\n");

    inverted = true;
    printf("sorting inverted\n");
    sortLines(pointers, lines_count, inverted);
    printArr(pointers, lines_count, write);
    printf("inverted sort completed\n");

    printBuff(buff, ch_count, write);

    fclose(write);

    free(buff);
    for (int i = 0; i < lines_count; i++) {
        free(pointers[i]);
    }
    free(pointers);

}

void sortLines(char **arr, int lines_count, bool inverted) {
    if(inverted) {
        for (int i = 0; i < lines_count; i++) {
            for (int j = i + 1; j < lines_count; j++) {
                if (inverse_strcmp(arr[i], arr[j]) > 0) {
                    char* str = arr[i];
                    arr[i] = arr[j];
                    arr[j] = str;
                }
            }
        }
    } else {
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

void printArr(char **arr, int lines_count, FILE *write) {

    for (int i = 0; i < lines_count; i++) {
        fputs(arr[i], write);
        fputs("\n", write);
    }
}

void connectPointers(char ***pointers_pt, char *buff, size_t *lines_count_pt, size_t *pointersSize_pt, int ch_count){
    **pointers_pt = buff;
    for (int i = 1, j = 1; i < ch_count; i++) {
        if (buff[i] == '\n') {
            (*lines_count_pt)++;
            buff[i] = '\0';
            (*pointers_pt)[j++] = &(buff[i+1]);
            if (j == *pointersSize_pt) {
                *pointersSize_pt *= 2;
                printf("trying to realloc with pointer %p\n", *pointers_pt);
                *pointers_pt = (char**)(realloc(*pointers_pt, *pointersSize_pt * sizeof(char*)));
                printf("realloc done with pointer %p\n", *pointers_pt);
            }
        }
    }
}

int inverse_strcmp(const char *s1, const char *s2){
    assert(s1 != nullptr);
    assert(s2 != nullptr);

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    const char *v1 = s1 + len1 - 1;
    const char *v2 = s2 + len2 - 1;

    while (!isAlpha(*v1) && v1 > s1) {
                v1--;
        }
    while (!isAlpha(*v2) && v2 > s2) {
                v2--;
        }

        while (*v1 == *v2 && v1 > s1 && v2 > s2) {
            v1--;
            v2--;
            while (!isAlpha(*v1) && v1 > s1) {
                v1--;
            }
            while (!isAlpha(*v2) && v2 > s2) {
                v2--;
            }
        }

        return (*v1 - *v2);
}

void printBuff (char *buff, int ch_count, FILE *write) {
    for (int i = 0; i < ch_count; i++) {
        if (buff[i] == '\0') {
            fputc('\n', write);
        } else {
            fputc(buff[i], write);
        }
    }
}
