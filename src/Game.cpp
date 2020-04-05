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

int makelowerbound(int distance, int maxwt, int threshold, int threshold1, int df){
  float lowerboundtemp=0;
  if (distance == 0){
    lowerboundtemp = (float(threshold)/df) + (float(threshold1)/df*(df-1)) - float(maxwt)/(df-1);
    }
  if (distance == 1){
      lowerboundtemp =  (float(threshold1)/(df-1)) + -1*float(maxwt)/(df-1);
    }

  //cout << lowerboundtemp << " " << floor(lowerboundtemp) << endl;
  
  return floor(lowerboundtemp);
}

int makeupperbound(int distance, int maxwt, int threshold, int threshold1, int df){
  float upperboundtemp=0;
  if (distance == 0){
      upperboundtemp = (float(threshold)/df) + (float(threshold1)/df*(df-1)) + float(maxwt)/(df-1);
    }
  if (distance == 1){
      upperboundtemp = (float(threshold1)/(df-1)) + float(maxwt)/(df-1);
    }

  //cout << upperboundtemp << " " << floor(upperboundtemp) << endl;
  
  return floor(upperboundtemp);
}


Game::Game(){
  this->initial = "NA";
  this->winning = {};
  this->stateToPlayer = {};
  this->transFunc = {};
  this->reverseFunc = {};
}

Game::Game(Graph* gg, int df, int value, int value2){

  // Initally winning states are set to the upperboundstates
  
  this->initial = "NA";
  this->winning = {};
  this->stateToPlayer = {};
  this->transFunc = {};
  this->reverseFunc = {};

  unordered_map<int, vector<Transition*>>* transF = gg->getTrans();
  unordered_map<int, int>* statePlayerID = gg->getStateToPlayer();

  unordered_map<string, int> statetoplayeraux = {};

  unordered_map<string, vector<string>> transfuncaux = {};
  
  unordered_map<string, vector<string>> reversefuncaux = {};

  unordered_map<string, int> winningaux = {};
  
  //*****Begin: Get the bounds for comparator autoamta*****//
  //Comparator state bounds (Currently we assume that the value is v.0)

  int maxWt = gg->getWt();
  //int maxWttemp2 = max(maxWttemp, abs(value2)]);
  
  //distance = 0
  int lowerbound0 = makelowerbound(0, maxWt, value, value2, df);
  int upperbound0 = makeupperbound(0, maxWt, value, value2, df);
  //distance = 1
  int lowerbound1 = makelowerbound(1, maxWt, value, value2, df);
  int upperbound1 = makeupperbound(1, maxWt, value, value2, df);

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
  winningaux[init] = 0;
  
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

  winningaux[uboundplayer0] = 1;
  winningaux[uboundplayer1] = 1;
  winningaux[lboundplayer0] = 0;
  winningaux[lboundplayer1] = 0;
  
  
  while(!statestack.empty()){
    string state = statestack.front();
    statestack.pop();
    //cout << "State being explored is " << state << endl;
    //cout << "State belongs to player " << statetoplayeraux[state] << endl;

    
    //Used to store all outgoing neighbours
    vector<string> deststatelist;
    //Aux variables to ensure no guarantee that the special 4 states are not entered into desstatelist multiple times
    bool booluboundplayer0 = false;
    bool booluboundplayer1 = false;
    bool boollboundplayer0 = false;
    bool boollboundplayer1 = false;
  
    
    vector<string> statetemp = split(state);
    int cur_state = stoi(statetemp[0]);
    int cur_comparator  = stoi(statetemp[1]);
    int distance = stoi(statetemp[2]);
    //cout << cur_state << " " << cur_comparator << " " << distance << endl;

    vector<Transition*> translist = transF->at(cur_state);
    
    for(auto &trans : translist){

      //trans->toString();

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
	next_comparator_temp = df*cur_comparator + wt - value2;
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
      
      //cout << "The new state is " << new_state << endl;

      //TODO:: Make sure that special states are not entered more than once
      if (upperboundstateflag or lowerboundstateflag){
	//test if state has been entered before
	if (new_state == uboundplayer0){
	  if (booluboundplayer0 == false){
	    deststatelist.push_back(new_state);
	    booluboundplayer0 = true;
	  }
	}
	if (new_state == uboundplayer1){
	  if (booluboundplayer1 == false){
	      deststatelist.push_back(new_state);
	      booluboundplayer1 = true;
	  }
	}
	if (new_state == lboundplayer0){
	  if (boollboundplayer0 == false){
	    deststatelist.push_back(new_state);
	    boollboundplayer0 = true;
	  }
	}
	if (new_state == lboundplayer1){
	  if (boollboundplayer1 == false){
	    deststatelist.push_back(new_state);
	    boollboundplayer1 = true;
	  }
	}
      }
      else{
	//new_state is a tuples-state
	deststatelist.push_back(new_state);
      }
      
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
	winningaux[new_state] = 0;
	
	//cout << "Length of statestack before insertion is " << statestack.size() << endl;
	statestack.push(new_state);
	//cout << "Length of statestack after insertion is " << statestack.size() << endl;
      }
    }
    
    transfuncaux[state] = deststatelist;

  }
  this->stateToPlayer = statetoplayeraux;
  this->transFunc = transfuncaux;

  //Make reverse transitions
  reversefuncaux;
  unordered_map<string, vector<string>> :: iterator p;
  
  for (p = transfuncaux.begin(); p != transfuncaux.end(); p++){
    string srcstate = p->first;
    vector<string> temptranslist= p->second;
    for (auto & element : temptranslist){
      try{
        reversefuncaux[element].push_back(srcstate);
	}
      catch(const std::out_of_range){
        reversefuncaux[element].push_back(srcstate);      
      }
    }
  }
  this->reverseFunc = reversefuncaux;
  this->winning = winningaux;
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

