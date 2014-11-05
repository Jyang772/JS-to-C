#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H


/** In-Tic-Tac-Toe-Ception
 *
Tic-tac-toe
Tic-tac-toe is a simple, 2-player, strategy game played on a board of 9 cells set out in a 3x3 square.  The first player places an "X" an any cell.  The second player places an "0" in any remaining cell.  The players continue taking turns placing their mark in remaining cells.  The winner is the first player to place three of his marks in a row, column, or diagonal.

TIC-TAC-TOE-CEPTION

Tic-tac-toe-ception is a 2-player computer game played on a game board comprising nine tic-tac-toe games set out in nine mini-squares within a 3x3 greater-square.  Thus, there are 81 cells, arranged in nine 3x3 mini-squares within a greater square.

Objective:
The objective of this game is to be the first to win three tic-tac-toe games lying in a row, column, or diagonal within the greater-square.

Rules of Play:
1.  The first player may place an "X" in any cell within any mini-square on the board.
2.  The selected cell position within this mini-square corresponds to the mini-square position within the greater-square where the second player must then place an "O".
3.  Thereafter, the two players take turns placing their mark in any unfilled cell within the mini-square dictated by the cell position marked by the previous player.  For the first player, this mini-square will be outlined in red.
4.  (variation A) The first tic-tac-toe winner in a mini-square remains the winner in that mini-square for the remainder of the game.
4. (variation B - currently not implemented) - Anytime you get 3 in a row in any mini board, you become a winner of that board. If the other player also gets 3 in a row in that board, either before or after they are then co-owners of the board with you.
5 (variation A).  If a player is sent to a mini-square in which all the cells are filled, the player may next place his mark in any unfilled cell in any other mini-board.
5 (variation B - current default).  If a player is sent to a mini-square that has already been won, or in which all the cells are already filled, then the player may next place his mark in any unfilled cell in any other mini-board.
5 (variation C - currently not implemented). If a player is sent to a mini-square that has already been won, they must play in that mini-square (if not empty), but then they get to go again.  If a player is sent to a totally filled square then they may go anywhere.
5 (variation D - currently not implemented). If a player is sent to a mini-square that has already been won, they must play in that mini-square (if not empty) but then they get to continue to keep control and continue to send themselves to other boards that have already been won until they send themselves and play in a board that has not yet been won.
 *
 * AI discussion:
 * There are three different Bots now.  A random bot, which just plays randomly.  A monte carlo bot (which plays out as many random games as it can within 5 seconds and plays which ever next position had the greatest percentage of wins).  There is also a perfect bot, which if played as X should never lose under rule 5a.  Under 5b its strategy is very bad.
 *
 * TODO: make Perfect Bot as "O" be perfect if "X" messes up under rule 5a.
 * TODO: implement rule 5c
 * TODO: implement rule 5d
 * TODO: implement rule 4b
 * TODO: modify Monte to better detect when the next move might cause the opponent to win
 * TODO: when the number of a available moves left becomes small enough to be able to fully calculate out all possible future moves, consider doing this (though Monte might handle this just fine)
 * TODO: rearrange the code a bit to be able to reuse the same class for the large board and the mini-boards, and hence allow one more layer of recursion.  One possilbe rule of where you are forced to play is by keeping track of the last two moves.  The move two turns ago says which big board to play on. The move one turn ago says which mid-sized board to play on.
 * TODO: crazy long term idea is to allow an arbitrary depth of recursion and allow people to zoom in or out of the board. (please feel free to write a spin-off that does this so I don't have to spend another weekend doing it myself :)
 *
 * Search space:
 * The first move of the game has 81 different possibilities.
 * After that the moves are constrained to a single board.
 * There will be 8 boards (the first board has one filled in
 * already) that have 9 choices when you first arrive. After
 * that there will 9 board with 8 choices, 9 with 7 choices,
 * 9 with 6, etc, until you are forced to move in the last
 * spot. So at the beginning of the game there will be:
 * 81*9^8*8^9*7^9*6^9*5^9*4^9*3^9*2^9 = 9.8*10^50 ways to
 * randomly fill in the whole board. Many paths will win the
 * game early, so the actual search space will be less than
 * this, but as you can see the number would still be way to
 * big to brute force.
 **/
 //the computer player may be "random", "monte", or "perfect"
 //monte bot will take a sec per move (you can change that) to calculate what it thinks is best - it seems to pretty much beat RandomBot every time.
 //perfect bot only works as "x" for now and only if useRule5b is false (currently crashes if it is true).  Before you choose perfect play against yourself and see if you can figure out how you can guarantee a win.
