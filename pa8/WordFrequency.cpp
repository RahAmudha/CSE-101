/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{8}
* WordFrequency.cpp
* Main file that creates Dictionary and fills it with frequency of words
***/ 

#include <cctype>
#include <cstdio>
#include <iostream>
#include<fstream>
#include <string>
#include"Dictionary.h"

using namespace std;

int main(int argc, char * argv[]){

    ifstream in;
    ofstream out;
    string line;
    keyType s;
    int line_count;

    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
    size_t len, begin, end;

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

    // read each line of input file, then count and print tokens 
    line_count = 0;
    while( getline(in, line) )  {
        line_count++;
        len = line.length();

        // get first token
        begin = min(line.find_first_not_of(delim, 0), len);
        end   = min(line.find_first_of(delim, begin), len);
        s = line.substr(begin, end-begin);

        for (int j = 0; j < (int) s.length(); ++j) s[j] = tolower(s[j]);
        if (s != "") {
            if (D.contains(s)) ++D.getValue(s);
            else D.setValue(s, 1);
        }
        
        while( s!="" ){  // we have a token
            // get next token
            begin = min(line.find_first_not_of(delim, end+1), len);
            end   = min(line.find_first_of(delim, begin), len);
            s = line.substr(begin, end-begin);

            for (int j = 0; j < (int) s.length(); ++j) s[j] = tolower(s[j]);
            if (s != ""){
                if (D.contains(s)) ++D.getValue(s);
                else D.setValue(s, 1);
            }
        }
    }

    out << D << endl;
    in.close();
    out.close();
}