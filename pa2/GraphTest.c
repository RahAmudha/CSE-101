/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{2}
* GraphTest.c
* Testing Client for Graph ADT
***/ 
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main (int argc, char* argv []) {
   int i, n = 35;
   Graph G = NULL;
   List L = newList ();

   G = newGraph(n);
   for (i = 1; i < n; i++) {
      if (i % 7 != 0) addEdge (G, i, i + 1);
      if (i <= 28) addEdge (G, i, i + 7);
   }
   addEdge (G, 9, 31);
   addEdge (G, 17, 13);
   addEdge (G, 14, 33);
   addEdge (G, 5, 4);

   BFS (G, 3);
   printGraph (stdout, G);
   printf ("\n");


   getPath (L, G, 4);
   printf ("The shortest path to 4 from 3 is: ");
   printList (stdout, L);
   printf ("\n");

   freeGraph(&G);
   freeList (&L);

   return(0);
}