var useRule5b = true;
var xIsComputer = "monte";
var oIsComputer = false;
var botThinkingTime = 2000; //in ms for MonteCarloBot
//whether the monteCarloBot should weight each outcome by how quickly it is won/lost (this will hopefully dicourage Monte from losing quickly when it can instead drag the game out, likewise it will hopefully encourage Monte to choose options that will win the game more quickly).
var monteWeightByGameLength = true;
//sometimes it might be better strategically to move somewhere that will give the opponent a choice of boards to play on, but this will be very rare. To help MonteCarloBot avoid giving their opponent a choice, count any loss this number of times more than any win.  Monte needs this because it does a bad job of evaluating when an opponent has choice because it will be randomly placing the next move on all squares on the board, many which are bad and a smart opponent will avoid.  Another possibility to handle this is to run another monte carlo simulation for the opponent for that move with choice and score them by that.  For now this is a simpler approach -- with a high enough value Monte will avoid giving their opponent any choice unless no matter what the opponent choose randomly they ALWAYS lost.
var monteChoicePenalty=10;
var debugMode = false;
var debugHistory = true;
//the size of the buffer around each board
var boardBuffer=5;
//the size of the buffer around each players mark
var cellBuffer=5;
var cellSize=38;
var xColor = color(255, 0, 0);
var yColor = color(34, 54, 145);

var Player = function(pieceColor, backgroundColor, name) {
    this.color = pieceColor;
    this.background = backgroundColor;
    this.name = name;
};

var PlayerX = function() {
    Player.call(this, color(0, 0, 0), color(201, 106, 106), "X");
};

PlayerX.prototype = Object.create(Player.prototype);

PlayerX.prototype.drawShape = function(x,y) {
    //buffer
    var b = cellBuffer;
    //size of shape
    var s = cellSize-2*b;
    //width of line
    strokeWeight(3);
    stroke(this.color);
    line(x+b,y+b,x+b+s, y+b+s);
    line(x+b+s,y+b,x+b, y+b+s);
};

var playerX = new PlayerX();

var PlayerO = function() {
    Player.call(this, color(0, 0, 0), color(105, 174, 199), "O");
};

PlayerO.prototype = Object.create(Player.prototype);

PlayerO.prototype.drawShape = function(x,y) {
    //buffer
    var b = 5;
    //size of shape
    var s = cellSize-2*b;
    //width of line
    strokeWeight(3);
    stroke(this.color);
    ellipse(x+b+s/2,y+b+s/2,s,s);
};

var playerO = new PlayerO();

//from jquery
var arrayRemove = function(array, from, to) {
  var rest = array.slice((to || from) + 1 || array.length);
  array.length = from < 0 ? array.length + from : from;
  return array.push.apply(array, rest);
};

var Cell = function(row, col) {
    this.size = 38;
    //The row and col of the cell within its board
    this.row = row;
    this.col = col;
    this.owner = null;
    //keeps track of the winner of the board this cell is in
    this.winner = null;
};

Cell.prototype.clone = function() {
    var clone = new Cell(this.row, this.col);
    clone.owner = this.owner;
    clone.winner = this.winner;
    return clone;
};

Cell.prototype.setPos = function(x,y){
    //The pixel coords of the cell
    this.x = x;
    this.y = y;
};

Cell.prototype.setWinner = function(winner) {
    this.winner = winner;
    this.setBackground(this.winner.background);
};

Cell.prototype.setBackground = function(fillColor) {
    noStroke();
    fill(fillColor);
    rect(this.x,this.y,this.size,this.size);
    if(this.owner) {
        this.owner.drawShape(this.x, this.y);
    }
};

Cell.prototype.playCell = function(newOwner) {
    this.owner = newOwner;
    if(!this.owner) {
        return;
    }
    if(!this.winner) {
        this.setBackground(this.owner.background);
    } else {
        this.setBackground(this.winner.background);
    }
};

var TicTacToeBoard = function(row, col) {
    this.row = row;
    this.col = col;
    this.size=130;
    this.innerSize=this.size-2*boardBuffer;
    this.winner=null;

    this.cells=[[],[],[]];
    for(var i=0; i<3; i++){
        for(var j=0; j<3; j++){
            this.cells[i][j] = new Cell(i, j);
        }
    }
};

