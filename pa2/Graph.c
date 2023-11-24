/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{2}
* Graph.c
* Implementation of Graph ADT
***/ 


#include "List.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct GraphObj {
    List *adjLists;
    char *colors;
    int *parentNodes;
    int *distances;
    int order;
    int size;
    int lastVertexUsed;
} GraphObj;
/*** Constructors-Destructors ***/
Graph newGraph (int n) {
    Graph g = malloc (sizeof (GraphObj));
    g->adjLists = malloc ((n + 1) * sizeof (List));
    for (int i = 1; i <= n; ++i) {
        g->adjLists [i] = newList ();
    } 
    g->colors = calloc (n + 1, sizeof (char **));
    g->parentNodes = calloc (n + 1, sizeof (int));
    g->distances = calloc (n + 1, sizeof (int));
    g->order = n;
    g->size = 0;
    g->lastVertexUsed = NIL;
    return g;
}
void freeGraph (Graph* pG) {
    for (int i = 1; i <= getOrder (*pG); ++i) {
        freeList (&((*pG)->adjLists [i]));
    } free ((*pG)->adjLists);
    free ((*pG)->colors);
    free ((*pG)->parentNodes);
    free ((*pG)->distances);
    free (*pG);
    pG = NULL;
}
/*** Access functions ***/
int getOrder (Graph G) {
    return (G->order);
}
int getSize (Graph G) {
    return (G->size);
}
int getSource (Graph G) {
    return (G->lastVertexUsed);
}
int getParent (Graph G, int u) {
    if (getSource (G) == NIL) return NIL;
    if (u < 1 || u > getOrder (G)) exit (0);
    return (G->parentNodes [u]);
}
int getDist (Graph G, int u) {
    if (getSource (G) == NIL) return INF;
    if (u < 1 || u > getOrder (G)) exit (0);
    return (G->distances [u]);
}
void getPath (List L, Graph G, int u) {
    if (getSource (G) == NIL) return;
    if (u < 1 || u > getOrder (G)) return;
    if (u == getSource (G)) {
        append (L, getSource (G));
    } else if (getParent (G, u) == NIL) {
        append (L, NIL);
    } else {
        getPath (L, G, getParent (G, u));
        append (L, u);
    }
}
/*** Manipulation procedures ***/
void makeNull (Graph G) {
    for (int i = 1; i <= getOrder (G); ++i) {
        G->distances [i] = 0;
    } G->size = 0;
    G->lastVertexUsed = NIL;
}
void addEdge (Graph G, int u, int v) {
    addArc (G, u, v);
    addArc (G, v, u);
    G->size--;
}
bool arcExists (List L, int v) {
    for (moveFront (L); get (L) > 0; moveNext (L)) {
        if (get (L) == v) {
            return true;
        }
    } return false;
}
void addArc (Graph G, int u, int v) {
    if (u < 1 || v < 1 || u > getOrder (G) || v > getOrder (G)) return;
    List L = G->adjLists [u];
    if (arcExists (L, v)) return;

    if (v > back (L) || front (L) < 0) {
        append (L, v);
        ++(G->size);
        return;
    }
    for (moveFront (L); get (L) > 0; moveNext (L)) {
        if (v < get (L)) {
            insertBefore (L, v);
            ++(G->size);
            return;
        }
    }
}
void BFS (Graph G, int s) {
    for (int v = 1; v <= getOrder (G); ++v) {
        if (v != s) {
            G->colors [v] = 'w';
            G->distances [v] = INF;
            G->parentNodes [v] = NIL;
        }
    }
    G->colors [s] = 'g';
    G->distances [s] = 0;
    G->parentNodes [s] = NIL;
    G->lastVertexUsed = s;
    List Queue = newList ();
    append (Queue, s);
    while (length (Queue) > 0) {
        int dequeued = front (Queue);
        deleteFront (Queue);
        List neighbors = G->adjLists [dequeued];
        for (moveFront (neighbors); index (neighbors) >= 0; moveNext (neighbors)) {
            int y = get (neighbors);
            if (G->colors [y] == 'w') {
                G->colors [y] = 'g';
                G->distances [y] = getDist (G, dequeued) + 1;
                G->parentNodes [y] = dequeued;
                append (Queue, y);
            }
        } G->colors [dequeued] = 'b';
    } freeList (&Queue);
}
/*** Other operations ***/
void printGraph (FILE* out, Graph G) {
    for (int v = 1; v <= getOrder (G); ++v) {
        fprintf (out, "%d: ", v);
        printList (out, G->adjLists [v]);
        fprintf (out, "\n");
    }
}