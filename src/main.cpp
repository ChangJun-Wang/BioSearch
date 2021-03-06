#include <iostream>
#include <fstream>
#include <vector>
#include "searcher.h"
using namespace std;

int main(int argc, char** argv)
{
    fstream input, output;
    char* caseNum;

    if (argc == 4) {
        input.open(argv[1], ios::in);
        output.open(argv[2], ios::out);
        caseNum = argv[3];
        if (!input) {
            cerr << "Cannot open the input file \"" << argv[1]
                 << "\". The program will be terminated..." << endl;
            exit(1);
        }
        if (!output) {
            cerr << "Cannot open the output file \"" << argv[2]
                 << "\". The program will be terminated..." << endl;
            exit(1);
        }
    }
    else {
        cerr << "Usage: ./bio <input file> <output file>" << endl;
        exit(1);
    }
    

    Searcher* searcher = new Searcher(input);
    searcher->search(caseNum);
    searcher->printSummary();
    searcher->writeResult(output);

    return 0;
}
