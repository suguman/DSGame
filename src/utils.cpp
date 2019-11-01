//============================================================================
// Name        : utils.cpp
// Author      : Suguman Bansal
// Version     :
// Copyright   : Your copyright notice
// Description : Utility functions
//============================================================================

#include "common.h"
#include "Graph.h"
#include "utils.h"

using namespace std;
using namespace boost;
using namespace boost::algorithm;

Graph* readGraph(string filename){
  //cout << filename << endl;

  int numState = 0;
  int initState = 0;
  int maxWt = 0;
  unordered_map<int, int> stateToP = {};
  unordered_map<int, vector<Transition*>> transFunc = {};
  

  ifstream inFile;
  inFile.open(filename);
  if (!inFile){
    cerr << "Unable to open file" << endl;
    exit(1);
  }

  string output;

  int len;
  int src;
  int dest;
  int wt;
  int partition;
  
  while(!inFile.eof()){
    //output
    getline(inFile, output);
 
    trim(output);
    if (output[0] == '#'){
	//IGNORE COMMENT
      }
    else{
      
      //Split the sequence at the whitespace
      // If split-sequence has
      // (a). single element -- iniital state
      // (b). two elements -- state and player affiliation
      // (c). three elements -- transition

      vector<string> splitparts;
      split(splitparts, output, [](char c){return c == ' ';});
      len = splitparts.size();

      switch(len){
      case 1:
	//cout << splitparts[0] << endl;
	if (!(splitparts[0]=="" or splitparts[0]=="\n" or splitparts[0]=="\t" or splitparts[0]=="\r")){
	    initState = stoi(splitparts[0]);
	}	
	break;
	
      case 2:
	src = stoi(splitparts[0]);
	partition = stoi(splitparts[1]);
	//Set up the state-partition 
	stateToP.insert({src, partition});
	//Set up the transition relation
	transFunc.insert({src, {}});
	numState +=1;
	break;
	
      case 3:
	src = stoi(splitparts[0]);	
	dest = stoi(splitparts[1]);
        wt = stoi(splitparts[2]);
	//cout << src << " " << dest << " " << wt << endl;
        Transition* tempTrans = new Transition(src, dest, wt);
	//tempTrans->toString();
	transFunc[src].push_back(tempTrans);
	if (abs(wt)> maxWt){
	  maxWt = wt;
	  }
	break;
	
      }
    }
  }

  inFile.close();
  
  Graph* gg = new Graph(numState, initState, maxWt, &stateToP, &transFunc);
  return gg;

}