TicTacToeBoard.prototype.clone = function() {
    var clone = new TicTacToeBoard(this.row, this.col);
    clone.winner = this.winner;
    for(var i=0; i<3; i++){
        for(var j=0; j<3; j++){
            clone.cells[i][j] = this.cells[i][j].clone();
        }
    }
    return clone;
};

TicTacToeBoard.prototype.highlight = function() {
    strokeWeight(3);
    stroke(255, 0, 0);
    noFill();
    rect(this.x+1,this.y+1, this.size-3, this.size-3);
};

TicTacToeBoard.prototype.unhighlight = function() {
        strokeWeight(3);
        stroke(255, 255, 255);
        noFill();
        rect(this.x+1,this.y+1, this.size-3, this.size-3);
};

TicTacToeBoard.prototype.draw = function(y,x) {
    noStroke();
    var b=boardBuffer;
    //the position of the board on the screen
    this.y = y;
    this.x = x;
    //size of the board
    var s = this.size;
    //inner size of board
    var i = this.innerSize;
    //size of cell
    var c = 38;
    //width of dividing line
    var w = 3;
    //relative pos of first line
    var d1 = c;
    //relative pos of second line
    var d2 = 2*c+w;
    //relative pos of the cells
    this.p = [0,d1+w,d2+w];

    //set background color
    if(this.winner) {
        fill(this.winner.background);
    } else {
        //fill(255, 255, 0);
        fill(255, 255, 255);
    }

    //draw background
    rect(this.x,this.y,s,s);

    fill(0, 0, 0);
    rect(this.x+b,this.y+b+d1,i,w);
    rect(this.x+b,this.y+b+d2,i,w);
    rect(this.x+b+d1,this.y+b,w,i);
    rect(this.x+b+d2,this.y+b,w,i);

    for(var row=0; row<3; row++){
        for(var col=0; col<3; col++){
            var cell = this.cells[row][col];
            cell.setPos(this.x+b+this.p[col],
                        this.y+b+this.p[row]);
            if(cell.owner) {
               cell.playCell(cell.owner);
            }
        }
    }
};

TicTacToeBoard.prototype.isFull = function() {
        for(var i=0; i<3; i++) {
            for(var j=0; j<3; j++){
                if(!this.cells[i][j].owner) {
                    return false;
                }
            }
        }
        return true;
};

TicTacToeBoard.prototype.getEmptyCells = function(){
    /* Gets a list of the empty cells on the board
    *
    * TODO: This gets called a lot from the AI so we
    * should probably cache it.
    */
    var emptyCells = [];
    for(var i=0; i<3; i++){
        for(var j=0; j<3; j++){
            if(!this.cells[i][j].owner){
                emptyCells.push(this.cells[i][j]);
            }
        }
    }
    return emptyCells;
};

TicTacToeBoard.prototype.getCellCoords = function(x,y) {
    var relX = x-this.x;
    var relY = y-this.y;
    var row, col;
    var b=boardBuffer;
    if (relY<b+this.p[1]){
        row = 0;
    } else if(relY<b+this.p[2]){
        row = 1;
    } else {
        row = 2;
    }
    if (relX<b+this.p[1]){
        col = 0;
    } else if(relX<b+this.p[2]){
        col = 1;
    } else {
        col = 2;
    }
    return [row, col];
};

TicTacToeBoard.prototype.playCellSilently = function(row, col, player) {
        /** Plays the cell without showing the UI */
        var cell = this.cells[row][col];
        cell.owner = player;
        if(!this.winner){
            var justWon = this.checkWon(row, col);
            if(justWon) {
                this.winner = player;
                for(var i=0; i<3; i++) {
                    for(var j=0; j<3; j++) {
                        this.cells[i][j].winner=player;
                    }
                }
            }
            return justWon;
        }
        return false;
};

TicTacToeBoard.prototype.playCell = function(row, col, player) {
        var cell = this.cells[row][col];

        if (!cell.owner) {
            cell.playCell(player);
            var won = false;
            if(!this.winner){
                won = this.checkWon(row, col);
                if(won) {
                    this.winner = player;
                    for(var i=0; i<3; i++) {
                        for(var j=0; j<3; j++) {
                            this.cells[i][j].setWinner(
                                player);
                        }
                    }
                }
            }
            return won;
        } else {
            debug("Should not get here");
        }
        return false;

};

