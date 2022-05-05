#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;

class Node
{
public:
    // constructor and destructor
    Node(string& name) :
        _name(name) { }
    ~Node()  { }

    // basic access methods
    string getName()             const { return _name; }
    vector<int> getUpEdge( )     { return _upEdge; }
    vector<int> getDownEdge( )   { return _downEdge; }
    vector<int> getCatEdge( )    { return _catEdge; }

    // set functions

    // modify methods
    void addUpEdge(int edge)     { _upEdge.push_back(edge); }
    void addDownEdge(int edge)   { _downEdge.push_back(edge); }
    void addCatEdge(int edge)    { _catEdge.push_back(edge); }

private:
    string          _name;
    vector<int>     _upEdge;
    vector<int>     _downEdge;
    vector<int>     _catEdge;
    vector<int>     _label[5];
};

#endif  // NODE_H
