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
#include "path.h"
#include "searcher.h"
using namespace std;

bool belong( int item, vector<int> array ) {
    for (const auto& i: array)
    {
        if (item == i) return true;
    }
    return false;
}

void Searcher::search(char* input_species)
{
    if (*input_species == '0'){
        _inNode[0] = _nodeName2Id["lactate"];
        _inNode[1] = _nodeName2Id["ethanol"];
    }
    else if (*input_species == '1')
    {
        _inNode[0] = _nodeName2Id["acetone"];
        _inNode[1] = _nodeName2Id["D-glucose"];
    }
    else if (*input_species == '2')
    {
        _inNode[0] = _nodeName2Id["D-glucose"];
        _inNode[1] = _nodeName2Id["nitric_oxide"];
    }
    else{
        cout <<"no input" << endl;
        return;
    }
    // _outNode = _nodeName2Id["NADH"];
    _outNode = _nodeName2Id["resorufin"];
    for (int i = 0; i < 2; ++i) _nodeArray[_inNode[i]]->addLabel(5);
    _nodeArray[_outNode]->addLabel(6);

    // SearchUp(_outNode, 6);

    vector<int>     candidate;
    BuildStart();
    _nodeArray[_inNode[0]] -> copyToPath(0);
    vector<int> candidate0 = SearchDown(_inNode[0], 0, 1);
    cout << "candidate0 : " << candidate0.size() << endl;
    for (const auto& cand: candidate0) _nodeArray[cand] -> copyToRecord(0);
    ClearPath();

    BuildStart();
    _nodeArray[_inNode[1]] -> copyToPath(0);
    vector<int> candidate1 = SearchDown(_inNode[1], 0, 1);
    cout << "candidate1 : " << candidate0.size() << endl;
    cout << "start merging type A" << endl;
    for (const auto& cand0: candidate0) {
        for (const auto& cand1: candidate1) {
            if (cand0 == cand1) {
                _nodeArray[cand0] -> copyToRecord(0);
                candidate.push_back(cand0);
            }
        }
    }
    ClearPath();
    cout << "candidate : " << candidate.size() << endl;

    for (const auto& cand0: candidate) {
        ClearPathA1();
        ClearPathA2();
        ClearPath();
        cout << "candidate name : " << _nodeArray[cand0] -> getName() << endl;
        _nodeArray[cand0] -> copyToPath(0);
        vector<int> candidateB = SearchDown(cand0, 1, 1);
        cout << "candidateB : " << candidateB.size() << endl;
        for (const auto& node: candidateB) _nodeArray[node] -> copyToRecord(1);

        for (const auto& cand1: candidate) {
            if (cand0 == cand1) continue;
            _nodeArray[cand1] -> copyToPath(0);
            vector<int> candidateBside = SearchDown(cand1, -1, 1);
            cout << "start merging B" << endl;
            vector<int> merge = mergeB(candidateB, candidateBside);

            if (merge.size() != 0) cout << "merge success" << endl;
            for (const auto& target: merge) {
                clearPath();
                outputs = SearchDown(target, 2, 3);
                check = Check(outputs);
                for (const auto& output: outputs)
                {
                    cout << "output node" << endl;
                }

            }

        }

            // for j in range(len(candidate)):
            //     if candidate[i] != candidate[j]:
            //         self.ClearPath()
            //         self.ClearLevel()
            //         candidate_B = []
            //         candidate[j].CopyToPath("A")
            //         candidate_Bside = SearchDown(candidate[j], "B_side", self.up_limit, 1, self.input_species, self.output_species)
            //         print ("start merging B")
            //         for species in candidate_Bside:
            //             targetRec = self.mapToEdge[species[1]]
            //             targetB = targetRec.rec_node
            //             targetB.path = species[0].path
            //             # output = SearchDown(targetB, "output", self.up_limit, 2, self.input_species, self.output_species)
            //             merge = self.MergeBside(targetB.path, targetB.pathA1, targetB, species[0])
            //             if merge != []:
            //                 print("merge success")
            //                 targetB.pathA2 = merge
            //                 for path in targetB.pathA2:
            //                     path["pathlist"].append(targetRec)
            //                     for pro in targetRec.getpro():
            //                         path["related"].add(pro)
            //                     for rea in targetRec.getrea():
            //                         if rea not in path["pathnode"]:
            //                             path["related"].add(rea)
            //                 candidate_B.append((targetB, species[0], targetRec))
            //         print (len(candidate_B))
            //             # species[0].CopyToRecord("A2")

            //         for target in candidate_B:
            //             self.ClearPath()
            //             self.ClearLevel()
            //             target[0].CopyToPath("A2")
            //             target[1].mark = 1
            //             target[2].getenz().mark = 1
            //             # print (target.show())
            //             output = SearchDown(target[0], "output", self.up_limit, 3, self.input_species, self.output_species)
            //             target[1].mark = 0
            //             target[2].getenz().mark = 0
            //             # for can in output:
            //             #     assert (can[0] == self.output_species)
            //             print ("target : ", target[0].show(), target[0].layer)
            //             # tmp = []
            //             for outNode in self.output_species:
            //                 tmp = []
            //                 if len(outNode.path) == 0:
            //                     continue
            //                 for path in outNode.path:
            //                     allnodes = set()
            //                     for node in self.input_species:
            //                         allnodes.add(node)
            //                     for rec in path["pathlist"]:
            //                         for rea in rec.getrea():
            //                             if rea not in path["pathnode"]:
            //                                 allnodes.add(rea)
            //                     for node in path["related"]:
            //                         allnodes.add(node)
            //                     for rec in path["pathlist"]:
            //                         allnodes.add(rec.getenz())
            //                     flag = True
            //                     for rec in outNode.getUpedge():
            //                         if rec.activated(allnodes):
            //                             flag = False
            //                     for node in path["pathnode"]:
            //                         allnodes.add(node)
            //                     allnodes.add(outNode)
            //                     for rec in outNode.getDownedge():
            //                         if rec.activated(allnodes):
            //                             flag = False
            //                     if flag:
            //                         tmp.append(path)
            //                 outNode.path = tmp
            //                 if len(outNode.path) != 0:
            //                     print ("target : ", target[0].show(), target[0].layer)
            //                     for path in outNode.path:
            //                         thres = []
            //                         for upRec in target[1].getUpedge():
            //                             # thres.append(upRec)
            //                             flag = True
            //                             for rea in upRec.getrea():
            //                                 if rea in path["pathnode"]:
            //                                     flag = False
            //                             for pro in upRec.getpro():
            //                                 if pro != target[1] and pro in path["pathnode"]:
            //                                     flag = False
            //                                 if pro == outNode:
            //                                     flag = False
            //                             if flag and upRec.getenz().name != "spontaneous_reaction":
            //                                 thres.append(upRec)
            //                         # ans = 100
            //                         for x in thres:
            //                             allnodes = set()
            //                             for rec in path["pathlist"]:
            //                                 for node in rec.getrea():
            //                                     allnodes.add(node)
            //                                 for node in rec.getpro():
            //                                     allnodes.add(node)
            //                                 allnodes.add(rec.getenz())
            //                             for node in x.getrea():
            //                                 allnodes.add(node)
            //                             for node in x.getpro():
            //                                 allnodes.add(node)
            //                             allnodes.add(x.getenz())
            //                             allrec = self.CollectAll(allnodes, path["pathlist"])
            //                             c = 2
            //                             for rec in path["pathlist"]:
            //                                 c = c+1 
            //                             for rec in allrec:
            //                                 c = c+1
            //                             # return True
            //                             # print ("c : ", c)
            //                             self.count += 1
            //                             if c < ans:
            //                                 ans = c
            //                                 record_node = path["pathnode"].copy()
            //                                 record_ans = path["pathlist"].copy()
            //                                 record_ans.append(x)
            //                                 record_oth = allrec.copy()
    }

    return;
}

