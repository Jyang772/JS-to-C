#include "montebot.h"


void MonteBot::play(Game &game){
    int bestScore = -10000;
    Clone bestMove;

    for(unsigned int i=0; i<this->clones.size();i++){
        int penalty = 1;
        Clone clone = this->clones[i];
        if(!clone.clone.currentBoard){
            penalty = 30;
        }

        int score = (clone.weightedWins-clone.weightedLosses*penalty) / (clone.wins + clone.losses + clone.ties);

        if(score > bestScore){
            bestScore = score;
            bestMove = clone;
        }
    }

    game.playCell(bestMove.boardRow,bestMove.boardCol,bestMove.cellRow,bestMove.cellCol);

}

void MonteBot::startCalculation(Game &game){
    std::cout << "MonteBotStartCalculation" << std::endl;

    Move move;
    std::vector<Move> validMoves = game.getValidMoves();
    std::cout << "validMoves.length(): " << validMoves.size() << std::endl;

    clones.clear();

    Game *clone;

    for(unsigned int i=0; i<validMoves.size();i++){
        move = validMoves[i];
        clone = new Game(game);
        clone->playCellSilently(move.bRow,move.bCol,move.cRow,move.cCol);

        clones.push_back(Clone(move.bRow,move.bCol,move.cRow,move.cCol,*clone,0,0,0,0));
    }
}

int MonteBot::playOutHidden(Game &game){
    while(!game.finished){
        Move move = getRandomValidMove(game);
        game.playCellSilently(move.bRow,move.bCol,move.cRow,move.cCol);
    }

    return game.winner;
}

Move MonteBot::getRandomValidMove(Game &game){

    std::vector<Move> moves = game.getValidMoves();
    int moveNum = std::rand() % moves.size();

    return moves[moveNum];
}


void MonteBot::calculateAhead(Game &game){

    std::cout << "MonteBotCalculateAhead" << std::endl;


    for(unsigned int i=0; i<clones.size();i++){
        Clone &move = clones[i];

        Game *simGame = new Game(move.clone);
        int winner = playOutHidden(*simGame);

        double weight = 1;

        if(monteWeightByGameLength){
            //number of moves it took for sim to finish
            double f = simGame->countFilled - game.countFilled;
            //number of moves remaining in game
            double r = 81 - game.countFilled;
            //add a weight such that if the game
            //finishes in 0 moves the weight is 1, if
            //it drags out until the end then it is
            //worth only 1/# moves till the end and
            //anything in between is inversely
            //proportional to the number of moves it
            //takes.
            weight = (r - f) / r;
            //debug(f,r,weight);
        }
        if (winner == 0) {
            move.ties += 1;
        } else
            if(winner == game.currentPlayer){
                move.wins += 1;
                move.weightedWins += 1*weight;
            }  else {
                move.losses +=1;
                move.weightedLosses += 1* weight;
            }
    }
}
