#include <iostream>
#include <vector>

#include <time.h>
#include <ctime>
#include <cstdlib>

using namespace std;

bool monteWeightByGameLength = true;


class Cell{
public:
    Cell(int row, int col){
        this->row = row;
        this->col = col;
        owner = 0;
        winner = 0;
    }

    Cell& clone();
    void setWinner(int winner);
    void playCell(int newOwner);
    int row;
    int col;
    int owner;
    int winner;
};

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

class TicTacToeBoard{
public:
    TicTacToeBoard(int row, int col){
        this->row = row;
        this->col = col;
        winner = 0;

        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                cells[i][j] = new Cell(i,j);
            }
        }


    }

    bool operator!=(const TicTacToeBoard& other){

        if(this->row != other.row)
            return true;
        if(this->col != other.col)
            return true;

        if(this->winner != other.winner)
            return true;

        for(int i=0;i<3; i++){
            for(int j=0; j<3; j++){
                if(cells[i][j] != other.cells[i][j])
                    return true;
            }

        }

        return false;
    }

    TicTacToeBoard& clone();
    bool isFull();
    std::vector<Cell> getEmptyCells();
    bool playCellSilently(int row, int col, int player);
    bool playCell(int row, int col, int player);
    bool checkWon(int row, int col);

    int row;
    int col;
    int winner;
    Cell *cells[3][3];

};

TicTacToeBoard& TicTacToeBoard::clone(){

    TicTacToeBoard clone(this->row,this->col);

    clone.winner = this->winner;


    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            clone.cells[i][j] = this->cells[i][j];
        }
    }

    return clone;
}

bool TicTacToeBoard::isFull(){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(!this->cells[i][j]->owner){
                return false;
            }
        }
    }
    return true;
}

std::vector<Cell> TicTacToeBoard::getEmptyCells(){

    std::vector<Cell> emptyCells;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(!this->cells[i][j]->owner){
                emptyCells.push_back(*this->cells[i][j]);
            }
        }
    }

    return emptyCells;
}

bool TicTacToeBoard::playCellSilently(int row, int col, int player){
    Cell cell = *this->cells[row][col];

    cell.owner = player;
    if(!this->winner){
        bool justWon = this->checkWon(row, col);
        if(justWon){
            cout << "JustWon!!";
            this->winner = player;
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    this->cells[i][j]->winner = player;
                }
            }
        }
        return justWon;
    }

    return false;
}

bool TicTacToeBoard::playCell(int row, int col, int player){
    Cell cell = *this->cells[row][col];

    if(!cell.owner){
        cell.playCell(player);
        bool won = false;
        if(!this->winner){
            won = this->checkWon(row, col);
            if(won){
                this->winner = player;
                for(int i=0; i<3; i++){
                    for(int j=0; j<3; j++){
                        this->cells[i][j]->setWinner(player);
                    }
                }
            }
        }
        return won;
    } else{
        //Should not get here.
    }

    return false;
}

bool TicTacToeBoard::checkWon(int row, int col){
    int player = this->cells[row][col]->owner;

    int i=0;
    while(i<3 &&
          this->cells[row][i]->owner &&
          this->cells[row][i]->owner == player){
        i += 1;
    }
    if(i == 3){
        return true;
    }

    //check the col
    i=0;
    while(i<3 &&
          this->cells[i][col]->owner &&
          this->cells[i][col]->owner== player){
        i += 1;
    }
    if(i == 3){
        return true;
    }

    //check the upper left to lower right diagnol
    i=0;
    while(i<3 &&
          this->cells[i][i]->owner &&
          this->cells[i][i]->owner== player){
        i += 1;
    }
    if(i == 3){
        return true;
    }

    //check the uper right to lower left diagnol
    i=0;
    while(i<3 &&
          this->cells[i][2-i]->owner &&
          this->cells[i][2-i]->owner == player){
        i += 1;
    }
    if(i == 3){
        return true;
    }
    //None of the rows, cols, or diagnols were winning
    return false;

}

struct Move{
    Move(){}
    Move(int bRow,int bCol,int cRow,int cCol){
        this->bRow = bRow;
        this->bCol = bCol;
        this->cRow = cRow;
        this->cCol = cCol;
    }

