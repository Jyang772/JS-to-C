#ifndef CLONE_H
#define CLONE_H

#include "game.h"

class Clone{
public:

    Clone(){}
    Clone(int boardRow,int boardCol,int cellRow,int cellCol,Game clone,int wins,double weightedWins,double weightedLosses,double ties);
    Clone(const Clone& other);
    Clone& operator=(const Clone& other);
    int boardRow;
    int boardCol;
    int cellRow;
    int cellCol;
    Game clone;
    int wins;
    double weightedWins;
    double losses;
    double weightedLosses;
    double ties;
};


#endif // CLONE_H
