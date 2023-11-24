/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{3}
* FindComponent.c
* Main File, runs strongly connected edge finder using Graph ADT
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
    if (fscanf (in, "%d", &numVertices) == 1) {
        G = newGraph (numVertices);
    } 
    int source = 0;
    int target = 0;
    while (fscanf (in, "%d %d", &source, &target) == 2) {
        if (source == 0 && target == 0) break;
        addArc (G, source, target);
    } fprintf (out, "Adjacency list representation of G:\n");
    printGraph (out, G);
    Graph gT = transpose (G);
    List stack = newList ();
    for (int i = 1; i <= getOrder (G); ++i) {
        append (stack, i);
    } fprintf (out, "\n");
    DFS (G, stack);
    DFS (gT, stack);
    int numOfSCC = 0;
    for (moveFront (stack); index (stack) >= 0; moveNext (stack)) {
        if (getParent (gT, get (stack)) == NIL) ++numOfSCC;
    } fprintf (out, "G contains %d strongly connected components:\n", numOfSCC);
    int i = 1;
    List tempStack = newList ();
    for (moveBack (stack); index (stack) >= 0; movePrev (stack)) {
        if (getParent (gT, get (stack)) != NIL) {
            prepend (tempStack, get (stack));
        } else {
            prepend (tempStack, get (stack));
            if (i <= numOfSCC)  fprintf (out, "Component %d: ", i++);
            for (moveFront (tempStack); index (tempStack) >= 0; moveNext (tempStack)) {
                fprintf (out, "%d ", get (tempStack));
            } fprintf (out, "\n");
            clear (tempStack);
        }
    } freeList (&tempStack);
    freeList (&stack); 
    freeGraph (&G);
    freeGraph (&gT);
    fclose (in);
    fclose (out);
}
