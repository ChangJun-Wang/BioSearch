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

// vector<string> split(inputline)
// {
//     temp    = ""
//     tmpList = []
//     for w in inputline:
//         if (w == " " or w == "\n"):
//             if temp != "":
//                 tmpList.append(temp)
//             temp = ""
//         else:
//             temp += w
//     return tmpList
// }

// 0.2
// NET n1       c4486 c43922 ;
// NET n2       c136070 c136069 c20932 c71230 ;
// NET n3       c150418 c150410 c57992 c74380 ;
// NET n4       c47228 c68994 ;
// NET n5       c35857 c147459 c147458 c122391 c84003 c23446 c23445 ;
// NET n6       c150417 c150412 c109185 c83786 c74379 ;
// NET n7       c136069 c20932 c64645 c71230 c122210 ;
// NET n8       c47229 c68995 ;
// NET n9       c35912 c147613 c147612 c122697 c84207 c23448 c23447 ;
// NET n10      c150416 c150411 c109185 c83786 c74378 ;
// NET n11      c17669 c68996 ;
// NET n12      c96010 c27316 ;
// NET n13      c35837 c147497 c147496 c122424 c84025 c23450 c23449 ;
// NET n14      c96009 c27315 ;
// NET n15      c30257 c7822 c22293 c52319 c16937 c5008 ;
// NET n16      c98155 c2194 ;
// NET n17      c150415 c57992 c70507 c74377 ;
// NET n18      c30258 c7823 c7822 c40180 ;
// NET n19      c150415 c150414 c110040 c20900 c74376 ;

void Searcher::parseInput(fstream& inFile)
{
    string str;
    cout << "parsing database......";

    // Set up whole graph
    while (inFile >> str) {
        string enzyme = "";
        if (str == "RN") {
            string nodeName = "";
            inFile >> nodeName;
            while (inFile >> nodeName) {
                if (nodeName == "\n") break;
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
                if (nodeName == "\n")  break;
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
                        Node* node = _nodeArray[nodeId];
                        _nodeArray[nodeId]->addUpEdge(edgeId);
                        _edgeArray[edgeId]->addPro(nodeId);
                    }
                    else {
                        int nodeId = _nodeName2Id[nodeName];
                        Node* node = _nodeArray[nodeId];
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
    outFile << "NodeNum = " << buff.str() << '\n';

    for (size_t i = 0, end_i = _edgeArray.size(); i < end_i; ++i) {
        outFile << ";\n";
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
