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

  Graph* gamegraph = readGraph(filename);
  gamegraph->printAll();
  //gamegraph->printTrans();

  Game* game = new Game();

  Game* game2 =  new Game(gamegraph, df, value);

  //game2->printAll();

  bool syswins = game2->playgame("leq");

  cout << "Has system won? " << syswins << endl;
  
  return 0;

}

