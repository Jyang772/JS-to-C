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

    std::srand(time(NULL));
    MonteBot test;
    Game game;
    //game.boards[0][0]->cells[0][1]->winner = 2341;
    //game.winner = 1;
    //Game newgame(game);

    //std::cout << endl << newgame.boards[0][0]->cells[0][1]->winner << endl;


    test.startCalculation(game);
    test.calculateAhead(game);
    test.play(game);
    cout << "Hello World!" << endl;
    return 0;
}



