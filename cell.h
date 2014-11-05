#ifndef CELL_H
#define CELL_H

class Cell{
public:
    Cell(int row, int col);
    Cell& clone();
    void setWinner(int winner);
    void playCell(int newOwner);
    int row;
    int col;
    int owner;
    int winner;
};


#endif // CELL_H
