//============================================================================
// Name        : game.cpp
// Author      : Suguman Bansal
// Version     :
// Copyright   : Your copyright notice
// Description : Class for Graph
//============================================================================

#include "common.h"
#include "Game.h"
#include "Graph.h"
#include "Transition.h"

using namespace std;


string stringify(int a, int b, int c){
  string sep = "_";
  //cout << to_string(a) + sep + to_string(b) + sep + to_string(c) << endl;
  return to_string(a) + sep + to_string(b) + sep + to_string(c);
}

vector<string> split (const string &s) {
  char delim = '_';
  vector<string> result;
  stringstream ss (s);
  string item;

  while (getline (ss, item, delim)) {
    result.push_back (item);
  } 
  return result;
}

string makenewstate(int playerID, bool isupper, bool islower, int graphstateID, int compstateID, int distance){

  if (isupper and (playerID==0)){
    return "Upperboundstateplayer0";
  }
  if (isupper and (playerID ==1)){
    return "Upperboundstateplayer1";
  }
  if (islower and (playerID == 0)){
    return "Lowerboundstateplayer0";
  }
  if (islower and (playerID == 1)){
    return "Lowerboundstateplayer1";
  }
  if (!islower and !isupper){
    return stringify(graphstateID, compstateID, distance);
  }
  //THROW EXCEPTION
}

int makelowerbound(int distance, int maxwt, int threshold, int df){
  float lowerboundtemp=0;
  if (distance == 0){
      lowerboundtemp = (float(threshold)/df) - float(maxwt)/(df-1);
    }
  if (distance == 1){
      lowerboundtemp =  -1*float(maxwt)/(df-1);
    }

  cout << lowerboundtemp << " " << floor(lowerboundtemp) << endl;
  
  return floor(lowerboundtemp);
}

int makeupperbound(int distance, int maxwt, int threshold, int df){
  float upperboundtemp=0;
  if (distance == 0){
      upperboundtemp = (float(threshold)/df) + float(maxwt)/(df-1);
    }
  if (distance == 1){
      upperboundtemp = float(maxwt)/(df-1);
    }

  cout << upperboundtemp << " " << floor(upperboundtemp) << endl;
  
  return floor(upperboundtemp);
}


Game::Game(){
  this->initial = "NA";
  this->winning = {};
  this->stateToPlayer = {};
  this->transFunc = {};
  this->reverseFunc = {};
}

