#include "common.h"
#include "Game.h"
#include "Graph.h"
#include "Transition.h"
#include "utils.h"


//Command line input is <filename> <df>
int main(int argc, char** argv){

  string filename = argv[1];
  int df = stoi(argv[2]);

  Graph* gamegraph = readGraph(filename);
  gamegraph->printAll();

  Game* game = new Game();

  Game* game2 =  new Game(gamegraph, df, {});

  game2->printAll();
  
  return 0;

}

