#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <cmath>
#include <map>
#include "node.h"
#include "edge.h"
#include "searcher.h"
using namespace std;

vector<int> Searcher::SearchDown(int startNode, const int Target_type, const int search_limit) {
    vector<int>     bfs;
    bfs.push_back(startNode);
    vector<int>     candidate;

    // initializtion for starting node
//     bfs[0].CopyToPath()

    // start to traverse
//     while (bfs != []):
//         startPro = bfs.pop(0)
//         if (Target_type in startPro.label):
//             found(Target_type, startPro, candidate)

//         if startPro.level + 1 <= search_limit:
//             for downRec in startPro.getDownedge():
//                 // check whether this reaction has conflict to previous reactions and input
//                 CheckDownRec = startPro.CheckDownRec(downRec)
//                 if CheckDownRec != []:
//                     for product in downRec.getpro():
//                         if product.layer != 0:
//                             CheckProduct = product.CheckProduct(CheckDownRec)
//                             if CheckProduct != []:
//                                 product.AddPath(startPro, downRec, CheckProduct)
//                                 product.level = startPro.level + 1
//                                 if (product not in bfs and product.level <= search_limit):
//                                     bfs.append(product)
//     return candidate
// }


void Searcher::parseInput(fstream& inFile)
{
    string str;
    cout << "parsing database......";

    // Set up whole graph
    while (inFile >> str) {
        string enzyme = "";
        // cout << str << endl;
        if (str == "RN") {
            string nodeName = "";
            // inFile >> nodeName;
            while (inFile >> nodeName) {
                if (nodeName == ";") break;
                else {
                    // a newly seen node
                    if (_nodeName2Id.count(nodeName) == 0) {
                        int nodeId = _nodeNum;
                        enzyme = nodeName;
                        _nodeArray.push_back(new Node(nodeName));
                        _nodeName2Id[nodeName] = nodeId;
                        ++_nodeNum;
                    }
                }
            }
        }
        else {
            string edgeName, nodeName = "";
            edgeName = str;
            int edgeId = _edgeNum;
            bool is_pro = false;
            _edgeArray.push_back(new Edge(edgeId));
            _edgeName2Id[edgeName] = edgeId;

            int enzId = _nodeName2Id[enzyme];
            _nodeArray[enzId]->addCatEdge(edgeId);
            _edgeArray[edgeId]->setEnz(enzId);
            while (inFile >> nodeName) {
                if (nodeName == ";")  break;
                else if (nodeName == "=") is_pro = true;
                else {
                    // a newly seen node
                    if (_nodeName2Id.count(nodeName) == 0) {
                        int nodeId = _nodeNum;
                        _nodeArray.push_back(new Node(nodeName));
                        _nodeName2Id[nodeName] = nodeId;
                        ++_nodeNum;
                    }

                    // an existed node
                    if (is_pro) {
                        int nodeId = _nodeName2Id[nodeName];
                        _nodeArray[nodeId]->addUpEdge(edgeId);
                        _edgeArray[edgeId]->addPro(nodeId);
                    }
                    else {
                        int nodeId = _nodeName2Id[nodeName];
                        _nodeArray[nodeId]->addDownEdge(edgeId);
                        _edgeArray[edgeId]->addRea(nodeId);
                    }
                }
            }
            ++_edgeNum;
        }
    }
    return;
}

void Searcher::search()
{
}




void Searcher::printSummary() const
{
    cout << endl;
    cout << "==================== Summary ====================" << endl;
    cout << " Total node number: " << _nodeNum << endl;
    cout << " Total edge number:  " << _edgeNum << endl;
    cout << "=================================================" << endl;
    cout << endl;
    return;
}

void Searcher::report() const
{
    for (size_t i = 0, end_i = _edgeArray.size(); i < end_i; ++i) {
        cout << " Reaction " << _edgeArray[i]->getId() << ": ";
        vector<int> nodeList = _edgeArray[i]->getRea();
        for (size_t j = 0, end_j = nodeList.size(); j < end_j; ++j) {
            cout << _nodeArray[nodeList[j]]->getName() << " ";
        }
        
        cout << _nodeArray[_edgeArray[i]->getEnz()]->getName() << " ";
        
        nodeList = _edgeArray[i]->getPro();
        for (size_t j = 0, end_j = nodeList.size(); j < end_j; ++j) {
            cout << _nodeArray[nodeList[j]]->getName() << " ";
        }
        cout << endl;
    }
    return;
}

void Searcher::writeResult(fstream& outFile)
{
    stringstream buff;
    buff << _nodeNum;
    outFile << "NodeNum = " << _nodeNum << '\n';
    buff << _edgeNum;
    outFile << "EdgeNum = " << _edgeNum << '\n';

    for (size_t i = 0, end_i = _edgeArray.size(); i < end_i; ++i) {
        outFile << "\n";
        outFile << " Reaction " << _edgeArray[i]->getId() << ": ";
        vector<int> nodeList = _edgeArray[i]->getRea();
        for (size_t j = 0, end_j = nodeList.size(); j < end_j; ++j) {
            outFile << _nodeArray[nodeList[j]]->getName() << " ";
        }
        
        outFile << _nodeArray[_edgeArray[i]->getEnz()]->getName() << " ";
        
        nodeList = _edgeArray[i]->getPro();
        for (size_t j = 0, end_j = nodeList.size(); j < end_j; ++j) {
            outFile << _nodeArray[nodeList[j]]->getName() << " ";
        }
        outFile << endl;
    }
    outFile << ";\n";
    return;
}

void Searcher::clear()
{
    for (size_t i = 0, end = _nodeArray.size(); i < end; ++i) {
        delete _nodeArray[i];
    }
    for (size_t i = 0, end = _edgeArray.size(); i < end; ++i) {
        delete _edgeArray[i];
    }
    return;
}
