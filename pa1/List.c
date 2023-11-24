/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{1}
* List.c
* List ADT Program
***/ 

#include <stdio.h>
#include "List.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct NodeObj *Node;

struct NodeObj {
    int weight;
    Node next;
    Node back;
} NodeObj;

typedef struct ListObj {
    Node head;
    Node tail;
    Node cursor;
    int length;
    int index;
} ListObj;

List newList(void) {
    List L = calloc (1, sizeof (ListObj));
    L->cursor = NULL;
    L->head = NULL;
    L->tail = NULL;
    L->length = 0;
    L->index = -1;
    return L;
}
Node newNode (int x) {
    Node node = calloc (1, sizeof (NodeObj));
    node->weight = x;
    node->back = NULL;
    node->next = NULL;
    return node;
}
void freeList (List* pL) {
    clear (*pL);
    free (*pL);
    pL = NULL;
}
int length (List L) {
    return L->length;
}
int index (List L) {
    if (L->cursor != NULL) {
        return L->index;
    } return -1;
}
int front (List L) {
    if (length (L)) {
        return L->head->weight;
    } return -1;
}
int back (List L) {
    if (length (L)) {
        return L->tail->weight;
    } return -1;
}
int get (List L) {
    if (length (L) && index (L) >= 0)
        return L->cursor->weight;
    return -1;
}
bool equals (List A, List B) {
    if (length (A) == 0 && length (B) == 0) return true;

    Node nodeA = A->head;
    Node nodeB = B->head;
    if (length (A) != length (B)) return false;
    while (nodeA->next && nodeB->next) {
        if (nodeA->weight != nodeB-> weight) {
            return false;
        } 
        nodeA = nodeA->next;
        nodeB = nodeB->next;
    } return true;
}
void clear (List L) {
    while (length (L)) {
        moveFront (L);
        deleteFront (L);
    }
}
void set (List L, int x) {
    if (length (L) && L->index >= 0) {
        L->cursor->weight = x;
    }
}
void moveFront (List L) {
    if (length (L)) {
        L->cursor = L->head;
        L->index = 0;
    }
}
void moveBack (List L) {
    if (length (L)) {
        L->cursor = L->tail;
        L->index = L->length - 1;
    }
}
void movePrev (List L) {
    if (L->index < 0) {
        exit(1);
    }
    if (L->index > 0) {
        //printf ("%d\n", L->cursor->weight);
        L->cursor = L->cursor->back;
        --(L->index);
    } else {
        L->cursor = NULL;
        L->index = -1;
    }
}
void moveNext (List L) {
    if (L->index < 0) {
        exit(1);
    }
    if (L->index < L->length) {
        //printf ("%d\n", L->cursor->weight);
        L->cursor = L->cursor->next;
        ++(L->index); 
    } else {
        L->cursor = NULL;
        L->index = -1;
    }
}
void prepend (List L, int x) {
    Node node = newNode (x);
    if (length (L)) {
        node->next = L->head;
        L->head->back = node;
        L->head = node;
    } else {
        L->tail = node;
        L->head = node;
    } //printf ("%d\n%d\n", L->length, L->index);
    
    ++(L->length);
    if (L->cursor)
    ++(L->index);
}
void append (List L, int x) {
    Node node = newNode (x);
    if (length (L)) {
        node->back = L->tail;
        L->tail->next = node;
        L->tail = node;
    } else {
        L->tail = node;
        L->head = node;
    } //printf ("%d\n%d\n", L->length, L->index);
    
    ++(L->length);
}
void insertBefore (List L, int x) {
    if (!length (L) || index < 0) {
        exit (1);
    } 
    if (index (L) == 0) {
        Node node = newNode (x);
        L->head->back = node;
        node->next = L->head;
        L->head = node;
        ++(L->length);
        ++(L->index);
    } else {
        Node node = newNode (x);
        L->cursor->back->next = node; 
        node->back = L->cursor->back;
        L->cursor->back = node;
        node->next = L->cursor;
        ++(L->length);
        ++(L->index);
    }
}
void insertAfter (List L, int x) {
    if (!length (L) || index < 0) {
        exit (1);
    }
    if (index (L) == length (L) - 1) {
        Node node = newNode (x);
        L->tail->next = node;
        node->back = L->tail;
        L->tail = node;
        ++(L->length);
    }
    else {
        Node node = newNode (x);
        L->cursor->next->back = node;
        node->next = L->cursor->next;
        L->cursor->next = node;
        node->back = L->cursor;
        ++(L->length);
    }
} 
void deleteFront (List L) {
    if (index (L) == 0) {
        L->cursor = NULL;
        L->index = -1;
    }
    if (length (L) > 1) { 
        L->head = L->head->next;
        L->head->back->next = NULL;
        free (L->head->back);
        L->head->back = NULL;
    } else if (length (L) == 1) {
        free (L->head);
    } --(L->length);
    if (L->cursor)
        --(L->index);
}
void deleteBack (List L) {
    if (index (L) == length (L) - 1) {
        L->cursor = NULL;
        L->index = -1;
    }
    if (length (L) > 1) {
        L->tail = L->tail->back;
        L->tail->next->back = NULL;
        free (L->tail->next);
        L->tail->next = NULL;
    } else if (length (L) == 1) {
        free (L->tail);
    } --(L->length);
}
void delete (List L) {
    if (length (L) && index (L) >= 0) {
        if (index (L) == length (L) - 1) {
            L->cursor->back->next = NULL;
            L->tail = L->cursor->back;
            L->cursor->back = NULL;
            free (L->cursor);
            --(L->length);
            L->index = -1;
            return;
        }
        if (index (L) == 0) {
            L->cursor->next->back = NULL;
            L->head = L->cursor->next;
            L->cursor->next = NULL;
            free (L->cursor);
            --(L->length);
            L->index = -1; 
            return;
        }
        if (L->cursor->next && L->cursor->back) {
            L->cursor->next->back = L->cursor->back;
            L->cursor->back->next = L->cursor->next;
            free (L->cursor);
            L->cursor = NULL;
            L->index = -1;
            --(L->length);
        }
    }
}
void printList (FILE* out, List L) {
    Node tempNode = L->head;
    while (tempNode) {
        fprintf (out, "%d ", tempNode->weight);
        tempNode = tempNode->next;
    }
}
List copyList (List L) {
    List newL = newList ();
    if (length (L) == 0) return newL;
    Node node = L->head;
    while (node) {
        append (newL, node->weight);
        node = node->next;
    } return newL;
}