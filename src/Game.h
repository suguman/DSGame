/*
 * game.h
 *
 * Created on: October 31, 2019
 * Author: Suguman Bansal
 */

#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "Transition.h"

using namespace std;

class Game{

 public:
  Game();
  Game(Graph* gg);
  virtual ~Game();

  //Access functions
  int getInitial();
  unordered_map<string, bool>* getWinning;
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
  vector<string, bool> winning;
  unordered_map<string, int> stateToPlayer;
  unordered_map<string, vector<string>> transFunc;
  unordered_map<string, vector<string>> reverseFunc;
  
};


#endif 
