#ifndef EDGE_H
#define EDGE_H

#include <vector>
using namespace std;

class Edge
{
public:
    // Constructor and destructor
    Edge(const int& id) :
        _id(id) { }
    ~Edge() { }

    // Basic access methods
    int getId() const       { return _id; }
    vector<int> getRea() const   { return _toRea; }
    vector<int> getPro() const   { return _toPro; }
    int         getEnz() const   { return _toEnz; }

    // Set functions
    void setId(const int& id) { _id = id; }
    void setEnz(const int& enz)  { _toEnz = enz; }
    void addRea(const int& rea)  { _toRea.push_back(rea); }
    void addPro(const int& pro)  { _toPro.push_back(pro); }

private:
    int               _id;
    bool              _visited;
    vector<int>       _toRea;  
    vector<int>       _toPro; 
    int               _toEnz;
    vector<int>       _label[5];
};

#endif  // EDGE_H
