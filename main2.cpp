#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <stdbool.h>
#include "log.h"
#include <assert.h>

int my_strcmp (const void *s1, const void *s2);
int inverse_strcmp(const void *s1, const void *s2);
int isAlpha(const int ch);
void sortLines(char **arr, size_t lines_count, int (*comp)(const void*, const void*));
void printArr(char **arr, int lines_count, FILE *write);
void connectPointers(struct Text* onegin_ptr);
void printBuff (char *buff, int ch_count, FILE *write);
void _swap(char* s1, char* s2);
void readFile(struct Text* onegin_ptr);

const size_t defaultSize = 10;

struct Text {
    char *buff;
    char **pointers;
    size_t pointersSize;
    size_t ch_count;
    size_t lines_count;
};

//typedef struct Text Text;
const char *FILENAME = "onegin.txt";

int main() {
    struct Text onegin;

    //size_t pointersSize = defaultSize;
    /*struct stat file;
    stat(FILENAME, &file);
    size_t buffSize = file.st_size;
    fprintf(LogFile,"buffSize is %ld\n", buffSize);

    FILE *read = NULL;

    if ((read = fopen(FILENAME, "r")) == NULL) {
        fprintf(LogFile,"cant't open file\n");
        return 0;
    }*/

    onegin.buff = (char*)(calloc(buffSize, sizeof(char)));
    onegin.pointers = (char**)(calloc(pointersSize, sizeof(char*)));
    onegin.pointersSize = defaultSize;
    //ASSERT(buff != NULL);
    //ASSERT(pointers != NULL);

    readFile(&onegin);


    //size_t lines_count = 0;

    connectPointers(&onegin);

    fprintf(LogFile,"lines_count is %ld\n", lines_count);

    FILE *write = NULL;

    if ((write = fopen("result.txt", "w")) == NULL) {
        printf("cant't open file\n");
        return 0;
    }
    //ASSERT(write != NULL);

    fprintf(LogFile,"sorting normally\n");
    qsort(pointers, lines_count, sizeof(char*), (int(*) (const void *, const void *)) my_strcmp);
    //sortLines(pointers, lines_count, (int(*) (const void*, const void*)) my_strcmp);
    printArr(pointers, lines_count, write);
    fprintf(LogFile, "normal sort completed\n");

    fprintf(write, "\n\n now inversed\n\n");

    fprintf(LogFile,"sorting inverted\n");
    qsort(pointers, lines_count, sizeof(char*), (int(*) (const void *, const void *)) inverse_strcmp);
    //sortLines(pointers, lines_count, (int(*) (const void*, const void*)) inverse_strcmp);
    printArr(pointers, lines_count, write);
    fprintf(LogFile,"inverted sort completed\n");

    fprintf(write, "\n\n now based\n\n");

    printBuff(buff, ch_count, write);

    fclose(write);

    free(buff);
    for (int i = 0; i < lines_count; i++) {
        free(pointers[i]);
    }
    free(pointers);

}

void sortLines(char **arr, size_t lines_count, int (*comp)(const void*, const void*)) {
    for (size_t i = 0; i < lines_count; i++) {
        for (size_t j = i + 1; j < lines_count; j++) {
            if (comp(&arr[i], &arr[j]) > 0) {
                _swap(arr[i], arr[j]);
            }
        }
    }
}

int my_strcmp (const void *s1, const void *s2) {
    //ASSERT(s1 != NULL);
    //ASSERT(s2 != NULL);

    char* str1 = *((char**) s1);
    char* str2 = *((char**) s2);

    while (!isAlpha(*str1) && *(str1) != '\0') {
                str1 += sizeof(char);
        }
    while (!isAlpha(*str2) && *(str2) != '\0') {
                str2 += sizeof(char);
        }

    while ((*str1 == *str2) && (*str1 != '\0' )) {
            str1 += sizeof(char);
            str2 += sizeof(char);

            while (!isAlpha(*str1) && *(str1) != '\0') {
                str1 += sizeof(char);
            }
            while (!isAlpha(*str2) && *(str2) != '\0') {
                str2 += sizeof(char);
            }

        }

return (*str1 - *str2);
}

int isAlpha(const int ch) {
    if (ch >= -64 && ch <= -1 || ch == 72 || isalpha(ch)) {
        return 1;
    } else {
        return 0;
    }
}

void printArr(char **arr, int lines_count, FILE *write) {
    //ASSERT(arr != NULL);
   // ASSERT(write != NULL);

    for (size_t i = 0; i < lines_count; i++) {
        fputs(arr[i], write);
        fputs("\n", write);
    }
}

void connectPointers(char ***pointers_pt, char *buff, size_t *lines_count_pt, size_t *pointersSize_pt, int ch_count){
    //ASSERT(pointers_pt != NULL);
    //ASSERT(buff != NULL);
    //ASSERT(lines_count_pt != NULL);
    //ASSERT(pointersSize_pt != NULL);

    **pointers_pt = buff;
    for (size_t i = 1, j = 1; i < ch_count; i++) {
        if (buff[i] == '\n') {
            (*lines_count_pt)++;

            buff[i] = '\0';
            (*pointers_pt)[j++] = &(buff[i+1]);

            if (j == *pointersSize_pt) {
                *pointersSize_pt *= 2;
                fprintf(LogFile,"trying to realloc with pointer %p\n", *pointers_pt);
                *pointers_pt = (char**)(realloc(*pointers_pt, *pointersSize_pt * sizeof(char*)));
                fprintf(LogFile,"realloc done with pointer %p\n", *pointers_pt);
            }
        }
    }
}

int inverse_strcmp(const void *s1, const void *s2){
    //ASSERT(s1 != NULL);
    //ASSERT(s2 != NULL);

    char* str1 = *((char**) s1);
    char* str2 = *((char**) s2);

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    const char *v1 = str1 + len1 - 1;
    const char *v2 = str2 + len2 - 1;

    while (!isAlpha(*v1) && v1 > str1) {
                v1--;
        }
    while (!isAlpha(*v2) && v2 > str2) {
                v2--;
        }

        while (*v1 == *v2 && v1 > str1 && v2 > str2) {
            v1--;
            v2--;
            while (!isAlpha(*v1) && v1 > str1) {
                v1--;
            }
            while (!isAlpha(*v2) && v2 > str2) {
                v2--;
            }
        }

        return (*v1 - *v2);
}

void printBuff (char *buff, int ch_count, FILE *write) {
   //ASSERT(buff != NULL);
   // ASSERT(write != NULL);
    for (size_t i = 0; i < ch_count; i++) {
        if (buff[i] == '\0') {
            fputc('\n', write);
        } else {
            fputc(buff[i], write);
        }
    }
}

void _swap(char *s1, char *s2) {
   // ASSERT(s1 != NULL);
    //ASSERT(s2 != NULL);

    char* str = s1;
    s1 = s2;
    s2 = str;
}

void readFile(struct Text *onegin_ptr) {

    struct stat file;
    stat(FILENAME, &file);
    size_t buffSize = file.st_size;

    FILE *read = NULL;
    if ((read = fopen(FILENAME, "r")) == NULL) {
        fprintf(LogFile,"cant't open file\n");
        return 0;
    }
    onegin_ptr->ch_count = fread(onegin_ptr->buff, sizeof(char), buffSize, read);
    fclose(read);



}

