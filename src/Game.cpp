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

Game::Game(){
  this->initial = "NA";
  this->winning = {};
  this->stateToPlayer = {};
  this->transFunc = {};
  this->reverseFunc = {};
}

/*
Game::Game(Graph* gg, int df, vector<int> value){

  int wt = gg->getWt();
  return new Game();
  }
*/

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

