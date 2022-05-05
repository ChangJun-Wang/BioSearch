#ifndef NODE_H
#define NODE_H

#include <vector>
using namespace std;

class Node
{
public:
    // constructor and destructor
    Node(string& name) :
        _name(name) {
        _partCount[0] = 0; _partCount[1] = 0;
    }
    ~Node()  { }

    // basic access methods
    string getName()           const { return _name; }
    int getPartCount(int part) const { return _partCount[part]; }

    // set functions
    void setName(const string name) { _name = name; }
    void setPartCount(int part, const int count) { _partCount[part] = count; }

    // modify methods
    void addUpEdge(int part)     { ++_partCount[part]; }
    void decPartCount(int part)     { --_partCount[part]; }

private:
    int             _partCount[2];
    string          _name;
    vector<int>     _upEdge;
    vector<int>     _downEdge;
    vector<int>     _catEdge;
};

#endif  // NODE_H
