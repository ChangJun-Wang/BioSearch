#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <cmath>
#include <map>
#include "cell.h"
#include "net.h"
#include "searcher.h"
using namespace std;


void Seacher::parseInput(fstream& inFile)
{
    string str;
    // Set balance factor
    inFile >> str;
    _bFactor = stod(str);

    // Set up whole circuit
    while (inFile >> str) {
        if (str == "NET") {
            string netName, cellName, tmpCellName = "";
            inFile >> netName;
            int netId = _netNum;
            _netArray.push_back(new Net(netName));
            _netName2Id[netName] = netId;
            while (inFile >> cellName) {
                if (cellName == ";") {
                    tmpCellName = "";
                    break;
                }
                else {
                    // a newly seen cell
                    if (_cellName2Id.count(cellName) == 0) {
                        int cellId = _cellNum;
                        _cellArray.push_back(new Cell(cellName, 0, cellId));
                        _cellName2Id[cellName] = cellId;
                        _cellArray[cellId]->addNet(netId);
                        _cellArray[cellId]->incPinNum();
                        _netArray[netId]->addCell(cellId);
                        ++_cellNum;
                        tmpCellName = cellName;
                    }
                    // an existed cell
                    else {
                        if (cellName != tmpCellName) {
                            assert(_cellName2Id.count(cellName) == 1);
                            int cellId = _cellName2Id[cellName];
                            _cellArray[cellId]->addNet(netId);
                            _cellArray[cellId]->incPinNum();
                            _netArray[netId]->addCell(cellId);
                            tmpCellName = cellName;
                        }
                    }
                }
            }
            ++_netNum;
        }
    }
    return;
}

