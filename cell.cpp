#include "cell.h"

Cell::Cell(int row, int col){
    this->row = row;
    this->col = col;
    owner = 0;
    winner = 0;
}


Cell& Cell::clone(){
    Cell temp(this->row, this->col);
    temp.owner = this->owner;
    temp.winner = this->winner;

    return temp;
}

void Cell::setWinner(int winner){
    this->winner = winner;
}

void Cell::playCell(int newOwner){

    this->owner = newOwner;

    if(!this->owner)
        return;
    if(!this->winner){
        //
    }
}
