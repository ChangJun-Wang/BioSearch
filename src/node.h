#ifndef NODE_H
#define NODE_H

#include <vector>
#include "path.h"
using namespace std;

class Node
{
public:
    // constructor and destructor
    Node(string& name) :
        _name(name) {
        for (int i = 0; i < 7; ++i) _label[i] = 0;
        _layer = 0;
        }
    ~Node()  { }

    // basic access methods
    string getName()             const { return _name; }
    vector<int> getUpEdge( )     { return _upEdge; }
    vector<int> getDownEdge( )   { return _downEdge; }
    vector<int> getCatEdge( )    { return _catEdge; }
    int getLabel(int i)          { return _label[i]; }
    int getLayer()               { return _layer;}
    vector<Path*> getPath()      { return _pathArray;}


    // set functions
    void setLayer(int laynum)    { _layer = laynum;}


    // modify methods
    void addUpEdge(int edge)     { _upEdge.push_back(edge); }
    void addDownEdge(int edge)   { _downEdge.push_back(edge); }
    void addCatEdge(int edge)    { _catEdge.push_back(edge); }
    void addLabel(int i)         { _label[i]++; }
    void addPath(Path* path)     { _pathArray.push_back(path); }

    void clearPath()                   { _pathArray.clear(); }
    void clearRecA()                   { _recordA.clear(); }
    void clearRecA1()                  { _recordA1.clear(); }
    void clearRecA2()                  { _recordA2.clear(); }
    void clearLayer()                  { _layer = 0; }
    void mergeAll(int const type)      { for (const auto& i: _pathArray) _recordA.push_back(i); }
    void copyToPath(int const type) { 
        if (type == 0) {
            for (const auto& i: _recordA) _pathArray.push_back(i);
        }
        else if (type == 1) {
            for (const auto& i: _recordA1) _pathArray.push_back(i);
        }
        else if (type == 2) {
            for (const auto& i: _recordA2) _pathArray.push_back(i);
        }
    }
    void copyToRecord(int const type)  {
        if (type == 0) {
            for (const auto& i: _pathArray) _recordA.push_back(i);
        }
        else if (type == 1) {
            for (const auto& i: _pathArray) _recordA1.push_back(i);
        }
        else if (type == 2) {
            for (const auto& i: _pathArray) _recordA2.push_back(i);
        }
    }

private:
    string          _name;
    int             _layer;
    vector<int>     _upEdge;
    vector<int>     _downEdge;
    vector<int>     _catEdge;
    int             _label[7];
    vector<Path*>   _pathArray;
    vector<Path*>   _recordA;
    vector<Path*>   _recordA1;
    vector<Path*>   _recordA2;
};

#endif  // NODE_H