TicTacToeBoard.prototype.checkWon = function(row, col) {
        /* check if the move at row, col won the board */
        var player = this.cells[row][col].owner;
        //check the row
        var i=0;
        while(i<3 &&
              this.cells[row][i].owner &&
              this.cells[row][i].owner.name === player.name){
            i += 1;
        }
        if(i === 3){
            return true;
        }

        //check the col
        i=0;
        while(i<3 &&
              this.cells[i][col].owner &&
              this.cells[i][col].owner.name === player.name){
            i += 1;
        }
        if(i === 3){
            return true;
        }

        //check the upper left to lower right diagnol
        i=0;
        while(i<3 &&
              this.cells[i][i].owner &&
              this.cells[i][i].owner.name === player.name){
            i += 1;
        }
        if(i === 3){
            return true;
        }

        //check the uper right to lower left diagnol
        i=0;
        while(i<3 &&
              this.cells[i][2-i].owner &&
              this.cells[i][2-i].owner.name === player.name){
            i += 1;
        }
        if(i === 3){
            return true;
        }
        //None of the rows, cols, or diagnols were winning
        return false;
};

var Game = function(game) {
    this.size = 400;

    if (game) {
        this.currentPlayer = game.currentPlayer;
        this.finished = game.finished;
        this.winner = game.winner;
        this.countFilled = game.countFilled;
        this.moveHistory = game.moveHistory;
        this.boards = [[],[],[]];
        for(var row=0; row<3; row++){
            for(var col=0; col<3; col++){
                this.boards[row][col] = game.boards[row][col].clone();
            }
        }
        this.currentBoard = null;
        if (game.currentBoard) {
            this.currentBoard = this.boards[game.currentBoard.row]
                [game.currentBoard.col];
        }
    } else {
        this.currentPlayer = playerX;
        this.currentBoard = null;
        this.finished = false;
        this.winner = null;
        this.countFilled = 0;
        this.moveHistory = "";

        this.boards = [[],[],[]];
        for(var row=0; row<3; row++){
            for(var col=0; col<3; col++){
                this.boards[row][col] = new TicTacToeBoard(row, col);
            }
        }
    }
};

Game.prototype.draw = function() {
        noStroke();
        //size of board
        var s = 400;
        //size of cell
        var c = 130;
        //width of divider
        var w = 5;
        //pos of first line
        var d1 = c;
        //pos of second line
        var d2 = 2*c+w;
        //pos of the cells
        this.p = [0,d1+w,d2+w];
        fill(0, 0, 0);
        rect(0,d1,s,w);
        rect(0,d2,s,w);
        rect(d1,0,w,s);
        rect(d2,0,w,s);

        for(var row=0; row<3; row++){
            for(var col=0; col<3; col++){
                var b = this.boards[row][col];
                b.draw(this.p[row], this.p[col]);
            }
        }

};

Game.prototype.playCellSilently = function(board_row, board_col,
                                     cell_row, cell_col) {
        /** Plays move without showing to the UI
        *
        * This is useful for Bots. It assumes valid input.
        **/
        this.moveHistory += "game.playCell("+board_row+","+
            board_col+","+
            cell_row+","+
            cell_col+");\n";

        var board = this.boards[board_row][board_col];
        var cell = board.cells[cell_row][cell_col];
        var justWon = board.playCellSilently(
            cell_row,cell_col, this.currentPlayer);
        //debug("playing at", board_row, board_col,
        //                             cell_row, cell_col);
        this.countFilled += 1;
        if(justWon) {
            if(this.checkWonGame(board_row, board_col,
                true)) {
                this.finished=true;
                this.winner = this.currentPlayer;
                return;
            }
        }
        this.currentBoard = this.boards[cell_row][cell_col];
        if(this.currentBoard.isFull()) {
            this.currentBoard = null;
        } else if (this.currentBoard.winner && useRule5b) {
            this.currentBoard = null;
        }

        //check for draws
        if(this.countFilled === 81) {
            this.finished=true;
            return;
        } else if (useRule5b && !this.getNonFinishedBoards().length) {
            this.finished=true;
            return;
        }

        if(this.currentPlayer.name === playerX.name){
            this.currentPlayer = playerO;
        } else {
            this.currentPlayer = playerX;
        }
};