    int bRow;
    int bCol;
    int cRow;
    int cCol;
};


class Game{
public:
    Game(){
        this->currentPlayer = 1;
        this->currentBoard = NULL;
        this->finished = false;
        this->winner = 0;
        this->countFilled = 0;

        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                this->boards[i][j] = new TicTacToeBoard(i,j);
            }
        }

    }

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

        this->currentBoard = NULL;

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


void Game::playCellSilently(int board_row, int board_col, int cell_row, int cell_col){

    TicTacToeBoard *board = this->boards[board_row][board_col];
    //Cell cell = *board.cells[cell_row][cell_col];
    bool justWon = board->playCellSilently(cell_row, cell_col, this->currentPlayer);

    this->countFilled += 1;
    if(justWon){
        if(this->checkWonGame(board_row, board_col,true)){
            this->finished = true;
            this->winner = this->currentPlayer;
            return;
        }
    }

    this->currentBoard = this->boards[cell_row][cell_col];
    if(this->currentBoard->isFull()){
        this->currentBoard = NULL;
    }
    else if(this->currentBoard->winner && useRule5b){
        this->currentBoard = NULL;
    }

    //check for draws
    if(this->countFilled == 81){
        this->finished = true;
        return;
    }
    else if(useRule5b && !this->getNonFinishedBoards().size()){
        this->finished = true;
        return;
    }

    if(this->currentPlayer == -1){
        this->currentPlayer = 1;
    }
    else{
        this->currentPlayer = -1;
    }
}

void Game::playCell(int board_row, int board_col, int cell_row, int cell_col){

    TicTacToeBoard *board = this->boards[board_row][board_col];
    if(this->currentBoard && *board != *this->currentBoard){
        //ERROR WRONG BOARD
        return;
    }

    Cell *cell = board->cells[cell_row][cell_col];



    if(cell->owner){
        return;
    }

    bool justWon = board->playCell(cell_row,cell_col,this->currentPlayer);

    this->countFilled += 1;
    if(this->currentBoard){
        //unhiglight
    }

    if(justWon){
        if(this->checkWonGame(board_row,board_col,true)){
            this->finished = true;
            this->winner = this->currentPlayer;
            //Show winning screen
            return;
        }
    }

    this->currentBoard = this->boards[cell_row][cell_col];

    if(this->currentBoard->isFull()){
        this->currentBoard = NULL;
        if(this->countFilled == 81){
            this->finished = true;
            //Show draw screen
        }
    }
    else if(this->currentBoard->winner && useRule5b){
        if(!this->getNonFinishedBoards().size()){
            this->finished = true;
            //show draw screen
        }

        cout << "WINNER: " << endl;
        this->currentBoard = NULL;
    }
    else{
        //this->currentBoard->hightlight;
    }

    if(this->currentPlayer == -1){
        currentPlayer = 1;
    }
    else{
        this->currentPlayer = -1;
        std::cout << "Board Row: " << board_row << endl;
        cout << "Board Col: " << board_col << endl;
        cout << "Cell Row: " << cell_row << endl;
        cout << "Cell Col: " << cell_col << endl;
    }

}

std::vector<Move> Game::getValidMoves(){
    std::vector<TicTacToeBoard> validBoards;

    if(this->currentBoard){
        validBoards.push_back(*this->currentBoard);
    }
    else{
        if(useRule5b){
            validBoards = this->getNonFinishedBoards();
        }
    }


    std::vector<Move> validMoves;
    cout << validBoards.size() << endl;
    for(unsigned int i=0; i<validBoards.size(); i++){
        TicTacToeBoard board = validBoards[i];
        std::vector<Cell> validCells = board.getEmptyCells();

        for(unsigned int j=0; j<validCells.size();j++){
            Cell cell = validCells[j];
            validMoves.push_back(Move(board.row,board.col,cell.row,cell.col));
        }
    }

    return validMoves;
}