Game::Game(Graph* gg, int df, int value){


  unordered_map<int, vector<Transition*>>* transF = gg->getTrans();
  unordered_map<int, int>* statePlayerID = gg->getStateToPlayer();

  unordered_map<string, int> statetoplayeraux = {};
  //unoredered_map<string, vector<string>>* transfuncaux = {};
  //unoredered_map<string, vector<string>>* reversefuncaux = {};
  
  //*****Begin: Get the bounds for comparator autoamta*****//
  //Comparator state bounds (Currently we assume that the value is v.0)

  int maxWt = gg->getWt();
  
  //distance = 0
  int lowerbound0 = makelowerbound(0, maxWt, value, df);
  int upperbound0 = makeupperbound(0, maxWt, value, df);
  //distance = 1
  int lowerbound1 = makelowerbound(1, maxWt, value, df);
  int upperbound1 = makeupperbound(1, maxWt, value, df);

  //*****End: Get the bounds for comparator autoamta*****//

  //Setting initial state of game
  string init = stringify(gg->getInitial(), 0, 0);
  this->initial = init;

  //Making remaining states
  //Maintaining statestack to maintain all states that have to be explored
  //Mainitaining isstate to maintain all states that have already been explored

  queue<string> statestack;
  unordered_map<string, bool> isstate;

  //Special state : init
  statestack.push(init);
  isstate[init] = true;
  statetoplayeraux[init] = statePlayerID->at(gg->getInitial());
    
  //Special state : Upperboundstateplayer0 (State exists since all upperboundstates for player 0 can be condensed to one)
  //Special state : Upperboundstateplayer1 (State exists since all upperboundstates for player 1 can be condensed to one)
  //Special state : Lowerboundstateplayer0 (State exists since all lowerboundstate for player 0 can be condensed to one)
  //Special state : Lowerboundstateplayer1 (State exists since all lowerboundstate for player 1 can be condensed to one)
  //These states never go into the statestack
  
  string uboundplayer0 = "Upperboundstateplayer0";
  string uboundplayer1 = "Upperboundstateplayer1";
  string lboundplayer0 = "Lowerboundstateplayer0";
  string lboundplayer1 = "Lowerboundstateplayer1";

  isstate[uboundplayer0] = true;
  isstate[uboundplayer1] = true;
  isstate[lboundplayer0] = true;
  isstate[lboundplayer1] = true;

  statetoplayeraux[uboundplayer0] = 0;
  statetoplayeraux[uboundplayer1] = 1;
  statetoplayeraux[lboundplayer0] = 0;
  statetoplayeraux[lboundplayer1] = 1;
  
  
  while(!statestack.empty()){
    string state = statestack.front();
    statestack.pop();
    cout << "State being explored is " << state << endl;
    cout << "State belongs to player " << statetoplayeraux[state] << endl;
    
    vector<string> statetemp = split(state);
    int cur_state = stoi(statetemp[0]);
    int cur_comparator  = stoi(statetemp[1]);
    int distance = stoi(statetemp[2]);
    cout << cur_state << " " << cur_comparator << " " << distance << endl;

    vector<Transition*> translist = transF->at(cur_state);
    
    for(auto &trans : translist){

      trans->toString();

      //*****Begin: Make new state*****//
      // (a). Find the next state in the graph
      // (b). Find the next state in the comparator
      // (c). Find the next state in the distance. (Currently we assume that the value is v.0)      
     
      // (a). Find the next state in the graph

      int next_src = trans->getDest();

      // (b). Calculate the next state in the comparator

      int wt = trans->getWt();
      
      int next_comparator = 0;
      int next_comparator_temp = 0;

      //These flags will be true only if the comparator values are beyond
      bool upperboundstateflag = false;
      bool lowerboundstateflag = false;
      if (distance == 0){
	next_comparator_temp = df*cur_comparator + wt - value;
	if (next_comparator_temp > upperbound0){
	  upperboundstateflag = true;
	}
	else if (next_comparator_temp <= lowerbound0){
	  lowerboundstateflag = true;
	}
	else{
	  next_comparator  = next_comparator_temp;
	}
      }
      
      else{//distance == 1
	next_comparator_temp = df*cur_comparator + wt;
	if (next_comparator_temp > upperbound1){
	  upperboundstateflag = true;
	}
	else if (next_comparator_temp <= lowerbound1){
	  lowerboundstateflag = true;
	}
	else{
	  next_comparator  = next_comparator_temp;
	}
      }
      
      // (c). Find the next state in the distance. (Currently we assume that the value is v.0)
      
      int next_distance = 1;
      
      // Make the final state using (a), (b) and (c)
      
      string new_state = makenewstate(statePlayerID->at(next_src) , upperboundstateflag, lowerboundstateflag, next_src, next_comparator, next_distance);

      cout << "The new state is " << new_state << endl;
      
      //*****End: Make new state*****//
      
      //Check if new_state is indeed a new state
      //It is a new state if new_state is not already a key in isstate
      //If new_state is a new state, then add it to statestack
      //If new_state is a winning state, add it to winning_state_stack
      
      try{
        bool boolval = isstate.at(new_state);
	}
      catch(const std::out_of_range){
	isstate[new_state] = true;
	statetoplayeraux[new_state] = statePlayerID->at(next_src);
	  
	cout << "Length of statestack before insertion is " << statestack.size() << endl;
	statestack.push(new_state);
	cout << "Length of statestack after insertion is " << statestack.size() << endl;
      }      
      
    }
        
  }
  
  this->winning = {};
  this->stateToPlayer = {};
  this->transFunc = {};
  this->reverseFunc = {};
}

Game::~Game(){
  //TODO
}

string Game::getInitial(){
  return this->initial;
}

unordered_map<string, int>* Game::getWinning(){
  return &(this->winning);
}

unordered_map<string, int>* Game::getStateToPlayer(){
  return &(this->stateToPlayer);
}

unordered_map<string, vector<string>>* Game::getTrans(){
  return &(this->transFunc);
}

void Game::printInitial(){
  cout << "Initial state is " << this->getInitial() << endl;
}

void Game::printWinning(){
  //TODO
}

void Game::printStoPlayer(){
  //TODO
}

void Game::printTrans(){
  //TODO
}

void Game::printAll(){
  printInitial();
  printWinning();
  printStoPlayer();
  printTrans();
}

