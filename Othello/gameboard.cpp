#include <stdio.h>
#include <tuple>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include "board.h"

const char alphabet[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

int uniquify(int column, int row);
Board::Board(){
	//initialize the board as zeroes
	memset(board, 0, BOARDSIZE*BOARDSIZE*4);	
	
	//start positions
	board[3][3] = WHITE; // board[y-axis][x-axis]
	board[3][4] = BLACK;
	board[4][3] = BLACK;
	board[4][4] = WHITE;
	//create start signals
	score[BLACK] = 2;
	score[WHITE] = 2;
	currentPlayer = BLACK;
	pieceCounter = 4;
}


bool Board::legalChoice(int y, int x){
	return (x >= 0) && (x < BOARDSIZE) && (y >= 0) && (y < BOARDSIZE); 
}

int Board::Print(unordered_map<int, list<int>> &moves){

	//print out the board UI
	cout << "  01234567" << endl;
	cout << " ---------" << endl;
	int choice = 1;

	for (int ii = 0; ii < BOARDSIZE; ii++){
		cout << alphabet[ii] << "|";
		for (int jj = 0; jj < BOARDSIZE; jj++){
			cout<<board[ii][jj];
		}
		cout << endl;
	}//print user move stuff
	cout << "Current Player: " << ((currentPlayer == WHITE) ? "White(1)" : "Black(2)") << endl;
	cout<< "Black Pieces: " << score[BLACK] <<endl;
	cout<< "White Pieces: "<< score[WHITE] <<endl;
	cout<<"----------------------------------"<<endl;
	for (auto mv : moves){
		int piece = mv.first;	//piece == 34
		int ii = piece / 10;// extract "3"
		int jj = piece % 10;// extract "4"
		cout<< "Possible Move "<< choice<< ": "<< alphabet[ii] << jj<<"\t ";
		cout<< " Pieces to be flipped: ";
		list<int> l = mv.second;

		for (auto k : l){ 
			int i = k / 10;
			int j = k % 10;  
			cout << alphabet[i] << j << " ";
		}
		cout << endl;
		choice++;
	}
	return choice - 1;
}

void Board::GenerateViableMoves (int Direction[], int curPlayer, int X, int Y, list<int> flip, unordered_map<int, list<int>> &flippablePieces){
	int oppoPlayer = (curPlayer == WHITE) ? BLACK : WHITE;
	int key;
	bool firstpce = false;
	
	while (board[Y+Direction[1]][X+Direction[0]] == oppoPlayer){
		firstpce = true;
		flip.push_back(uniquify((Y+Direction[1]),(X + Direction[0])));
		Y = Y + Direction[1];	
		X = X + Direction[0];
		continue;
	}
	if (board[Y+Direction[1]][X+Direction[0]] == 0 && firstpce == true && legalChoice(Y + Direction[1], X + Direction[0])){
		int key = uniquify((Y + Direction[1]), (X + Direction[0]));
		auto search = flippablePieces.find(key);
		if (search == flippablePieces.end()){//key is not already in the hash function
			pair<int, list<int>> pairing (key, flip);	
			flippablePieces.insert(pairing);
			return;
		}
		else{ // key is in the hash function already
			search->second.merge(flip);	
			return;	
		}
	}
	else{
		flip.clear();
		return;
	}
}

int uniquify(int column, int row){
	return (column * 10) + row; //tens digit is the column, ones digit is the row
}
 
//player == either 1 (white) or 2 (black)
void Board::LegalMoves(int player, unordered_map<int, list<int>> &moves){	
	int Up[2] = {0,-1}; //{iterate in x direction, iterate in y direction}
	int Down[2] = {0,1};
	int Left[2] = {-1,0};
	int Right[2] = {1,0};
	int UpLeft[2] = {-1,-1};
	int UpRight[2] = {1,-1};
	int DownLeft[2] = {-1,1};
	int DownRight[2] = { 1,1};

	list<int> flip;	
	
	for (int ii = 0; ii < BOARDSIZE; ii++){
		for (int jj = 0; jj < BOARDSIZE; jj++){
			if (board[ii][jj] == player){
				int xcoord = jj;//save location of the piece in question
				int ycoord = ii;
				//go through 8-Directions
				GenerateViableMoves(Up, player, xcoord, ycoord, flip, moves);
				GenerateViableMoves(Down, player, xcoord, ycoord, flip, moves);
				GenerateViableMoves(Left, player, xcoord, ycoord, flip, moves);
				GenerateViableMoves(Right, player, xcoord, ycoord, flip, moves);
				GenerateViableMoves(UpLeft, player, xcoord, ycoord, flip, moves);
				GenerateViableMoves(UpRight, player, xcoord, ycoord, flip, moves);
				GenerateViableMoves(DownLeft, player, xcoord, ycoord, flip, moves);
				GenerateViableMoves(DownRight, player, xcoord, ycoord, flip, moves);
			}
		}
	}
}
	
void Board::applyMove(int mvchoice, unordered_map<int, list<int>> &moves){
	int output;
	int count = 1;

//check for valid input
	if (!moves.clear()){//while the hash table is not empty
		for (auto mv : moves){
			if (count == mvchoice){
				output = mv.first;	
				flipMoves = mv.second;
			}
			count ++;
		}
		int oppositePlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
		int ycoord = output /10;
		int xcoord = output %10;

		board[ycoord][xcoord] = currentPlayer;
		pieceCounter++;
		score[currentPlayer]++;
		pastMoves.push_back(output);


		//apply all flips
		for (int n : flipMoves){	
			int y = n / 10;
			int x = n % 10;
			board[y][x] = currentPlayer;
		}		

		//update score with the flips
		score[currentPlayer] += flipMoves.size();
		score[oppositePlayer] -= flipMoves.size();
	// switch the current player, wipe the hash table	
		pass[currentPlayer] = 0;
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
		return;
	}
	if(moves.clear()){//if the hash table is empty, done even apply any moves and just skip
		pass[currentPlayer] = 1
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
	
}	

void Board::clear(unordered_map<int, list<int>> &moves){
	moves.clear();	
	return;
}


bool Board::TerminalTest(unordered_map<int, list<int>> &moves){
	if (score[BLACK] == 0 || score [WHITE] == 0){// one of the players has no pieces 
		return true;
	}
	if (score[BLACK] + score[WHITE] == 64){
		return true; 
	}
}
