#include <stdio.h>
#include <limits>
#include <string>
#include <assert.h>
#include <numeric>
#include <tuple>
#include <string.h>
#include <iostream>
#include <fstream>
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
	pass[WHITE] = 0;
	pass[BLACK] = 0;
	currentPlayer = BLACK;
	pieceCounter = 4;
}

bool Board::legalChoice(int y, int x){
	return (x >= 0) && (x < BOARDSIZE) && (y >= 0) && (y < BOARDSIZE); 
}

int Board::Print(unordered_map<int, list<int>> &moves){

	//print out the board UI
	cout << "  0 1 2 3 4 5 6 7" << endl;
	cout << " -----------------" << endl;
	int choice = 1;

	for (int ii = 0; ii < BOARDSIZE; ii++){
		cout << alphabet[ii] <<bGREEN << fBLACK << "|";
		for (int jj = 0; jj < BOARDSIZE; jj++){
			if (board[ii][jj] == WHITE){
				cout<< bGREEN << fWHITE << board[ii][jj] << fBLACK <<"|"<< RESET;	
			}
			else if (board[ii][jj] == BLACK){
				cout<< bGREEN << fBLACK << board[ii][jj] << fBLACK <<"|"<< RESET;	
			}
			else {
				cout << bGREEN << fGREEN<< board[ii][jj] << fBLACK <<"|"<< RESET;
			}
		}
		cout << RESET << endl;
	}//print user move stuff
	cout<<fRED "*************************"<<endl;
	cout<<fRED "Current Player: " << ((currentPlayer == WHITE) ? "White(1)" : "Black(2)") << endl;
	cout<<fRED "Black Pieces: " << score[BLACK] <<endl;
	cout<<fRED "White Pieces: "<< score[WHITE] <<endl;
	cout<<"*************************"<<RESET<<endl;
	for (auto mv : moves){
		int piece = mv.first;	//piece == 34
		int ii = piece / 10;// extract "3"
		int jj = piece % 10;// extract "4"
		cout<< "Possible Move "<< choice<< ": "<< fBLUE << alphabet[ii] << jj<<RESET<<"\t ";
		cout<< " Pieces to be flipped: ";
		list<int> l = mv.second;

		for (auto k : l){ 
			int i = k / 10;
			int j = k % 10;  
			cout <<fYELLOW<< alphabet[i] << j << " ";
		}
		cout <<RESET<<  endl;
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
	if (mvchoice != 0){//while the hash table is not empty
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
		pass[currentPlayer] = 0;//set the pass player to 0, no passes here!
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
		return;
	}
	if(mvchoice == 0){//if the hash table is empty, dont even apply any moves and just skip
		cout << "No moves, player : " << currentPlayer <<"  skipped. " <<endl;
		pass[currentPlayer] = 1;
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
	
	}	
}

void Board::clear(unordered_map<int, list<int>> &moves){
	moves.clear();	
	return;
}

void Board::PrintEndScreen(){
	for (int ii = 0; ii < BOARDSIZE; ii++){
		cout << alphabet[ii] << "|";
		for (int jj = 0; jj < BOARDSIZE; jj++){
			if (board[ii][jj] == WHITE){
				cout<< bGREEN << fWHITE << board[ii][jj] << fBLACK <<"|"<< RESET;	
			}
			else if (board[ii][jj] == BLACK){
				cout<< bGREEN << fBLACK << board[ii][jj] << fBLACK <<"|"<< RESET;	
			}
			else {
				cout << bGREEN << fGREEN<< board[ii][jj] << fBLACK <<"|"<< RESET;
			}
		}
		cout << RESET << endl;
	}
	cout << " ######################################### " << endl;
	cout << " ######################################### " << endl;
	cout << " The " << (currentPlayer == BLACK ? "White" : "Black") << " Player Wins!" << endl;
	cout << " Winner: " << (currentPlayer == BLACK ? "White" : "Black");
	cout << " with score: " << (currentPlayer == BLACK ? score[WHITE] : score[BLACK]) << endl; 
	cout << " ######################################### " << endl;
	cout << " ######################################### " << endl;
}

bool Board::TerminalTest(unordered_map<int, list<int>> &moves){
	if (score[BLACK] == 0 || score [WHITE] == 0){// one of the players has no pieces 
		cout << "PLAYER DOMINATION: Game is Over!"<<endl;
		//PRINT FINAL BOARD
		cout << "  0 1 2 3 4 5 6 7" << endl;
		cout << " -----------------" << endl;
		int choice = 1;
		PrintEndScreen();
		return true;
	}
	if (score[BLACK] + score[WHITE] == 64){
		cout << "BOARD FILLED:Game is Over!"<<endl;
		PrintEndScreen();
		return true; 
	}
	if (pass[BLACK] ==1 && pass[WHITE] == 1){
		cout << "Both Players Passed! Game Over"<< endl;
		PrintEndScreen();
		return true;
	}
	else{// not a Terminal State
		return false;
	}
}
/***************************************************************
LOAD AND SAVE BOARD FUNCTIONALITY
****************************************************************/
void Board::SaveBoard(string pathname){
	//0-63 is the board 	
	//64 is the current player
	//65-66 is the pass states
	vector<int> saveboard;

	for (int ii = 0; ii < BOARDSIZE; ii++){ // load the board into saveboard
		for (int jj = 0; jj < BOARDSIZE; jj++){	
			saveboard.push_back(board[ii][jj]);
		}
	}
	saveboard.push_back(currentPlayer);
	saveboard.push_back(pass[BLACK]);
	saveboard.push_back(pass[WHITE]);	
	
	ofstream filestrm;
	filestrm.open(pathname);	
	for (int n : saveboard){
		filestrm << n;
	} 
	cout << endl << "Game Saved." << endl;
}
	
void Board::LoadBoard(string pathname){
	ifstream instream;
	instream.open(pathname);
	string b_info;
	if(!instream)
	{
		cout<< "Error occurred opening file"<<endl;
	}
	else {
		cout << "File successfully opened"<< endl;
	}
	while(!instream.eof()){	
		getline(instream, b_info);	//read information into string
	}
	instream.close();
	//reset the score to zeroes
	score[WHITE] = 0;	
	score[BLACK] = 0;

	for( int ii = 0; ii < BOARDSIZE; ii++){
		for (int jj = 0; jj < BOARDSIZE; jj++){
			board[ii][jj] = b_info[ii*8+jj] - 48;//some weird ascii arithmetic
			if(b_info[ii*8+jj] - 48 == WHITE){
				score[WHITE]++;		//recalculate scores
			}
			if(b_info[ii*8+jj] - 48 == BLACK){
				score[BLACK]++;		
			}
		}
	}
	currentPlayer = b_info[64] - 48;
	pass[BLACK] = b_info[65] - 48; 
	pass[WHITE] = b_info[66] - 48;
}


void Board::HumanMove(){ 
	int mvchoice;
	string userInput;
	clear(moves);
	LegalMoves(currentPlayer, moves);
    int choice = Print(moves);
    if ( choice == 0) {
        cout << "No Valid Moves. Turn Skipped! Please press 0" <<endl;

        cin >> mvchoice; 
        while (mvchoice != 0){
            cout << "No Valid Moves. Turn Skipped! Please press 0" <<endl;
            cin >> mvchoice;
        }
        if (mvchoice == 0){
            applyMove(mvchoice, moves);
            clear(moves);
        }
        return;
    }
    cout <<"Which move do you wish to pick? Pick (1-"<<choice<<")";
    cout <<", or pick (99) to save the game to a File." << endl;
    cout <<"-----------------------------------------------------"<<endl;
    cout <<"User picks: ";
    cin >> mvchoice;
    if (mvchoice == 99){//user wants to save a game
        cout << " Enter the filepath at which you want to store the board: ";
        cin >> userInput;
        SaveBoard(userInput);
        return;
    }
    while (mvchoice <= 0 || mvchoice > choice || cin.fail()){
        cout<<"Input is not a valid move. Which move do you wish to pick? Pick (1-"<<choice<<")"<<endl; 
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> mvchoice;    
        if (mvchoice == 99){//user wants to save a game
            cout << " Enter the filename which you want to store the board: ";
            cin >> userInput;
            SaveBoard(userInput);
            return;
        }
    }
    applyMove(mvchoice, moves); 
}

void Board::AIMove(){




}
