/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{1}
* Lex.c
* Main File, runs Lexographic sorter using List ADT
***/ 

#include "List.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_LEN 1024

int main(int argc, char * argv[]) {

    int line_count;
    FILE *in = NULL;
    FILE *out = NULL;
    char line [MAX_LEN];

    // check command line for correct number of arguments
    if (argc != 3) {
       printf ("Usage: %s <input file> <output file>\n", argv [0]);
       exit (1);
    } // open files for reading and writing 
    in = fopen (argv [1], "r");
    if (in == NULL) {
       fprintf(stderr, "Unable to open file %s for reading\n", argv [1]);
       exit (1);
    }
    out = fopen (argv [2], "w");
    if (out == NULL) {
       fprintf (stderr, "Unable to open file %s for writing\n", argv [2]);
       exit (1);
    }
    line_count = 0;
    while (fgets (line, MAX_LEN, in) != NULL)  {
        line_count++;
    }
    if (line_count == 0) {
        exit (1);
    }
    char **strarr = calloc (line_count, sizeof (char *));
    rewind (in);

    for (int i = 0; fgets (line, MAX_LEN, in) != NULL; i++)  {
        line [strcspn (line, "\n")] = 0;
        int len = strlen (line) + 1;
        char *str = (char *) malloc (sizeof (char) * len);
        strcpy (str, line);
        strarr [i] = str;
    }

    fclose (in);

    List L = newList ();
    
    prepend (L, 0);
    moveFront (L);
    for (int i = 1; i < line_count; i++) {
        moveFront (L);
        while (index (L) >= 0) {
            if (strcmp (strarr [get (L)], strarr [i]) > 0) {
                break;
            } moveNext (L);
        } if (index (L) < 0) append (L, i);
        else insertBefore (L, i);
    } moveFront (L);
    
    for (int k = 0; k < line_count; ++k) {
        fprintf (out, "%s\n", strarr [get(L)]);
        moveNext (L);
    }
    fclose (out);

    for (int i = 0; i < line_count; i++) {
        free (strarr [i]);
    } free (strarr);
    freeList (&L);

    
}