Game.prototype.playCell = function(board_row, board_col,
                            cell_row, cell_col) {
            this.moveHistory += "game.playCell("+board_row+","+
            board_col+","+
            cell_row+","+
            cell_col+");\n";
        var board = this.boards[board_row][board_col];
        if(this.currentBoard &&
           board !== this.currentBoard){
            //TODO: show error message or flash correct board
            debug("Error - wrong board");
            debug(this.currentBoard);
            return;
        }
        var cell = board.cells[cell_row][cell_col];

        if(cell.owner) {
            //Invalid play, most likely just held down mouse
            //over last move, so ignoring
            return;
        }

        var justWon = board.playCell(cell_row,cell_col,
                                         this.currentPlayer);
        this.countFilled += 1;
        if(this.currentBoard) {
            this.currentBoard.unhighlight();
        }

        if(justWon) {
            if(this.checkWonGame(board_row, board_col)) {
                this.finished=true;
                this.winner = this.currentPlayer;
                this.showWinningScreen();
                return;
            }
        }

        this.currentBoard = this.boards[cell_row][cell_col];
        if(this.currentBoard.isFull()) {
            this.currentBoard = null;
            if(this.countFilled === 81) {
                this.finished=true;
                this.showDrawScreen();
            }
        } else if (this.currentBoard.winner && useRule5b) {
            if (!this.getNonFinishedBoards().length) {
                this.finished=true;
                this.showDrawScreen();
            }
            this.currentBoard = null;
        } else {
            this.currentBoard.highlight();
        }

        if(this.currentPlayer === playerX){
            this.currentPlayer = playerO;
        } else {
            this.currentPlayer = playerX;
        }
};

Game.prototype.playAt = function(x,y) {
        var row = floor(y/(this.size/3));
        var col = floor(x/(this.size/3));
        var board = this.boards[row][col];
        var cell_coords = board.getCellCoords(x,y);
        var cell_row = cell_coords[0];
        var cell_col = cell_coords[1];
        this.playCell(row,col,cell_row,cell_col);
};

Game.prototype.getValidMoves = function() {
        var validBoards = [];
        if(this.currentBoard) {
            validBoards.push(this.currentBoard);
        } else {
            //Choose a random board
            if(useRule5b) {
                validBoards=this.getNonFinishedBoards();
            } else {
                validBoards=this.getNonFullBoards();
            }
        }
        var validMoves = [];
        for(var i=0; i<validBoards.length; i++){
            var board = validBoards[i];
            var validCells=board.getEmptyCells();
            for (var j=0; j<validCells.length; j++){
                var cell = validCells[j];
                validMoves.push([board.row, board.col,
                                 cell.row, cell.col]);
            }

        }
        return validMoves;
};

Game.prototype.getNonFinishedBoards = function() {
        /* Returns a list of the boards that aren't finished
        *
        * A board is considered finished if it is full or
        * if someone has won it.
        * TODO: This might get called a lot from
        * the Bots so we should probably cache it.
        */
        var nonFinishedBoards=[];
        for(var i=0; i<3; i++){
            for(var j=0; j<3; j++){
                var board = this.boards[i][j];
                if(!board.winner && !board.isFull()){
                    nonFinishedBoards.push(board);
                }
            }
        }
        return nonFinishedBoards;
};

Game.prototype.getNonFullBoards = function() {
        /* Returns a list of the non-full boards
        *
        * TODO: This might get called a lot from
        * the Bots so we should probably cache it.
        */
        var nonFullBoards=[];
        for(var i=0; i<3; i++){
            for(var j=0; j<3; j++){
                if(!this.boards[i][j].isFull()){
                    nonFullBoards.push(
                        this.boards[i][j]);
                }
            }
        }
        return nonFullBoards;
};

Game.prototype.showDrawScreen = function() {
        fill(255, 255, 255);
        strokeWeight(3);
        stroke(46, 209, 65);
        var boxx = 95;
        var boxy = 150;
        rect(boxx, boxy, 210, 100);
        fill(0,0,0);
        textSize(40);
        text("The game", boxx+10, boxy+48);
        text("is a draw!", boxx+18, boxy+90);
};

Game.prototype.showWinningScreen = function(player){
        if(player) {
            for(var i=0; i<3; i++) {
                for(var j=0; j<3; j++) {
                    for(var k=0; k<3; k++) {
                        for (var l=0; l<3; l++) {
                            this.boards[i][j].cells[k][l].
                                setBackground(
                                    this.currentPlayer
                                    .background);
                        }
                    }
                }
            }
        }
        fill(255, 255, 255);
        strokeWeight(3);
        stroke(46, 209, 65);
        var boxx = 95;
        var boxy = 150;
        rect(boxx, boxy, 210, 100);
        var winnerImage = new Cell();
        winnerImage.setPos(boxx+10, boxy+10);
        winnerImage.playCell(this.currentPlayer);
        fill(this.currentPlayer.background);
        textSize(40);
        text("has won", boxx+55, boxy+48);
        text("the game!", boxx+17, boxy+90);
};