void Seacher::partition()
{
    double min = (1-_bFactor)*_cellNum/2;
    double max = (1+_bFactor)*_cellNum/2;
    /*double start,duration;
    start = clock();
    duration = clock() - start;*/

    //initialize
    for (int netid = 0; netid < _netNum; netid++){
        vector<int> cellList = _netArray[netid]->getCellList();
        //分出part A 和 part B
        for (int j = 0; j < cellList.size(); j++){
            int cellid = cellList[j];

            if (cellid > (_cellNum/2-1)){
                _cellArray[cellid]->setPart(1);
            }
            else{
                _cellArray[cellid]->setPart(0);
            }
        }
    }
    for (int i = 0; i < _cellNum; ++i) {
    	int pin = (_cellArray[i] -> getNetList()).size();
    	if(pin > _maxPinNum){
    		_maxPinNum = pin;
    	}
    }

    //count for net part size
	for (int netid = 0; netid < _netNum; netid++){
        vector<int> cellList = _netArray[netid]->getCellList();

        for (int j = 0; j < cellList.size(); j++){
        	if (_cellArray[cellList[j]] -> getPart() == 0){
        		_netArray[netid]->incPartCount(0);
        	}
        	if (_cellArray[cellList[j]] -> getPart() == 1){
        		_netArray[netid]->incPartCount(1);
        	}
        }
    }

    //count for cell cut size and part size
	for (int netid = 0; netid < _netNum; netid++){
		vector<int> cellList = _netArray[netid]->getCellList();
        int A = _netArray[netid]->getPartCount(0);
        int B = _netArray[netid]->getPartCount(1);
        if (A*B != 0){
            _cutSize++;
        }
        for (int j = 0; j < cellList.size(); j++){
            if (A*B == 0){
                _cellArray[cellList[j]]->decGain();
            }
            else{
                if (A == 1){
                    if (_cellArray[cellList[j]]->getPart() == 0){
                        _cellArray[cellList[j]]->incGain();
                    }
                }
                if (B == 1){
                    if (_cellArray[cellList[j]]->getPart() == 1){
                        _cellArray[cellList[j]]->incGain();
                    }
                }
            }
        }
	}
	for (int cellid = 0; cellid < _cellNum; cellid++){
        //安排cell到gain值相同的bucket上
        int gain 	  = _cellArray[cellid] -> getGain();
        bool p   	  = _cellArray[cellid] -> getPart();
        Node* newnode = _cellArray[cellid] -> getNode();

        if (_bList[p][gain] == NULL){
        	_bList[p][gain] = newnode;
        	//cout << "bList " << gain << (_bList[p][gain] == NULL) << endl;
        	//cout << "newnode " << gain << (newnode == NULL) << endl;
        }else if (_bList[p][gain] -> getNext() == NULL){
        	_bList[p][gain] -> setNext(newnode);
        	newnode 		-> setPrev(_bList[p][gain]);
        }else{
        	Node* bnext   = _bList[p][gain] -> getNext();
            bnext   		  -> setPrev(newnode);
            newnode 		  -> setNext(bnext);
            newnode 		  -> setPrev(_bList[p][gain]);
            _bList[p][gain]   -> setNext(newnode);
        }
        _partSize[p]++;
    }

//FM
/*
	int gainmax 	= 1;
	int gainsum     = 0;
	int maxgainsum 	= -_maxPinNum;
	while (gainmax > 0) {
	    if (_partSize[0]-1 < min){
	        //此時一定要從part B裡面找maxcell
	       
	        bool part = 1;
	        bool get  = 0;
	        for (int i = _maxPinNum; i > -_maxPinNum-1; --i){
		    	if (_bList[part][i] != NULL) {
		    		int blist	= _bList[part][i]->getId();
		    		int trace = 1;
		        	while(trace == 1){
		        		if (!(_cellArray[blist] -> getLock()) and get == 0){
		        			maxCellid = blist;
		        			get 	= 1;
		        		}
		        		if (_cellArray[blist] -> getNode() -> getNext() == NULL){
		        			trace 	= 0;
		        		}else{
		        			blist   = _cellArray[blist] -> getNode() -> getNext() -> getId();
		        		}
		        	}
		    	}
		    }
	    }
	    else if (_partSize[0]+1 > max){
	        //此時一定要從part A裡面找maxcell
	       
	        bool part = 0;
	        bool get  = 0;
	        for (int i = _maxPinNum; i > -_maxPinNum-1; --i){
		    	if (_bList[part][i] != NULL){
		    		int blist	= _bList[part][i] -> getId();
		    		int trace = 1;
		        	while(trace == 1){
		        		if (!(_cellArray[blist] -> getLock()) and get == 0){
		        			maxCellid = blist;
		        			get 	= 1;
		        		}
		        		if (_cellArray[blist] -> getNode() -> getNext() == NULL){
		        			trace 	= 0;
		        		}else{
		        			blist   = _cellArray[blist] -> getNode() -> getNext() -> getId();
		        		}
		        	}
		    	}
		    }
	    }
	    else {
	    	//cout << "partition is still balanced" << endl;
	        int part = 0;
	        bool get = 0;
	        for (int i = _maxPinNum; i > -_maxPinNum-1; --i){
	        	part = 0;
	        	while(part < 2){
			    	if (_bList[part][i] != NULL){
			    		int blist	= _bList[part][i] -> getId();
			    		int trace = 1;
			        	while(trace == 1){
			        		if (!(_cellArray[blist] -> getLock()) and get == 0){
		        				maxCellid = blist;
		        				get 	= 1;
		        			}
			        		if (_cellArray[blist] -> getNode() -> getNext() == NULL){
			        			trace 	= 0;
			        		}else{
			        			blist   = _cellArray[blist] -> getNode() -> getNext() -> getId();
			        		}
			        	}
			    	}
			    	part ++;
		    	}
		    }
	    }// end of finding max gain cell

	    //update gain
	    updategain();

	    if (_moveNum == _cellNum){
	        _iterNum ++;
	        cout << "iteration num is : " << _iterNum <<endl;
	        
	        int step = _moveNum - _bestMoveNum;
	        cout << "step num is : " << step <<endl;
	        cout << "gainmax  is : " << gainmax <<endl;
	        cout << "gainsum  is : " << gainsum <<endl;
	        while (step != 0){
	            maxCellid   = _moveStack.back();
	            _moveStack.pop_back();
	            _cellArray[maxCellid] -> move();
	            step --;
	        }
	        for (int cellid = 0; cellid < _cellNum; cellid++){
	        	Node* newnode 	= _cellArray[cellid] -> getNode();
	            _cellArray[cellid] -> unlock();
	            newnode  -> setPrev(NULL);
    			newnode  -> setNext(NULL);
    			_cellArray[cellid] -> setGain(0);
	        }
	        for (int i = _maxPinNum; i > -_maxPinNum-1; --i){
	        	_bList[0][i] = NULL;
	        	_bList[1][i] = NULL;
	        }
	        for (int netid = 0; netid < _netNum; netid++){
		        _netArray[netid]->setPartCount(0,0);
		        _netArray[netid]->setPartCount(1,0);
	    	}
	        _moveStack.clear();
	        gainmax 	= maxgainsum;
	        maxgainsum 	= -_maxPinNum;
	        _moveNum = 0;
	        gainsum  = 0;
	        _partSize[1] = 0;
	        _partSize[0] = 0;
	        _cutSize	 = 0;
	        //計算gain值
			for (int netid = 0; netid < _netNum; netid++){
		        vector<int> cellList = _netArray[netid]->getCellList();
		        int Ainc = 0;
		        int Binc = 0;
		        for (int j = 0; j < cellList.size(); j++){
		        	if (_cellArray[cellList[j]] -> getPart() == 0)
		        	{
		        		Ainc = 1;
		        	}
		        	if (_cellArray[cellList[j]] -> getPart() == 1)
		        	{
		        		Binc = 1;
		        	}
		        }
		        if (Ainc)
		        {
		        	_netArray[netid]->incPartCount(0);
		        }
		        if (Binc)
		        {
		        	_netArray[netid]->incPartCount(1);
		        }
		    }
			for (int netid = 0; netid < _netNum; netid++){
				vector<int> cellList = _netArray[netid]->getCellList();
		        int A = _netArray[netid]->getPartCount(0);
		        int B = _netArray[netid]->getPartCount(1);
		        if (A*B != 0){
		            _cutSize++;
		        }
		        for (int j = 0; j < cellList.size(); j++){
		            if (A*B == 0){
		                _cellArray[cellList[j]]->decGain();
		            }
		            else{
		                if (A == 1){
		                    if (_cellArray[cellList[j]]->getPart() == 0){
		                        _cellArray[cellList[j]]->incGain();
		                    }
		                }
		                if (B == 1){
		                    if (_cellArray[cellList[j]]->getPart() == 1){
		                        _cellArray[cellList[j]]->incGain();
		                    }
		                }
		            }
		        }
	    	}
	    	for (int cellid = 0; cellid < _cellNum; cellid++){
		        //安排cell到gain值相同的bucket上
		        int gain 	  = _cellArray[cellid] -> getGain();
		        bool p   	  = _cellArray[cellid] -> getPart();
		        Node* newnode = _cellArray[cellid] -> getNode();

		        if (_bList[p][gain] == NULL){
		        	_bList[p][gain] = newnode;
		        	//cout << "bList " << gain << (_bList[p][gain] == NULL) << endl;
		        	//cout << "newnode " << gain << (newnode == NULL) << endl;
		        }else if (_bList[p][gain] -> getNext() == NULL){
		        	_bList[p][gain] -> setNext(newnode);
		        	newnode 		-> setPrev(_bList[p][gain]);
		        }else{
		        	Node* bnext   = _bList[p][gain] -> getNext();
		            bnext   		  -> setPrev(newnode);
		            newnode 		  -> setNext(bnext);
		            newnode 		  -> setPrev(_bList[p][gain]);
		            _bList[p][gain]   -> setNext(newnode);
		        }
		        _partSize[p]++;
		    }
	    }else{
	    	_moveNum ++;
	        gainsum += _cellArray[maxCellid] -> getGain();
	        _moveStack.push_back(maxCellid);
	        if (gainsum > maxgainsum){
	            maxgainsum   = gainsum;
	            _bestMoveNum = _moveNum;
	        }
	    }
	}*/
}
//update gain
void Seacher::updategain()
{
    bool F               = _cellArray[maxCellid] -> getPart();
    bool T               = !F;
    vector<int> netList  = _cellArray[maxCellid] -> getNetList();
    _cellArray[maxCellid] -> move();
    _cellArray[maxCellid] -> lock();
    _partSize[F] --;
    _partSize[T] ++;

    //先將maxcell移到blist另一側
    Node* newnode 	= _cellArray[maxCellid] -> getNode();
    if ((newnode -> getPrev()) == NULL){
    	_bList[F][_cellArray[maxCellid] -> getGain()] = NULL;
    }else if ((newnode -> getNext()) == NULL) {
    	(newnode -> getPrev()) -> setNext(NULL);
    }else {
    	(newnode -> getNext()) -> setPrev(newnode -> getPrev());
    	(newnode -> getPrev()) -> setNext(newnode -> getNext());
    }
    newnode  -> setPrev(NULL);
    newnode  -> setNext(NULL);

    for (int i = 0; i < netList.size(); ++i){
        int Tc = _netArray[i] -> getPartCount(T);
        int Fc = _netArray[i] -> getPartCount(F);
        vector<int> cellList = _netArray[i] -> getCellList();

        for (int j = 0; j < cellList.size(); ++j){
        	int cellid      = cellList[j];
        	int gain 		= _cellArray[cellid]->getGain();
	        bool p   		= _cellArray[cellid]->getPart();
	        Node* newnode 	= _cellArray[cellid] -> getNode();
	        if (_cellArray[cellid] -> getLock() == 0)
	        {
		        if (Tc == 0){
		            if ((newnode -> getPrev()) == NULL){
				    	_bList[p][gain] = NULL;
				    }else if ((newnode -> getNext()) == NULL) {
				    	(newnode -> getPrev()) -> setNext(NULL);
				    }else {
				    	(newnode -> getNext()) -> setPrev(newnode -> getPrev());
				    	(newnode -> getPrev()) -> setNext(newnode -> getNext());
				    }
				    newnode  -> setPrev(NULL);
	    			newnode  -> setNext(NULL);
	                _cellArray[cellid] -> incGain();

		            //安排cell到gain值相同的bucket上
			        if (_bList[p][gain] == NULL){
			        	_bList[p][gain] = newnode;
			        }else if (_bList[p][gain] -> getNext() == NULL){
			        	_bList[p][gain] -> setNext(newnode);
			        	newnode 		-> setPrev(_bList[p][gain]);
			        }else{
			        	Node* bnext   = _bList[p][gain] -> getNext();
			            bnext   		  -> setPrev(newnode);
			            newnode 		  -> setNext(bnext);
			            newnode 		  -> setPrev(_bList[p][gain]);
			            _bList[p][gain]   -> setNext(newnode);
			        }
		        }else if (Tc == 1){
	                if (_cellArray[cellid] -> getPart() == T){
	                	if ((newnode -> getPrev()) == NULL){
					    	_bList[p][gain] = NULL;
					    }else if ((newnode -> getNext()) == NULL) {
					    	(newnode -> getPrev()) -> setNext(NULL);
					    }else {
					    	(newnode -> getNext()) -> setPrev(newnode -> getPrev());
					    	(newnode -> getPrev()) -> setNext(newnode -> getNext());
					    }
					    newnode  -> setPrev(NULL);
	    				newnode  -> setNext(NULL);
	                    _cellArray[cellid] -> decGain();

	                    //安排cell到gain值相同的bucket上
				        if (_bList[p][gain] == NULL){
				        	_bList[p][gain] = newnode;
				        }else if (_bList[p][gain] -> getNext() == NULL){
				        	_bList[p][gain] -> setNext(newnode);
				        	newnode 		-> setPrev(_bList[p][gain]);
				        }else{
				        	Node* bnext   = _bList[p][gain] -> getNext();
				            bnext   		  -> setPrev(newnode);
				            newnode 		  -> setNext(bnext);
				            newnode 		  -> setPrev(_bList[p][gain]);
				            _bList[p][gain]   -> setNext(newnode);
				        }
	                }
		        }
	    	}
    	}

        Tc ++;
        Fc --;
        _netArray[i] -> decPartCount(F);
        _netArray[i] -> incPartCount(T);

	    for (int j = 0; j < cellList.size(); ++j){
	    	int cellid      = cellList[j];
	    	int gain 		= _cellArray[cellid]->getGain();
	        bool p   		= _cellArray[cellid]->getPart();
	        Node* newnode 	= _cellArray[cellid] -> getNode();
	        Node* prenode 	= newnode -> getPrev();
	        
	        if (_cellArray[cellid] -> getLock() == 0)
	        {
		        if (Fc == 0){
	                if ((newnode -> getPrev()) == NULL){
				    	_bList[p][gain] = NULL;
				    }else if ((newnode -> getNext()) == NULL) {
				    	(newnode -> getPrev()) -> setNext(NULL);
				    }else {
				    	(newnode -> getNext()) -> setPrev(newnode -> getPrev());
				    	(newnode -> getPrev()) -> setNext(newnode -> getNext());
				    }
				    newnode  -> setPrev(NULL);
	    			newnode  -> setNext(NULL);
		            _cellArray[cellid] -> decGain();
	                
	                //安排cell到gain值相同的bucket上
			        if (_bList[p][gain] == NULL){
			        	_bList[p][gain] = newnode;
			        }else if (_bList[p][gain] -> getNext() == NULL){
			        	_bList[p][gain] -> setNext(newnode);
			        	newnode 		-> setPrev(_bList[p][gain]);
			        }else{
			        	Node* bnext   = _bList[p][gain] -> getNext();
			            bnext   		  -> setPrev(newnode);
			            newnode 		  -> setNext(bnext);
			            newnode 		  -> setPrev(_bList[p][gain]);
			            _bList[p][gain]   -> setNext(newnode);
			        }
		        }else if (Fc == 1){
	                if (_cellArray[cellid] -> getPart() == F){
	                	if ((newnode -> getPrev()) == NULL){
					    	_bList[p][gain] = NULL;
					    }else if ((newnode -> getNext()) == NULL) {
					    	(newnode -> getPrev()) -> setNext(NULL);
					    }else {
					    	(newnode -> getNext()) -> setPrev(newnode -> getPrev());
					    	(newnode -> getPrev()) -> setNext(newnode -> getNext());
					    }
					    newnode  -> setPrev(NULL);
	    				newnode  -> setNext(NULL);
	                    _cellArray[cellid] -> incGain();
	                    
	                    //安排cell到gain值相同的bucket上
				        if (_bList[p][gain] == NULL){
				        	_bList[p][gain] = newnode;
				        }else if (_bList[p][gain] -> getNext() == NULL){
				        	_bList[p][gain] -> setNext(newnode);
				        	newnode 		-> setPrev(_bList[p][gain]);
				        }else{
				        	Node* bnext   = _bList[p][gain] -> getNext();
				            bnext   		  -> setPrev(newnode);
				            newnode 		  -> setNext(bnext);
				            newnode 		  -> setPrev(_bList[p][gain]);
				            _bList[p][gain]   -> setNext(newnode);
				        }
	                }
		        }
	    	}
    	}
    }// end of update gain
}




