#ifndef PATH_H
#define PATH_H

#include <vector>
using namespace std;

class Path
{
public:
    // Constructor and destructor
    Path(const int& id) :
        _id(id) { }
    ~Path() { }

    // Basic access methods
    int         getId()  const   { return _id; }

    vector<int> getRec() const   { return _PathRec; }
    vector<int> getEnz() const   { return _PathEnz; }
    vector<int> getNode() const  { return _PathNode; }
    vector<int> getRel() const   { return _Related; }

    // Set functions
    void setId (const int& id)   { _id = id; }

    // Add functions
    void addRec(const int& rec)  { _PathRec.push_back(rec); }
    void addEnz(const int& enz)  { _PathEnz.push_back(enz); }
    void addNode(const int& node) { _PathNode.push_back(node); }
    void addRel(const int& related)  { _Related.push_back(related); }

private:
    int               _id;

    vector<int>       _PathRec;
    vector<int>       _PathEnz;
    vector<int>       _PathNode;
    vector<int>       _Related;
};

#endif  // PATH_H
