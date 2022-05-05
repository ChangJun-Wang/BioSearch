#ifndef SEARCHER_H
#define SEARCHER_H

#include <fstream>
#include <vector>
#include <map>
#include "edge.h"
#include "node.h"
using namespace std;

class Searcher
{
public:
    // constructor and destructor
    Searcher(fstream& inFile) :
    _nodeNum(0), _edgeNum(0) {
        parseInput(inFile);
    }
    ~Searcher() {
        clear();
    }

    // basic access methods

    // modify method
    void parseInput(fstream& inFile);
    void search();

    // member functions about reporting
    void printSummary() const;
    void report() const;
    void writeResult(fstream& outFile);

private:
    int                 _nodeNum;        // number of nets
    int                 _edgeNum;       // number of cells

    vector<Node*>       _nodeArray;      // net array of the circuit
    vector<Edge*>       _edgeArray;     // cell array of the circuit
    map<string, int>    _nodeName2Id;    // mapping from net name to id
    map<string, int>    _edgeName2Id;   // mapping from cell name to id

    // Clean up partitioner
    void clear();
};

#endif  // SEARCHER_H
