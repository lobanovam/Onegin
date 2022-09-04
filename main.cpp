#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
size_t getline (char **s, size_t *n, FILE *stream);
const int defaultBuffSize = 10;
int my_strcmp (const char *s1, const char *s2);

int main() {
    FILE *read = NULL;
    FILE *write = NULL;
    size_t lines_count = 0;
    size_t arr_size = defaultBuffSize;

    if ((read = fopen("test.txt", "r")) == NULL) {
        printf("cant't open file\n");
        return 0;
    }

    char** arr = NULL;

    printf("trying to calloc with pointer %p\n", arr);
    arr = (char**)(calloc(arr_size, sizeof(char*)));
    printf("calloc done with pointer %p\n\n", arr );




    while (!feof(read)) {
        size_t n = 5;
        char *line = nullptr;

        line = (char*)(calloc(n, sizeof(char)));

        n = getline(&line, &n, read);
        if (line[0] == '\0')
            continue;

        arr[lines_count] = line;
        printf("%s of len %d was read\n", arr[lines_count], n);
        lines_count++;

        if (lines_count == arr_size) {
            arr_size *= 2;
            printf("\ntrying to realloc with pointer %p\n", arr);
            arr = (char**)(realloc(arr, arr_size * sizeof(char*)));
            printf("realloc done with pointer %p\n\n", arr );

        }

    }

    fclose(read);
    printf("\nread file is closed. total lines (not empty) %d\n", lines_count);

    for (int i = 0; i < lines_count; i++) {
        for (int j = i + 1; j < lines_count; j++) {
            if (my_strcmp(arr[i], arr[j]) > 0) {
                char* str = arr[i];
                arr[i] = arr[j];
                arr[j] = str;
            }
         }
    }
    printf("sort completed\n");

    if ((write = fopen("result.txt", "w")) == NULL) {
        printf("cant't open file\n");
        return 0;
    }

    for (int i = 0; i < lines_count; i++) {
        fputs(arr[i], write);
        fputs("\n", write);
    }
    free(arr);
    fclose(write);
    printf("write file is closed. Sort completed\n");


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
    //printf("\n\ngetline is working\n");
    size_t i = 0;
    while ((ch = fgetc(fp))!= '\n' && ch != EOF) {
        (*s)[i] = ch;
        //printf("(*s)[%d] is %c\n",i, (*s)[i]);
        i++;
        //printf("i is %d and n is %d\n", i, *n);
        if (i == *n) {

            //printf("i exceeding n, time to realloc\n");
            *n *= 2;
            //size_t len = strlen(*s);
           // printf("Trying Realloc for size %d with ptr = %p\n", *n, *s);
             *s= (char*)(realloc(*s, *n * sizeof(char)));
           // printf("Realloc finished for size %d with ptr = %p\n", *n, *s);

            //printf("strlen finished\n");

            (*s)[i] = '\0';
            //printf("adding 0 finished\n");

        }
    }
    (*s)[i] = '\0';
    //*s = (char*)(realloc(*s, strlen(*s)));
    //printf("final len is %d\n", strlen(*s));
    //printf("final version is %s\n", *s);
    *n = i;
    return *n;
}

int my_strcmp (const char *s1, const char *s2) {
    assert(s1 != nullptr);
    assert(s2 != nullptr);

    while (!isalpha(*s1)) {
                s1++;
        }
    while (!isalpha(*s2)) {
                s2++;
        }

    while (*s1 == *s2 && *s1 != '\0' ) {
            s1++;
            s2++;
            while (!isalpha(*s1) && *s1 != '\0') {
                s1++;
            }
            while (!isalpha(*s2) && *s2 != '\0') {
                s2++;
            }

        }

return (*s1 - *s2);
}







