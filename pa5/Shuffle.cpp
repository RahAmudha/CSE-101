/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{5}
* Shuffle.cpp
* Shuffle program that calculates number of shuffles for number of cards in a deck using List ADT
***/ 
#include <string>
#include "List.h"

using namespace std;

#define MAX_LEN 300

int Shuffle(List L);

int main(int argc, char * argv[]){
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " Shuffle Number" << endl;
        return(EXIT_FAILURE);
    } int num_cards = stoi (argv [1]);
    List A = List();
    cout << "deck size\tshuffle count" << endl
         << "------------------------------" << endl;
    for (int i = 1; i <= num_cards; ++i) {
        A.insertBefore(i);
        cout << " " << i;
        cout << "\t\t " << Shuffle (A) << endl;
    } return(EXIT_SUCCESS);
}

int Shuffle(List L) {
    if (L.length() == 1) return L.length();
    List copyL = List (L);
    int shuffles = 0;
    while (true) {
        List firstHalf = List();
        int halfwaypoint = L.length() / 2;
        L.moveFront();
        for (int i = 0; i < halfwaypoint; ++i) {
            firstHalf.insertAfter(L.front());
            firstHalf.moveNext();
            L.eraseAfter();
        } firstHalf.moveFront();
        L.moveFront();
        for (int i = 0; i < halfwaypoint; ++i) {
            L.moveNext();
            L.insertBefore(firstHalf.front());
            firstHalf.eraseAfter();
        } ++shuffles;
        if (L == copyL) break;
    } return shuffles;
}