void Searcher::parseInput(fstream& inFile)
{
    string str;
    cout << "parsing database......";

    // Set up whole graph
    string enzyme = "";
    while (inFile >> str) {
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
                        _nodeArray.push_back(new Node(nodeName));
                        _nodeName2Id[nodeName] = nodeId;
                        ++_nodeNum;
                    }
                    enzyme = nodeName;
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

void Searcher::initialize(){
    for (int i = 0; i < 5; ++i)
    {
        _label[i] = 0;
    }
    for (size_t i = 0, end_i = _nodeArray.size(); i < end_i; ++i)
    {
        if ((_nodeArray[i]->getUpEdge()).size() > 1)
        {
            _label[0] = _label[0] + 1;
            _nodeArray[i]->addLabel(0);
        }
        if ((_nodeArray[i]->getDownEdge()).size() > 1)
        {
            _label[1] = _label[1] + 1;
            _nodeArray[i]->addLabel(1);
        }
        if ((_nodeArray[i]->getUpEdge()).size() > 0 && (_nodeArray[i]->getDownEdge()).size() > 0)
        {
            _label[3] = _label[3] + 1;
            _nodeArray[i]->addLabel(3);
        }
        if ((_nodeArray[i]->getCatEdge()).size() > 1)
        {
            _label[4] = _label[4] + 1;
            _nodeArray[i]->addLabel(4);
        }
    }
    return;
}

void Searcher::SearchUp(int startNode, int search_limit) {
    vector<int>  bfs;
    bfs.push_back(startNode);
    _nodeArray[startNode] -> setLayer(1);

    // start to traverse
    while (bfs.size() > 0){
        int curNode = bfs.front();
        bfs.erase(bfs.begin());
        int curLayer = _nodeArray[curNode] -> getLayer();
        if ( curLayer <= search_limit)
        {
            for (const auto& upRec: (_nodeArray[curNode] -> getUpEdge()))
            {
                for (const auto& rea: (_edgeArray[upRec] -> getRea())){
                    if (_nodeArray[rea] -> getLayer() == 0)
                    {
                        _nodeArray[rea] -> setLayer(curLayer + 1);
                        bfs.push_back(rea);
                    }
                }
            }
        }
    }
    return;
}

vector<int> Searcher::SearchDown(int startNode, const int Target_type, const int search_limit) {
    vector<int>     bfs;
    bfs.push_back(startNode);
    vector<int>     candidate;

    // start to traverse
    while (bfs.size() > 0){
        // cout << "bfs : " << bfs.size() << endl;
        int curId = bfs.front();
        Node* curNode = _nodeArray[curId];
        int curLayer = curNode -> getLayer();
        if (Target_type >= 0) {
            int curLabel = curNode -> getLabel(Target_type);
            if ( curLabel != 0 ) found(curId, Target_type, &candidate);
        }
        else found(curId, Target_type, &candidate);
        // cout << "curLayer" << curLayer;
        // cout << "search_limit" << search_limit + 1;
        if ( curLayer < search_limit)
        {
            for (const auto& downRec: (curNode -> getDownEdge()))
            {
                for (const auto& nextId: (_edgeArray[downRec] -> getPro())){
                    Node* nextNode = _nodeArray[nextId];
                    if (nextNode -> getLayer() == 0)
                    {
                        AddPath(curId, downRec, nextId);
                        nextNode -> setLayer(curLayer + 1);
                        if (nextNode -> getLayer() <= search_limit) bfs.push_back(nextId);
                    }
                }
            }
        }
        bfs.erase(bfs.begin());
    }
    return candidate;
}

void Searcher::found(int curId, const int Target_type, vector<int>* candidate) {
    (*candidate).push_back(curId);
    return;
}

vector<int> Searcher::Check(vector<int> cand) {
    return {};
}

vector<int> Searcher::mergeB(vector<int> candB, vector<int> candBside) {
    
    return;
}

void Searcher::AddPath(int curId, int downRec, int nextId) {
    Node* curNode = _nodeArray[curId];
    Node* nextNode = _nodeArray[nextId];
    for (const auto& pro: (_edgeArray[downRec] -> getPro())) {
        if (curId == pro) return;
    }
    for (const auto& path: (curNode -> getPath()))
    {
        if (~belong(nextId, path -> getRel()) and ~belong(nextId, path -> getNode()) and nextNode -> getName() != "H2O") {
            Path* tmp = new Path();
            tmp -> setRec(path -> getRec());
            tmp -> setEnz(path -> getEnz());
            tmp -> setNode(path -> getNode());
            tmp -> setRel(path -> getRel());

            tmp -> addRec(downRec);
            tmp -> addEnz(_edgeArray[downRec] -> getEnz());
            tmp -> addNode(nextId);
            for (const auto& rea: (_edgeArray[downRec] -> getRea())) {
                if (rea != curId) tmp -> addRel(rea);
            }

            if ((nextNode -> getPath()).size() >= 10) {
                for (int i = 0; i < (nextNode -> getPath()).size(); ++i) {
                    if (((nextNode -> getPath())[i] -> getRel()).size() >= (tmp -> getRel()).size()) {
                        (nextNode -> getPath())[i] = tmp;
                        break;
                    }
                }
            }
            else nextNode -> addPath(tmp);
        }
    }
    return;
}


void Searcher::BuildStart() {
    Path* startPath0 = new Path();
    startPath0 -> addRel(_nodeName2Id["H2O"]);
    startPath0 -> addEnz(_nodeName2Id["spontaneous_reaction"]);
    startPath0 -> addNode(_inNode[1]);
    _nodeArray[_inNode[0]] -> addPath(startPath0);

    Path* startPath1 = new Path();
    startPath1 -> addRel(_nodeName2Id["H2O"]);
    startPath1 -> addEnz(_nodeName2Id["spontaneous_reaction"]);
    startPath1 -> addNode(_inNode[0]);
    _nodeArray[_inNode[1]] -> addPath(startPath1);

    return;
}

void Searcher::ClearPath() {
    for (const auto& node: _nodeArray) {
        node -> clearPath();
        node -> clearLayer();
    }
    return;
}

void Searcher::ClearPathA() {
    for (const auto& node: _nodeArray) node -> clearRecA();
    return;
}

void Searcher::ClearPathA1() {
    for (const auto& node: _nodeArray) node -> clearRecA1();
    return;
}

void Searcher::ClearPathA2() {
    for (const auto& node: _nodeArray) node -> clearRecA2();
    return;
}

void Searcher::printSummary() const
{
    cout << endl;
    cout << "==================== Summary ====================" << endl;
    cout << " Total node number: " << _nodeNum << endl;
    cout << " Total edge number:  " << _edgeNum << endl;
    cout << "=================================================" << endl;
    
    cout << " typeA : " << _label[0] << endl;
    cout << " typeB : " << _label[1] << endl;
    cout << " typeC : " << _label[2] << endl;
    cout << " typeD : " << _label[3] << endl;
    cout << " typeE : " << _label[4] << endl;
    cout << "=================================================" << endl;
    cout << "input_species : " << endl;
    for (int i = 0; i < 2; ++i)
    {
        cout << _nodeArray[_inNode[i]] -> getName() << " : " << _nodeArray[_inNode[i]] -> getLayer() << endl;
    }

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
