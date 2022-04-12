#ifndef EDGE_H
#define EDGE_H

#include <vector>
using namespace std;

class Edge
{
public:
    // Constructor and destructor
    Edge(const int& id) :
        _id(id), _prev(NULL), _next(NULL) { }
    ~Edge() { }

    // Basic access methods
    int getId() const       { return _id; }
    Edge* getPrev() const   { return _prev; }
    Edge* getNext() const   { return _next; }

    // Set functions
    void setId(const int& id) { _id = id; }
    void setPrev(Edge* prev)  { _prev = prev; }
    void setNext(Edge* next)  { _next = next; }

private:
    int         _id;    // id of the node (indicating the cell)
    Edge*       _prev;  // pointer to the previous node
    Edge*       _next;  // pointer to the next node
};