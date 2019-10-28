/*
 * graph.h
 *
 * Created on: October 26, 2019
 * Author: Suguman Bansal
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"
#include "Transition.h"

using namespace std;

class Graph{

 public:
  Graph();
  Graph(int num, int initial, unordered_map<int, int>* stateToP, unordered_map<int, vector< Transition*>>* transMap);
  virtual ~Graph();

  //Access functions
  int getInitial();
  int getStateNum();
  unordered_map<int, int>* getStateToPlayer();
  unordered_map<int, vector< Transition*>>* getTrans();

  //Functionality  
  int getTransNum();
  
  //Print functions
  void printInitial();
  void printStoPlayer();
  void printTrans();
  void printAll();
  
 private:
  int numState;
  int initState;
  unordered_map<int, int> stateToPlayer;
  unordered_map<int, vector<Transition*>> transFunc;
  
};


#endif 
