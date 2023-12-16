/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{8}
* Order.cpp
* Main file that creates Dictionary and prints it out
***/ 

#include <cstdio>
#include<fstream>
#include"Dictionary.h"

using namespace std;

int main(int argc, char * argv[]){

    ifstream in;
    ofstream out;

    // check command line for correct number of arguments
    if (argc != 3){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    in.open(argv[1]);
    if (!in.is_open()){
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if (!out.is_open()){
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }

    Dictionary D = Dictionary();
    keyType s;
    for (int i = 1; in >> s; ++i){
        D.setValue(s, i);
    }

    out << D << endl;
    out << D.pre_string();
    in.close();
    out.close();
}