unordered_map<string, vector<string>>* Game::getRevTrans(){
  return &(this->reverseFunc);
}

void Game::printInitial(){
  //cout << "Initial state is " << this->getInitial() << endl;
}

void Game::printWinning(){
  cout << "Default: Printing the upper states" << endl;
  unordered_map<string, int>:: iterator p;
  unordered_map<string, int>* temp = this->getWinning(); 
  for (p = temp->begin(); p != temp->end(); p++){
    cout << p->first << ", " << p->second << endl;
  }
}

void Game::printStoPlayer(){
 cout << "State-Player mapping is " << endl;

  unordered_map<string, int>:: iterator p;
  unordered_map<string, int>* temp = this->getStateToPlayer(); 
  for (p = temp->begin(); p != temp->end(); p++){
    cout << p->first << ", " << p->second << endl;
  }
}

void Game::printTrans(){
  cout << "Transition relation is " << endl;

  unordered_map<string, vector<string>> :: iterator p;
  unordered_map<string, vector<string>>* temp = this->getTrans();
  
  for (p = temp->begin(); p != temp->end(); p++){
    vector<string> temptranslist= p->second;
    for (auto & element : temptranslist) {
      cout << p->first << "-->" << element << endl;
    }
  }
}


void Game::printRevTrans(){
  cout << "Transition reverse relation is " << endl;

  unordered_map<string, vector<string>> :: iterator p;
  unordered_map<string, vector<string>>* temp = this->getRevTrans();
  
  for (p = temp->begin(); p != temp->end(); p++){
    vector<string> temptranslist= p->second;
    for (auto & element : temptranslist) {
      cout << p->first << "-->" << element << endl;
    }
  }
}


void Game::printAll(){
  printInitial();
  printWinning();
  printStoPlayer();
  printTrans();
}


bool Game::reachabilitygame(string relation, int player){
  
  int opponent = 1 - player;
  
  unordered_map<string, vector<string>>* reverse_map = this->getRevTrans();
  unordered_map<string, vector<string>>* map = this->getTrans();
  unordered_map<string, int>* statetoplayer = this->getStateToPlayer();
  unordered_map<string, int>* winning = this->getWinning();
  
  string initial = this->getInitial();

  //If Upperbound states are not reachable in the game, i.e. not present in reverse_trans, then player with reachability target cannot win
  try{
    reverse_map->at("Upperboundstateplayer0");
  }
  catch(const std::out_of_range){
    try{
      reverse_map->at("Upperboundstateplayer1");
    }
    catch(const std::out_of_range){
      //cout << "Player cannot win since the initial winning states are not reachable" << endl;
      return false;
    }
  }
  
  unordered_map<string, int> numtrans;
  unordered_map<string, vector<string>> :: iterator p;
  for(p = map->begin(); p != map->end(); p++){
    numtrans[p->first] = (p->second).size();
  }

  //Make work queue
  queue<string> statestack;
  statestack.push("Upperboundstateplayer0");
  statestack.push("Upperboundstateplayer1");
  
  vector<string> revtranslist;
  bool playerwins = false;

  //this->printRevTrans();
  while(!statestack.empty()){
    
    //cout << "Length of statestack before popping element " << statestack.size() << endl;
    string state = statestack.front();
    statestack.pop();
    //cout << "Current state is " << state << endl;
    try{
      revtranslist = reverse_map->at(state);
    }
    catch(const std::out_of_range){
      //cout << "Skipping " << state << endl;
      continue;
    }
    for(auto & element : revtranslist){
      //cout << "Exploration in current state is  " << element << endl;
      int statebelongsto = statetoplayer->at(element);
      //cout << "State belongs to " << statebelongsto << endl;

      //cout << "State is winning? " << numtrans[element] << endl;
      if (statebelongsto == player){
	//there exists state
	//element is a winning state
	if (element == initial){
	    //player has won, since it has arrived at the initial state that it controls
	    return true;
	}
	if (numtrans[element] != 0){
	  //element hasn't been visited yet, add it to the stack. A there exists state is visited if its numtrans = 0
	  // if it has been visited before, numtrans = 0, do nothing
	  //winning[element] = 1;
	  statestack.push(element);
	  numtrans[element] = 0;
	}
      }
      else{
	//statebelongs to opponent
	//for all state
	if (numtrans[element]!=0){
	  //cout << element << " is " << numtrans[element] << endl; 
	  numtrans[element] = numtrans[element]-1;
	  //cout << element << " is " << numtrans[element] << endl;
	  if (numtrans[element] == 0){
	    //element is  a winning state
	    if (element == initial){
	      //game won
	      return true;
	    }
	    //add winning state to stack
	    statestack.push(element);
	  }
	}
	
      }
    }
  }
  return false;
}


bool Game::playgame(string relation){

  //Env is player 0
  //Sys is player 1
  
  if (relation == "leq"){
    //safety game with all but the upper states. Player 1 must win. 
    //Equiv to opposite of:: reachability game with upper states as target. Player 0 must win.
    bool ans = reachabilitygame(relation, 0);
    //cout << !(ans) << endl;
    return !(ans);
  }
  if (relation == "lt"){
    //reachability game with appropriate winning states.
    //TODO:  change winning states
    return true;
  }
}