Game.prototype.checkWonGame = function(row, col, silent) {
        /* check if the move at row, col won the board */
        var player = this.boards[row][col].winner;
        //check the row
        var i=0;
        while(i<3 &&
              this.boards[row][i].winner &&
              this.boards[row][i].winner.name === player.name
              ) {
            i += 1;
        }
        if(i === 3){
            if(!silent) {
                for(i=0; i<3; i++) {
                    this.boards[row][i].highlight();
                }
            }
            return true;
        }

        //check the col
        i=0;
        while(i<3 &&
              this.boards[i][col].winner &&
              this.boards[i][col].winner.name === player.name
              ) {
            i += 1;
        }
        if(i === 3){
            if(!silent) {
                for(i=0; i<3; i++) {
                    this.boards[i][col].highlight();
                }
            }
            return true;
        }

        //check the upper left to lower right diagnol
        i=0;
        while(i<3 &&
              this.boards[i][i].winner &&
              this.boards[i][i].winner.name === player.name
              ) {
            i += 1;
        }
        if(i === 3){
            if(!silent) {
                for(i=0; i<3; i++) {
                    this.boards[i][i].highlight();
                }
            }
            return true;
        }

        //check the uper right to lower left diagnol
        i=0;
        while(i<3 &&
              this.boards[i][2-i].winner &&
              this.boards[i][2-i].winner.name === player.name
              ) {
            i += 1;
        }
        if(i === 3){
            if(!silent) {
                for(i=0; i<3; i++) {
                    this.boards[i][2-i].highlight();
                }
            }
            return true;
        }
        //None of the rows, cols, or diagnols were winning
        return false;
};

var RandomBot = function(){

};

RandomBot.prototype.getRandomValidMove = function(game) {
    var moves = game.getValidMoves();
    var moveNum = floor(random(0, moves.length));
    return moves[moveNum];
};

RandomBot.prototype.playOutHidden = function(game) {
    /**Plays out the rest of the game hidden from the UI
    * and returns the winner.**/
    while (!game.finished) {
        var move = this.getRandomValidMove(game);
        game.playCellSilently(move[0], move[1],
                              move[2], move[3]);
    }
    return game.winner;
};

RandomBot.prototype.play = function(game){
    debug("randomBotplay");
    var move = this.getRandomValidMove(game);
    game.playCell(move[0], move[1],
                  move[2], move[3]);
};

var randomBot = new RandomBot();

var MonteCarloBot = function() {
    this.clones = [];
    RandomBot.call(this);
};
MonteCarloBot.prototype = Object.create(RandomBot.prototype);

MonteCarloBot.prototype.startCalculation = function(game) {
    debug("MonteCarloBotStartCalc");
    var move;
    var validMoves = game.getValidMoves();
    //Create a cloned game with each of the possible next
    //moves.
    this.clones = [];
    var clone;
    this.endThinkTime = millis() + botThinkingTime;
    for (var i=0; i<validMoves.length; i++) {
        move = validMoves[i];
        clone = new Game(game);
        clone.playCellSilently(move[0], move[1],
                               move[2], move[3]);

        //debug(clone);
        this.clones.push({
            boardRow: move[0],
            boardCol: move[1],
            cellRow: move[2],
            cellCol: move[3],
            clone: clone,
            wins: 0,
            weightedWins: 0,
            losses: 0,
            weightedLosses: 0,
            ties: 0
        });
    }
};

