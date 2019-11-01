//============================================================================
// Name        : game.cpp
// Author      : Suguman Bansal
// Version     :
// Copyright   : Your copyright notice
// Description : Class for Graph
//============================================================================

#include "Game.h"

using namespace std;

Game::Game(){
  this->initial = "";
  this->winning = {};
  this->stateToPlayer = {};
  this->transFunc = {};
  this->reverseFunc = {};
}


Game::Game(Graph* gg){
  }

Game::~Game(){
  //TODO
}

int Game::getInitial(){
  return this->initial;
}

unordered_map<string, bool> Game::getWinning{
  return &(this->winning);
}

unordered_map<string, int>* Game::getStateToPlayer(){
  return &(this->stateToPlayer);
}

unordered_map<string, vector<string>>* Game::getTrans(){
  return &(this->transFunc);
}
