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
        initialize();
    }
    ~Searcher() {
        clear();
    }

    // basic access methods

    // modify method
    void parseInput(fstream& inFile);
    void initialize();
    void search(char* input_species);
    void SearchUp(int startNode, const int search_limit);
    vector<int> SearchDown(int startNode, const int Target_type, const int search_limit);
    vector<int> mergeB(vector<int> candB, vector<int> candBside);
    vector<int> Check(vector<int> cand);
    void found(int curId, const int Target_type, vector<int>* candidate);
    void BuildStart();
    void ClearPath();
    void ClearPathA();
    void ClearPathA1();
    void ClearPathA2();
    void AddPath(int curId, int downRec, int nextId);

    // member functions about reporting
    void printSummary() const;
    void report() const;
    void writeResult(fstream& outFile);

private:
    int                 _nodeNum;        // number of nets
    int                 _edgeNum;       // number of cells
    int                 _label[5];

    int                 _inNode[2];
    int                 _outNode;

    vector<Node*>       _nodeArray;      // net array of the circuit
    vector<Edge*>       _edgeArray;     // cell array of the circuit
    map<string, int>    _nodeName2Id;    // mapping from net name to id
    map<string, int>    _edgeName2Id;   // mapping from cell name to id

    // Clean up partitioner
    void clear();
};

#endif  // SEARCHER_H
