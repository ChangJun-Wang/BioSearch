#ifndef PATH_H
#define PATH_H

#include <vector>
using namespace std;

class Path
{
public:
    // Constructor and destructor
    Path() { }
    ~Path() { }

    // Basic access methods
    vector<int> getRec() const   { return _PathRec; }
    vector<int> getEnz() const   { return _PathEnz; }
    vector<int> getNode() const  { return _PathNode; }
    vector<int> getRel() const   { return _Related; }

    // Set functions
    // void setRec(vector<int> rec)  { _PathRec = rec; }
    // void setEnz(vector<int> enz)  { _PathEnz = enz; }
    // void setNode(vector<int> node) { _PathNode = node; }
    // void setRel(vector<int> related)  { _Related = related; }

    void setRec(vector<int> rec)  { for (const auto& i: rec) _PathRec.push_back(i); }
    void setEnz(vector<int> enz)  { for (const auto& i: enz) _PathEnz.push_back(i); }
    void setNode(vector<int> node) { for (const auto& i: node) _PathNode.push_back(i); }
    void setRel(vector<int> related)  { for (const auto& i: related) _Related.push_back(i); }

    // Add functions
    void addRec(const int& rec)  { _PathRec.push_back(rec); }
    void addEnz(const int& enz)  { _PathEnz.push_back(enz); }
    void addNode(const int& node) { _PathNode.push_back(node); }
    void addRel(const int& related)  { _Related.push_back(related); }

private:
    vector<int>       _PathRec;
    vector<int>       _PathEnz;
    vector<int>       _PathNode;
    vector<int>       _Related;
};

#endif  // PATH_H
