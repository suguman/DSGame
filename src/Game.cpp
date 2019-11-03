//============================================================================
// Name        : game.cpp
// Author      : Suguman Bansal
// Version     :
// Copyright   : Your copyright notice
// Description : Class for Graph
//============================================================================

#include "common.h"
#include "Game.h"
//#include "Graph.h"
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

int  nextcomparatorstate(int cur_state, int wt, int distance, int df, int value){
  //Calculate determine the next state
  // if distance == 0, return with -value
  // if else, return without value
  return 0;
}

Game::Game(){
  this->initial = "NA";
  this->winning = {};
  this->stateToPlayer = {};
  this->transFunc = {};
  this->reverseFunc = {};
}



Game::Game(Graph* gg, int df, int value){

  //Setting initial state
  string init = stringify(gg->getInitial(), 0, 0);
  this->initial = init;

  //Making remaining states

  unordered_map<int, vector<Transition*>>* transF = gg->getTrans();
  
  queue<string> statestack;
  unordered_map<string, bool> isstate;
  
  statestack.push(init);
  isstate[init] = true;
  
  while(!statestack.empty()){
    string state = statestack.front();
    cout << state << endl;
    statestack.pop();
  
    vector<string> statetemp = split(state);
    int cur_state = stoi(statetemp[0]);
    int cur_comparator  = stoi(statetemp[1]);
    int distance = stoi(statetemp[2]);
    cout << cur_state << " " << cur_comparator << " " << distance << endl;

    vector<Transition*> translist = transF->at(cur_state);
    //vector<Transition*>* transList = gg->getTrans()[cur_state];
    for(auto &trans : translist){
      //Make new state
      
      trans->toString();
      int next_src = trans->getDest();
      
      int wt = trans->getWt();
      int next_comparator = nextcomparatorstate(cur_comparator, wt, distance, df, value);
      //Make new state

      //TODO:: adjust next_comparator
      string new_state;
      if (distance == 0){
	 new_state = stringify(next_src, next_comparator, distance + 1);
	}
      else{
        new_state = stringify(next_src, next_comparator, distance);
      }

      /*
      try(string str = isstate.at(new_state)){
	}
      catch(const std::out_of_range){
	cout << here << endl;
      }
      */
      
    }
        
  }
  

  /*
  int stategraph = get<0>(state);
  int statecomparator = get<1>(state);
  int distance = get<2>(state);
  */
  
  
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