std::vector<TicTacToeBoard> Game::getNonFinishedBoards(){
    std::vector<TicTacToeBoard> nonFinishedBoards;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            TicTacToeBoard board = *this->boards[i][j];
            if(!board.winner && !board.isFull()){
                nonFinishedBoards.push_back(board);
            }
        }
    }

    return nonFinishedBoards;
}

bool Game::checkWonGame(int board_row, int board_col, bool silent){

    /* check if the move at row, col won the board */
    int player = this->boards[board_row][board_col]->winner;
    //check the row
    int i=0;
    while(i<3 &&
          this->boards[board_row][i]->winner &&
          this->boards[board_row][i]->winner== player) {
        i += 1;
    }
    if(i == 3){
        if(!silent) {
            for(i=0; i<3; i++) {
                //this->boards[row][i]->highlight();
            }
        }
        return true;
    }

    //check the col
    i=0;
    while(i<3 &&
          this->boards[i][board_col]->winner &&
          this->boards[i][board_col]->winner == player) {
        i += 1;
    }
    if(i == 3){
        if(!silent) {
            for(i=0; i<3; i++) {
                //this->boards[i][col].highlight();
            }
        }
        return true;
    }

    //check the upper left to lower right diagnol
    i=0;
    while(i<3 &&
          this->boards[i][i]->winner &&
          this->boards[i][i]->winner == player) {
        i += 1;
    }
    if(i == 3){
        if(!silent) {
            for(i=0; i<3; i++) {
                //this.boards[i][i].highlight();
            }
        }
        return true;
    }

    //check the uper right to lower left diagnol
    i=0;
    while(i<3 &&
          this->boards[i][2-i]->winner &&
          this->boards[i][2-i]->winner == player) {
        i += 1;
    }
    if(i == 3){
        if(!silent) {
            for(i=0; i<3; i++) {
                //this.boards[i][2-i].highlight();
            }
        }
        return true;
    }
    //None of the rows, cols, or diagnols were winning
    return false;
}


class Clone{
public:

    Clone(){}
    Clone(int boardRow,int boardCol,int cellRow,int cellCol,Game clone,int wins,double weightedWins,double weightedLosses,double ties){
        this->boardRow = boardRow;
        this->boardCol = boardCol;
        this->cellRow = cellRow;
        this->cellCol = cellCol;
        this->clone = clone;
        this->wins = wins;
        this->weightedWins = weightedWins;
        this->losses = losses;
        this->weightedLosses = weightedLosses;
        this->ties = ties;
    }
    Clone(const Clone& other){

        this->boardRow = other.boardRow;
        this->boardCol = other.boardRow;
        this->cellRow = other.cellRow;
        this->cellCol = other.cellCol;
        this->wins = other.wins;
        this->weightedWins = other.weightedWins;
        this->weightedLosses = other.weightedLosses;
        this->losses = other.losses;
        this->ties = other.ties;

        this->clone = other.clone;

    }

    Clone& operator=(const Clone& other){

        this->boardRow = other.boardRow;
        this->boardCol = other.boardRow;
        this->cellRow = other.cellRow;
        this->cellCol = other.cellCol;
        this->wins = other.wins;
        this->weightedWins = other.weightedWins;
        this->weightedLosses = other.weightedLosses;
        this->losses = other.losses;
        this->ties = other.ties;

        this->clone = other.clone;
    }

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

class MonteBot{
public:
    void startCalculation(Game&);
    void calculateAhead(Game&);
    void play(Game&);
    int playOutHidden(Game&);
    Move getRandomValidMove(Game &);
    std::vector<Clone> clones;
};


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
    std::cout << "MonteBotStartCalculation" << endl;

    Move move;
    std::vector<Move> validMoves = game.getValidMoves();
    std::cout << "validMoves.length(): " << validMoves.size() << endl;

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

    cout << moves.size() << endl;
    return moves[moveNum];
}


void MonteBot::calculateAhead(Game &game){

    cout << "MonteBotCalculateAhead" << endl;


    for(unsigned int i=0; i<clones.size();i++){
        Clone &move = clones[i];

        Game *simGame = new Game(move.clone);
        int winner = playOutHidden(*simGame);
        cout << "SimGame Winner: " << simGame->winner << endl;

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



