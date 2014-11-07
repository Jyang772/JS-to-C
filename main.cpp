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

    //Computer starts calculating moves.
    test.startCalculation(game);
    test.calculateAhead(game);

    //Computer will play move it found.
    test.play(game);


    cout << "END!" << endl;
    return 0;
}



