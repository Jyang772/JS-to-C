#include <iostream>
#include <vector>

#include <time.h>
#include <ctime>
#include <cstdlib>

#include "cell.h"
#include "clone.h"
#include "move.h"
#include "game.h"
#include "montebot.h"
#include "tictactoeboard.h"

using namespace std;

bool monteWeightByGameLength = true;

int main()
{
    //Test case.
    std::srand(time(NULL));
    MonteBot test;
    Game game;

    std::cout << "Starting game with boards:" << std::endl;
    for(int i=0; i<3; i++){
      for(int j=0; j<3; j++){
        std::cout << " " << game.boards[i][j];
      }
      std::cout << std::endl;
    }

    while(!game.finished){
      test.startCalculation(game);
      test.calculateAhead(game);
      test.play(game);
      std::cout << game.getBoardDraw();

    }

    cout << "END!" << endl;
    return 0;
}
