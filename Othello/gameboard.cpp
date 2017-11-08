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

void Board::Print(unordered_map<int, list<int>> &moves){
	cout<<"after value"<< board[4][5]<<endl;;	

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
	for (auto mv : moves){
		int piece = mv.first;	//piece == 34
		int ii = piece / 10;// extract "3"
		int jj = piece % 10;// extract "4"
		cout<< "Possible Move "<< choice<< ": "<< alphabet[ii] << jj<<"\t ";
		cout<< " Pieces to be flipped: ";
		list<int> l = mv.second;

		for (auto k : l){ //25
			int i = k / 10;//2
			int j = k % 10;//5  
			cout << alphabet[i] << j << "\t";
		}
		cout << endl;
		choice++;
	}
}
/*
int Board::movePicker(int &mvchoice, unordered_map<int, list<int>> &moves){
//move picker takes the availableMoves list, returns the move picked, and clears the available moves list
	int output;
	int count = 1;
	for (auto mv : moves){
		if (count == mvchoice){
			output = mv.first;	
			flipMoves = mv.second;
		}
		count ++;
	}
	return output;
}
*/
/*
bool Board::Terminal_test (bool currentPlayerPass){
	if((playerPassed && currentPlayerPass) || (score[BLACK] + score{WHITE} == NUMSQUARES) || score[BLACK] == 0 || score[WHITE] == 0){
		return true;
	}
	return false;
}*/

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
	if (board[Y+Direction[1]][X+Direction[0]] == 0 && firstpce == true){
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
/*bool Board::NextPlayer(bool currentPlayer){ //changes player
	if (TerminalState(currentPlayer))
		return true;
	return false;
}
*/

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
	for (auto mv : moves){
		if (count == mvchoice){
			output = mv.first;	
			flipMoves = mv.second;
		}
		count ++;
	}
	int oppositePlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
	cout<< "CurrentPlayer is:"<< currentPlayer<<endl;	

	int ycoord = output /10;
	int xcoord = output %10;

	cout<<"New Piece at : " <<alphabet[ycoord]<<xcoord<< endl;
	cout<< "prev value"<<board[ycoord][xcoord]<<endl;;	
	board[ycoord][xcoord] = currentPlayer;
	cout<<"after value"<< board[ycoord][xcoord]<<endl;;	
	cout<<"after value"<< board[4][5]<<endl;;	


	pieceCounter++;
	score[currentPlayer]++;
	pastMoves.push_back(output);


	//apply all flips
	for (int n : flipMoves){	
		int y = n / 10;
		int x = n % 10;
		cout<<"n is:"<< n << "and "<<alphabet[y] <<x<<endl;
		board[y][x] = currentPlayer;
	}		

	//update score with the flips
	score[currentPlayer] += moves.size();
	score[oppositePlayer] -= moves.size();
// switch the current player, wipe the hash table	
	currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
//	moves.clear();
}	
