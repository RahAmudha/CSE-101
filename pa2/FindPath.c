/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{2}
* FindPath.c
* Main File, runs shortest path finder algorithm using a Graph ADT
***/ 

#include "Graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char * argv[]) {
    FILE *in = NULL;
    FILE *out = NULL;

    // check command line for correct number of arguments
    if (argc != 3) {
       printf ("Usage: %s <input file> <output file>\n", argv [0]);
       exit (1);
    } // open files for reading and writing 
    in = fopen (argv [1], "r");
    if (in == NULL) {
       fprintf(stderr, "Unable to open file %s for reading\n", argv [1]);
       exit (1);
    } out = fopen (argv [2], "w");
    if (out == NULL) {
       fprintf (stderr, "Unable to open file %s for writing\n", argv [2]);
       exit (1);
    } 
    int numVertices;
    Graph G;
    if (fscanf (in, "%d", &numVertices) == 1) G = newGraph (numVertices);
    int source = 0;
    int target = 0;
    while (fscanf (in, "%d %d", &source, &target) == 2) {
        if (source == 0 && target == 0) break;
        addEdge (G, source, target);
    } printGraph (out, G);
    fprintf (out, "\n");
    List L = newList ();

    int distance = NIL;

    while (fscanf (in, "%d %d", &source, &target) == 2) {
        if (source == 0 && target == 0) break;
        BFS (G, source);
        getPath (L, G, target);

        if (back (L) > 0) {
            distance = length (L) - 1;
            fprintf (out, "The distance from %d to %d is %d\n", source, target, distance);
            fprintf (out, "A shortest %d-%d path is: ", source, target);
            printList (out, L);
            fprintf (out, "\n\n");
        } else {
            fprintf (out, "The distance from %d to %d is infinity\n", source, target); 
            fprintf (out, "No %d-%d path exists\n", source, target);
        } clear (L);
    } freeList (&L);
    freeGraph (&G);
    fclose (in);
    fclose (out);
}