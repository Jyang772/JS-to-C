#ifndef GAME_H
#define GAME_H

#include "move.h"
#include "tictactoeboard.h"

#include <vector>
#include <iostream>

class Game{
public:
    Game();

    Game(Game& other){
        this->currentPlayer = other.currentPlayer;
        this->finished = other.finished;
        this->winner = other.winner;
        this->countFilled = other.countFilled;

        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                this->boards[i][j] = new TicTacToeBoard(i,j);
            }
        }

        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                //Receiving copy
                *this->boards[i][j] = other.boards[i][j]->clone();
            }
        }

        this->currentBoard = nullptr;

        if(other.currentBoard){
            // currentBoard = new TicTacToeBoard(0,0);

            //this->currentBoard = this->boards[other.currentBoard->row][other.currentBoard->col];
            this->currentBoard = boards[other.currentBoard->row][other.currentBoard->col];
        }


    }

    void playCellSilently(int board_row, int board_col, int cell_row, int cell_col);
    void playCell(int board_row, int board_col, int cell_row, int cell_col);

    std::vector<Move> getValidMoves();
    std::vector<TicTacToeBoard> getNonFinishedBoards();
    bool checkWonGame(int board_row, int board_col, bool silent);


    int currentPlayer;
    bool finished;
    int winner;
    int countFilled;
    bool useRule5b = true;
    TicTacToeBoard *currentBoard; //Pointer to currentBoard
    TicTacToeBoard *boards[3][3];

};



#endif // GAME_H
