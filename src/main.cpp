#include "common.h"
#include "Game.h"
#include "Graph.h"
#include "Transition.h"
#include "utils.h"


//Command line input is <filename> <df> <value>
int main(int argc, char** argv){

  string filename = argv[1];
  int df = stoi(argv[2]);
  int value = stoi(argv[3]);
  int value2 = stoi(argv[4]);

  Graph* gamegraph = readGraph(filename);
  
  Game* game =  new Game(gamegraph, df, value, value2);

  //game->printAll();
  
  bool syswins = game->playgame("leq");

  cout << "Has system won? " << syswins << endl;
  
  return 0;

}