MonteCarloBot.prototype.calculateAhead = function(game) {
    debug("MonteCarloCalculateAhead");
    //For each of the possible next moves
    for (var i=0; i< this.clones.length; i++) {
        var move = this.clones[i];
        var simgame = new Game(move.clone);
        var winner = randomBot.playOutHidden(simgame);
        var weight = 1;
        if(monteWeightByGameLength){
            //number of moves it took for sim to finish
            var f = simgame.countFilled - game.countFilled;
            //number of moves remaining in game
            var r = 81 - game.countFilled;
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
        if (winner === null) {
            move.ties += 1;
        } else
        if(winner.name === game.currentPlayer.name){
            move.wins += 1;
            move.weightedWins += 1*weight;
        }  else {
            move.losses +=1;
            move.weightedLosses += 1* weight;
        }
    }
};

MonteCarloBot.prototype.play = function(game){
    debug("MonteCarloBotPlay");
    var bestScore = -10000;
    var bestMove = null;
    for (var i=0; i< this.clones.length; i++) {
        var penalty = 1;
        var clone = this.clones[i];
        if (!clone.clone.currentBoard){
            penalty = monteChoicePenalty;
        }
        var score = (
            clone.weightedWins-
            clone.weightedLosses*penalty)/
            (clone.wins+clone.losses+clone.ties);
        debug(score, [clone.boardRow, clone.boardCol,
            clone.cellRow, clone.cellCol], [clone.wins, clone.ties,
              clone.losses], clone.weightedWins,
              clone.weightedLosses, penalty);
        if(score > bestScore) {
            bestScore = score;
            bestMove = clone;
        }
    }

    debug(bestScore);
    if (!bestMove) {
        debug("no moves!");
        return;
    }
    game.playCell(bestMove.boardRow, bestMove.boardCol,
                  bestMove.cellRow, bestMove.cellCol);
};

var monteCarloBot = new MonteCarloBot();

var PerfectBot = function() {
    RandomBot.call(this);
};

PerfectBot.prototype = Object.create(RandomBot.prototype);

PerfectBot.prototype.play = function(game) {
    debug("PerfectBotPlay");
        var board = game.currentBoard;
        if(game.countFilled === 0) {
            game.playCell(1,1,1,1);
            return;
        } else if (!game.boards[1][1].isFull()) {
            game.playCell(board.row, board.col, 1,1);
            return;
        } else {
            if(!this.sendBoards) {
                this.sendBoards = [board];
            }
            if(board === null) {
                //the previous board is full
                var oldBoard = this.sendBoards[0];
                var newBoard = game.boards
                    [2-oldBoard.row]
                    [2-oldBoard.col];
                this.sendBoards.push(newBoard);
                board = newBoard;
            }
            var nonFullBoards = [];
            var curBoard;
            for(var i = 0; i<this.sendBoards.length; i++){
                curBoard = this.sendBoards[i];
                if(!curBoard.isFull()){
                    nonFullBoards.push(curBoard);
                }
            }
            debug("sendboards", this.sendBoards);
            for(i = 0; i<this.sendBoards.length; i++){
                curBoard = this.sendBoards[i];
                var cell = board.cells[curBoard.row]
                                      [curBoard.col];
                if(!cell.owner) {
                    game.playCell(board.row,
                                  board.col,
                                  curBoard.row,
                                  curBoard.col);
                    return;
                }
            }
        }
        debug("PerfectBot has not handled this case");
        RandomBot.play(game);
};
background(255, 255, 255);

//RandomBot.play(game);
//MonteCarloBot.play(game);

var perfectBot = new PerfectBot();

var game = new Game();

var debugGame = function() {
    debug(new Game(game));
    var stringBoard = '';
    stringBoard += "-------------\n";
    for (var bigRow = 0; bigRow < 3; bigRow++) {
        for (var smallRow = 0; smallRow < 3; smallRow++) {
            stringBoard += "|";
            for (var bigCol = 0; bigCol < 3; bigCol++) {
                for (var smallCol = 0; smallCol < 3; smallCol++) {
                    if (game.boards[bigRow][bigCol].cells[smallRow][smallCol].owner) {
                        stringBoard += game.boards[bigRow][bigCol].cells[smallRow][smallCol].owner.name;
                    } else{
                        stringBoard += " ";
                    }
                }
                stringBoard += "|";
            }
            stringBoard += "\n";
        }
        stringBoard += "-------------\n";
    }
    debug(stringBoard);
    debug(game.moveHistory);
};

var computerMove = function() {
        if ((game.currentPlayer === playerX &&
            xIsComputer === "random") ||
           (game.currentPlayer === playerO &&
           oIsComputer === "random")){
            debugGame();
            randomBot.play(game);
        } else if ((game.currentPlayer === playerX &&
            xIsComputer === "monte") ||
           (game.currentPlayer === playerO &&
           oIsComputer === "monte")){
            debugGame();
            //var clone = new Game(game);
            monteCarloBot.startCalculation(game);
        } else if ((game.currentPlayer === playerX &&
           xIsComputer === "perfect") ||
           (game.currentPlayer === playerO &&
           oIsComputer === "perfect")){
            debugGame();
            debug(perfectBot.play);
            perfectBot.play(game);
        }

};

var mousePressed = function() {
    if ((game.currentPlayer === playerX &&
           xIsComputer === false) ||
           (game.currentPlayer === playerO &&
           oIsComputer === false)){
        debugGame();
        game.playAt(mouseX, mouseY);
        computerMove();
    }
};


if (debugMode) {
var game = new Game();
game.draw();  game.playCell(1,1,2,1);
game.playCell(2,1,0,1);
game.playCell(0,1,2,1);
game.playCell(2,1,2,1);
game.playCell(2,1,1,2);
game.playCell(1,2,2,2);
game.playCell(2,2,1,0);
game.playCell(1,0,1,2);
game.playCell(1,2,0,0);
game.playCell(0,0,2,0);
game.playCell(2,0,0,0);
game.playCell(0,0,0,2);
game.playCell(0,2,1,2);
game.playCell(1,2,1,2);
game.playCell(1,2,0,2);
game.playCell(0,2,2,1);
game.playCell(2,1,1,0);
game.playCell(1,0,0,0);
game.playCell(0,0,1,1);
game.playCell(1,1,1,1);
game.playCell(1,1,0,2);
game.playCell(0,2,0,1);
game.playCell(0,1,2,2);
game.playCell(2,2,2,2);
game.playCell(2,2,0,0);
game.playCell(0,0,0,0);
game.playCell(0,0,1,0);
game.playCell(1,0,2,0);
game.playCell(2,0,1,1);
game.playCell(1,1,1,0);
game.playCell(1,0,0,1);
game.playCell(0,1,1,0);
game.playCell(1,0,2,1);
game.playCell(2,1,1,1);
game.playCell(1,1,1,2);
game.playCell(1,2,2,0);
game.playCell(2,0,2,2);
game.playCell(2,2,0,2);
game.playCell(0,2,1,1);
game.playCell(1,1,2,2);
game.playCell(2,2,1,2);
game.playCell(1,2,0,1);
game.playCell(0,1,1,2);
game.playCell(1,2,1,0);
game.playCell(1,0,1,0);
game.playCell(1,0,2,2);
game.playCell(2,2,2,0);
game.playCell(1,1,0,0);
game.playCell(0,0,0,1);
game.playCell(0,1,0,2);
game.playCell(0,2,0,2);
game.playCell(0,2,1,0);
game.playCell(1,0,0,2);
game.playCell(0,2,0,0);
game.playCell(0,0,2,2);
game.playCell(0,0,1,2);
game.playCell(1,2,2,1);
game.playCell(0,0,2,1);
game.playCell(0,1,2,0);
game.playCell(1,0,1,1);
monteCarloBot.startCalculation(game);
for (var i= 0; i<1; i++) {
  monteCarloBot.calculateAhead(game);
}
monteCarloBot.play(game);

/*
var clone = $.extend(true, {}, game);
clone.playCellSilently(2,1,0,2);
background(255, 255, 255);
clone.draw();
debug(clone);
debug(RandomBot.playOutHidden(clone));
debug(clone);*/
//MonteCarloBot.play(game);
//game.playCell(2,0,0,0);
/*
for(var i = 0; i<1; i++){
    var clone = $.extend(true, {}, game);
    var winner = RandomBot.playOutHidden(clone);
    debug("winner=",winner);
}*/
    /*
var running = false;
background(255, 255, 255);
var testGame = new Game();
testGame.draw();

var montePlaying = false;
var MonteCarloTestPlay = function(){
    MonteCarloBot.play(testGame);
    if(!testGame.finished) {
        RandomBot.play(testGame);
        g.setTimeout(MonteCarloTestPlay,0);
    }
};
var draw = function() {
    if(mouseIsPressed && !running){
        running = true;

        RandomBot.play(testGame);
        g.setTimeout(MonteCarloTestPlay,0);
    }
};*/
}
else{
    game.draw();
    var humanMove = false;
    if (xIsComputer) {
        computerMove();
    }

    var draw = function() {
        if(!game.finished) {
            if ((game.currentPlayer === playerX &&
                xIsComputer === "monte") ||
               (game.currentPlayer === playerO &&
               oIsComputer === "monte")){
                if (millis() > monteCarloBot.endThinkTime ) {
                    monteCarloBot.play(game);
                } else {
                    monteCarloBot.calculateAhead(game);
                }
            }
        } else {
            if(debugHistory) {
               debug(game.moveHistory);
               debugHistory = false;
            }
        }
    };

}









#endif // JAVASCRIPT_H
