/*
 * game.h
 *
 * Created on: October 31, 2019
 * Author: Suguman Bansal
 */

#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "Graph.h"
#include "Transition.h"

using namespace std;

class Game{

 public:
  Game();
  Game(Graph* gg, int df, vector<int> value);
  virtual ~Game();

  //Access functions
  string getInitial();
  unordered_map<string, int>* getWinning();
  unordered_map<string, int>* getStateToPlayer();
  unordered_map<string, vector<string>>* getTrans();

  
  //Print functions
  void printInitial();
  void printWinning();
  void printStoPlayer();
  void printTrans();
  void printAll();
  
  
 private:
  string initial;
  unordered_map<string, int> winning;
  unordered_map<string, int> stateToPlayer;
  unordered_map<string, vector<string>> transFunc;
  unordered_map<string, vector<string>> reverseFunc;
  
};


#endif 