void Seacher::printSummary() const
{
    cout << endl;
    cout << "==================== Summary ====================" << endl;
    cout << " Cutsize: " << _cutSize << endl;
    cout << " Total cell number: " << _cellNum << endl;
    cout << " Total net number:  " << _netNum << endl;
    cout << " Cell Number of partition A: " << _partSize[0] << endl;
    cout << " Cell Number of partition B: " << _partSize[1] << endl;
    cout << "=================================================" << endl;
    cout << endl;
    return;
}

void Seacher::reportNet() const
{
    cout << "Number of nets: " << _netNum << endl;
    for (size_t i = 0, end_i = _netArray.size(); i < end_i; ++i) {
        cout << setw(8) << _netArray[i]->getName() << ": ";
        vector<int> cellList = _netArray[i]->getCellList();
        for (size_t j = 0, end_j = cellList.size(); j < end_j; ++j) {
            cout << setw(8) << _cellArray[cellList[j]]->getName() << " ";
        }
        cout << endl;
    }
    return;
}

void Seacher::reportCell() const
{
    cout << "Number of cells: " << _cellNum << endl;
    for (size_t i = 0, end_i = _cellArray.size(); i < end_i; ++i) {
        cout << setw(8) << _cellArray[i]->getName() << ": ";
        vector<int> netList = _cellArray[i]->getNetList();
        for (size_t j = 0, end_j = netList.size(); j < end_j; ++j) {
            cout << setw(8) << _netArray[netList[j]]->getName() << " ";
        }
        cout << endl;
    }
    return;
}

void Seacher::writeResult(fstream& outFile)
{
    stringstream buff;
    buff << _cutSize;
    outFile << "Cutsize = " << buff.str() << '\n';
    buff.str("");
    buff << _partSize[0];
    outFile << "G1 " << buff.str() << '\n';
    for (size_t i = 0, end = _cellArray.size(); i < end; ++i) {
        if (_cellArray[i]->getPart() == 0) {
            outFile << _cellArray[i]->getName() << " ";
        }
    }
    outFile << ";\n";
    buff.str("");
    buff << _partSize[1];
    outFile << "G2 " << buff.str() << '\n';
    for (size_t i = 0, end = _cellArray.size(); i < end; ++i) {
        if (_cellArray[i]->getPart() == 1) {
            outFile << _cellArray[i]->getName() << " ";
        }
    }
    outFile << ";\n";
    return;
}

void Seacher::clear()
{
    for (size_t i = 0, end = _cellArray.size(); i < end; ++i) {
        delete _cellArray[i];
    }
    for (size_t i = 0, end = _netArray.size(); i < end; ++i) {
        delete _netArray[i];
    }
    return;
}
