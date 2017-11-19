#include <stdio.h>
#include <limits>
#include <string>
#include <numeric>
#include <tuple>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <list>
#include <chrono>
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
	gameCounter = 4;
	AI_timelimit = 5;
}

//copy constructor
Board::Board(Board &b){
	for (int ii = 0; ii < BOARDSIZE; ii++){
		for (int jj = 0; jj < BOARDSIZE; jj++){
			this->board[ii][jj] = b.board[ii][jj];
		}
	} 
	for (int ii = 0; ii < 3; ii++){
		this->score[ii] = b.score[ii];
		this->pass[ii] = b.pass[ii];
	}
	this ->currentPlayer = b.currentPlayer;
	this ->pieceCounter = b.pieceCounter;
}


bool Board::legalChoice(int y, int x){
	return (x >= 0) && (x < BOARDSIZE) && (y >= 0) && (y < BOARDSIZE); 
}
void Board::PrintSolo(){
	cout << "  0 1 2 3 4 5 6 7" << endl;
	cout << " -----------------" << endl;

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
	}
	cout<<fRED "*************************"<<endl;
	cout<<fRED "Move : "<<gameCounter<<endl;
	cout<<fRED "Current Player: " << ((currentPlayer == WHITE) ? "White(1)" : "Black(2)") << endl;
	cout<<fRED "Black Pieces: " << score[BLACK] <<endl;
	cout<<fRED "White Pieces: "<< score[WHITE] <<endl;
	cout<<"*************************"<<RESET<<endl;
}


int Board::Print(){
	//print out the board UI
	int choice = 1;
	PrintSolo();//calls the print function
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

void Board::GenerateViableMoves (int Direction[], int curPlayer, int X, int Y, list<int> flip){
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
		auto search = moves.find(key);
		if (search == moves.end()){//key is not already in the hash function
			pair<int, list<int>> pairing (key, flip);	
			moves.insert(pairing);
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
void Board::LegalMoves(int player){
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
				GenerateViableMoves(Up, player, xcoord, ycoord, flip);
				GenerateViableMoves(Down, player, xcoord, ycoord, flip);
				GenerateViableMoves(Left, player, xcoord, ycoord, flip);
				GenerateViableMoves(Right, player, xcoord, ycoord, flip);
				GenerateViableMoves(UpLeft, player, xcoord, ycoord, flip);
				GenerateViableMoves(UpRight, player, xcoord, ycoord, flip);
				GenerateViableMoves(DownLeft, player, xcoord, ycoord, flip);
				GenerateViableMoves(DownRight, player, xcoord, ycoord, flip);
			}
		}
	}
}
	
void Board::applyMoveAI(int key, list<int> flipflop){
//check for valid input
	//if (!moves.empty()){//while the hash table is not empty
	int oppositePlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
	int ycoord = key /10;
	int xcoord = key %10;
	//cout<< "AI best move is: "<<alphabet[ycoord]<< xcoord << endl;
	board[ycoord][xcoord] = currentPlayer;
	pieceCounter++;
	score[currentPlayer]++;
	//pastMoves.push_back(output);

	//apply all flips
	for (int n : flipflop){	
		int y = n / 10;
		int x = n % 10;
		board[y][x] = currentPlayer;
	}		

	//update score with the flips
	score[currentPlayer] += flipMoves.size();
	score[oppositePlayer] -= flipMoves.size();
// switch the current player, wipe the hash table	
	pass[currentPlayer] = 0;//set the pass player to 0, no passes here!
	gameCounter++;
	currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
	return;
	//}
	/*
	if(moves.empty()){//if the hash table is empty, dont even apply any moves and just skip
		cout << "AI Found no moves, player : " << currentPlayer <<"  skipped. " <<endl;
		pass[currentPlayer] = 1;
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;	
		return;
	}	
	*/
}
void Board::applyMove(int mvchoice){
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
		//cout << " output is " << output<< endl;
		//cout<< "ycoord is "<<ycoord << " and xcoord is : " << xcoord << endl;
		board[ycoord][xcoord] = currentPlayer;
		pieceCounter++;
		score[currentPlayer]++;
		//pastMoves.push_back(output);

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
		gameCounter++;
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
		return;
	}
	if(mvchoice == 0){//if the hash table is empty, dont even apply any moves and just skip
		cout << "No moves, player : " << currentPlayer <<"  skipped. " <<endl;
		pass[currentPlayer] = 1;
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
		gameCounter++;
		return;	
	}	
}

void Board::clear(){
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
	if ( score[BLACK] == score[WHITE]){
		cout << " ######################################### " << endl;
		cout << " ######################################### " << endl;
		cout << " The GameBoard is Tied" << endl;
		cout << " Black has score: " << score[BLACK] <<" and White has score: " << score[WHITE] << endl;
		cout << " ######################################### " << endl;
		cout << " ######################################### " << endl;
	return;
	}
	cout << " ######################################### " << endl;
	cout << " ######################################### " << endl;
	cout << " The " << (score[WHITE] > score[BLACK] ? "White" : "Black") << " Player Wins!" << endl;
	cout << " Winner: " << (score[WHITE] > score[BLACK] ? "White" : "Black");
	cout << " with score: " << (score[WHITE] > score[BLACK] ? score[WHITE] : score[BLACK]) << endl; 
	cout << " The " << (score[WHITE] > score[BLACK] ? "Black" : "White") << " Player loses " << endl;
	cout << " with score: " << (score[WHITE] > score[BLACK] ? score[BLACK] : score[WHITE]) << endl; 
	cout << " ######################################### " << endl;
	cout << " ######################################### " << endl;
}

bool Board::TerminalTest(){
	if (score[BLACK] == 0 || score [WHITE] == 0){// one of the players has no pieces 
		cout << "PLAYER DOMINATION: Game is Over!"<<endl;
		//PRINT FINAL BOARD
		cout << "  0 1 2 3 4 5 6 7" << endl;
		cout << " -----------------" << endl;
		int choice = 1;
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


int Board::get_board(){
	return board[BOARDSIZE][BOARDSIZE];
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
	gameCounter = 0;
	for( int ii = 0; ii < BOARDSIZE; ii++){
		for (int jj = 0; jj < BOARDSIZE; jj++){
			board[ii][jj] = b_info[ii*8+jj] - 48;//some weird ascii arithmetic
			if(b_info[ii*8+jj] - 48 == WHITE){
				score[WHITE]++;		//recalculate scores
				gameCounter++;
			}
			if(b_info[ii*8+jj] - 48 == BLACK){
				score[BLACK]++;		
				gameCounter++;
			}
		}
	}
	currentPlayer = b_info[64] - 48;
	pass[BLACK] = b_info[65] - 48; 
	pass[WHITE] = b_info[66] - 48;
	gameCounter += pass[BLACK] + pass[WHITE];
}
/*********************************************************************************************8
*HUMAN MOVE LOGIC AND AI MOVE LOGIC
***********************************************************************************************/

void Board::HumanMove(){ 
	int mvchoice;
	string userInput;
	clear();
	LegalMoves(currentPlayer);
    int choice = Print();
    if ( choice == 0) {
        cout << "No Valid Moves. Turn Skipped! Please press 0" <<endl;

        cin >> mvchoice; 
        while (mvchoice != 0){
            cout << "No Valid Moves. Turn Skipped! Please press 0" <<endl;
            cin >> mvchoice;
        }
        if (mvchoice == 0){
            applyMove(mvchoice);
            clear();
        }
        return;
    }
    cout <<"Which move do you wish to pick? Pick (1-"<<choice<<")";
    cout <<", or pick (99) to save the game to a File." << endl;
    cout <<"User picks: ";
    cout <<"-----------------------------------------------------"<<endl;
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
    applyMove(mvchoice); 
}

chrono::time_point<std::chrono::system_clock> tic() { 
	return chrono::system_clock::now();
}

chrono::duration<double> toc(chrono::time_point<std::chrono::system_clock> start) {
	chrono::time_point<std::chrono::system_clock> end = chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	return elapsed_seconds;
}




void Board::AIMove(Board &boardgame){
	chrono::time_point<std::chrono::system_clock> start = tic();//start the timer
	
	PrintSolo();//just print the State of the Board
	int ai_mvchoice;
	clear();// clear the board hash table
	flipMoves.clear();
	Board child = boardgame;//make a copy of the board
	int bigNum = 1000000;	
	pair<int,list<int>> moveToMake;
	pair<int,list<int>> tempmoveToMake;
	int alpha = -bigNum;
	int beta = bigNum;
	OGplayer = currentPlayer;
	int depth;
	cout<<"Searched to depth: ";
	for (depth = 1; depth < 64-boardgame.gameCounter; depth++){//assuming depth 50 is never achievable
		int bestVal = -bigNum;
		clear();
		flipMoves.clear();		
		pair<int, pair<int,list<int>>> moveScore = alphaBeta(child, depth, depth, alpha, beta, true, OGplayer, start);
		if( moveScore.first > bestVal && moveScore.first != (numeric_limits<int>::max() -1) && moveScore.first != numeric_limits<int>::min() + 1) {	
			bestVal = moveScore.first; // of type int -- is the weighted value
			tempmoveToMake = moveScore.second;// of type pair<int, list<int>>
		}
		if(moveScore.first != (numeric_limits<int>::max() -1) && moveScore.first != (numeric_limits<int>::min() + 1)){
			moveToMake = tempmoveToMake;	
		}
		else{
			break;
		}	
	}
	cout<<depth<<endl;	
	child.flipMoves = moveToMake.second;
	
	if (child.flipMoves.size() != 0){	
		//apply the result gotten from AIv_One();
		int oppositePlayer = (currentPlayer == WHITE) ? BLACK : WHITE;				
		int ykey = moveToMake.first / 10;
		int xkey = moveToMake.first % 10; 
		cout << "AI Makes Move:	 "<<alphabet[ykey] << xkey<<endl;
		boardgame.board[ykey][xkey] = currentPlayer;
		pieceCounter++;
		score[currentPlayer]++;


		//apply all flips
		for (int n : child.flipMoves){
			int y = n/10;
			int x = n%10;
			boardgame.board[y][x] = currentPlayer;
		}
		score[currentPlayer] += child.flipMoves.size();
		score[oppositePlayer] -= child.flipMoves.size();
		pass[child.currentPlayer] = 0;
		gameCounter++;
		currentPlayer = oppositePlayer;
		chrono::duration<double> elapsed_seconds = toc(start);
		cout<<"AI took: " <<elapsed_seconds.count() <<" seconds to make a move."<<endl;
		cout<<endl;
		cout<<endl;
		return;
	}
	if (flipMoves.size() == 0){
		cout << " The AI has no moves to make: "<< currentPlayer <<" skipped. "<< endl;
		pass[currentPlayer] = 1;
		gameCounter++;
		currentPlayer = (currentPlayer = WHITE) ? BLACK : WHITE;
		chrono::duration<double> elapsed_seconds = toc(start);
		cout<<"AI took: " <<elapsed_seconds.count() <<" seconds to make a move."<<endl;
		cout<<endl;
		cout<<endl;
		return;
	}
}
/*************************************************************************
*RANDOM AI TEST CASE
*************************************************************************	

int Board::randomAI(){
	if (moves.empty()){
		return 0;
	}
	else{
	int randomchoice = rand() % moves.size()+1;;//+ 1;// 0 - xx elements		
	//cout<< "the randomchoice is: " << randomchoice << " out of " << moves.size()<<" moves" <<endl;
	return randomchoice;
	}
}
***********************************************************************
*COMPUTER MOVE
***********************************************************************
pair<int,list<int>> Board::AIv_One(){
	for (int d = 1; d < 4; d++) {
		int bestVal = -bigNum;
		cout <<"Searching at depth: " <<d<<endl;	
		pair<int, pair<int,list<int>>> moveScore = alphaBeta(child, d, d, alpha, beta, true);
		bestVal = moveScore.first; // of type int -- is the weighted value
		moveToMake = moveScore.second;// of type pair<int, list<int>>

	if (moves.size() == 1){ //there is only one move to make for the computer
		for ( auto n : moves){
			n = moveToMake;	
		cout<< " There is only one move to make." << endl;
		return moveToMake;
	}

	}
	return moveToMake; // returns a key(move to make) and a list of moves to flip
}
*/

/***************************************************************************
* HEURISTIC STUFF
***************************************************************************/


int HeuristicEval::Heuristic(Board board, int Player){
	int B = simpleBoardWeightHeuristic(board, Player);
	int P = ParityCalc(board, Player);
	int t = terminalStateCheck(board, Player);
	int C = Corners(board, Player);
	
	return B + 500*P + 1000*t + 700*C; 
} 		


int HeuristicEval::simpleBoardWeightHeuristic(Board board, int Player){
	int score = 0;
	int playerCount = 0;
	vector<int> BoardWeight = {	 4, -3,  2,  2,  2,  2, -3,  4,
								-3, -5, -1, -1, -1, -1, -4, -3,
								 2, -1,  1,  0,  0,  1, -1,  2,
								 2, -1,  0,  1,  1,  0, -1,  2,
								 2, -1,  0,  1,  1,  0, -1,  2,
								 2, -1,  1,  0,  0,  1, -1,  2,
								-3, -4, -1, -1, -1, -1, -4, -3,
								 4, -3,  2,  2,  2,  2, -3,  4,
						      };
	//if you reach the corner, then its quadrant is no longer worth much
	if (board.board[0][0] != 0){
		BoardWeight[1] = 0;
		BoardWeight[2] = 0;
		BoardWeight[3] = 0;
		BoardWeight[8] = 0;
		BoardWeight[9] = 0;
		BoardWeight[10] = 0;
		BoardWeight[11] = 0;
		BoardWeight[16] = 0;
		BoardWeight[17] = 0;
		BoardWeight[18] = 0;
		BoardWeight[24] = 0;
		BoardWeight[25] = 0;
	}

	if (board.board[0][7]){
		BoardWeight[4] = 0;
		BoardWeight[5] = 0;
		BoardWeight[6] = 0;
		BoardWeight[12] = 0;
		BoardWeight[13] = 0;
		BoardWeight[14] = 0;
		BoardWeight[15] = 0;
		BoardWeight[21] = 0;
		BoardWeight[22] = 0;
		BoardWeight[23] = 0;
		BoardWeight[30] = 0;
		BoardWeight[31] = 0;
	}

	if (board.board[7][0] != 0){
		BoardWeight[32] = 0;
		BoardWeight[33] = 0;
		BoardWeight[40] = 0;
		BoardWeight[41] = 0;
		BoardWeight[42] = 0;
		BoardWeight[48] = 0;
		BoardWeight[49] = 0;
		BoardWeight[50] = 0;
		BoardWeight[51] = 0;
		BoardWeight[57] = 0;
		BoardWeight[58] = 0;
		BoardWeight[59] = 0;
	}

	if (board.board[8][8] != 0){
		BoardWeight[39] = 0;
		BoardWeight[38] = 0;
		BoardWeight[45] = 0;
		BoardWeight[46] = 0;
		BoardWeight[47] = 0;
		BoardWeight[52] = 0;
		BoardWeight[53] = 0;
		BoardWeight[54] = 0;
		BoardWeight[55] = 0;
		BoardWeight[60] = 0;
		BoardWeight[61] = 0;
		BoardWeight[62] = 0;
	}

	for( int ii = 0; ii < BOARDSIZE; ii++){
		for (int jj = 0; jj < BOARDSIZE; jj++){
			if (board.board[ii][jj] == Player){
				score += 1000*(BoardWeight[ii*8+jj]) +rand() % 10;// making it such that a board with more pieces is worth more than a board with less pieces?? greedy
				playerCount++;
			} 		
		}
	}
	return score + 300*((2*playerCount)/board.gameCounter);
}

int HeuristicEval::ParityCalc(Board board, int player){	//return the parity score multiplied by a constant
	int parity;
	if (board.gameCounter%2 == 0){//if the game is on an even move, whoever is playing has parity advantage
		if(board.currentPlayer == player){
			parity = 1;
		}
		else{
			parity = -1;
		}
	}
	else{						//if the game is on an odd move, then the other player has parity advantage
		if(board.currentPlayer == player){
			parity = -1;
		}
		else{
			parity = 1;
		}
	}
	return parity;
}

int HeuristicEval::terminalStateCheck(Board board, int player){// HIGH PRIORITY
	int otherPlayer = (player == WHITE) ? BLACK : WHITE;
	if (board.moves.size() == 0 || board.pieceCounter == 64){
		if ( board.score[WHITE] > board.score[BLACK]){
			if ( player == WHITE){
				return 1;
			}
			else{//player is BLACK
				return -1;
			}
		}
		else if (board.score[BLACK] > board.score[WHITE]){
			if (player ==BLACK){
				return 1;
			}	
			else{
				return -1; //do not want to lose
			}
		}
		else{//tie
			return -1;	//do not want a tie
		}	
	}
	else {	// not an end game state
		return 0;
	}
}
			
int HeuristicEval::Corners(Board board, int player){ //corner control is good to have
	int oplayer = (player == WHITE) ? BLACK : WHITE;
	int pCorners = 0;
	int oCorners = 0;
	//topleft 
	if (board.board[0][0] = player){
		pCorners++;
	}	
	if (board.board[0][0] = oplayer){
		oCorners++;
	}
	//topright	
	if (board.board[0][7] = player){
		pCorners++;
	}	
	if (board.board[0][7] = oplayer){
		oCorners++;
	}
	//botleft
	if (board.board[7][0] = player){
		pCorners++;
	}	
	if (board.board[7][0] = oplayer){
		oCorners++;
	}
	//botright
	if (board.board[7][7] = player){
		pCorners++;
	}	
	if (board.board[7][7] = oplayer){
		oCorners++;
	}


	if (pCorners > oCorners){
		return 100* (pCorners - oCorners)/(pCorners+oCorners);
	}
	else if (oCorners > pCorners){
		return -100* (pCorners - oCorners)/(pCorners+oCorners);
	}
	else{
		return 0;
	}
}







/**********************************************************************
*MINIMAX WITH ALPHABETA PRUNING
***********************************************************************/
	//returns an int which is the key, as well a hash table set
	
pair<int, pair<int, list<int>>> Board::alphaBeta (Board board, int maxDepth, int currentDepth, int alpha, int beta, bool MaxingPlayer, int OGplayer, chrono::time_point<std::chrono::system_clock> start){
	pair<int,pair<int, list<int>>> moveScore;
	chrono::duration<double> elapsed_seconds = toc(start);

	if(elapsed_seconds.count() > 0.995 * AI_timelimit){
		if (!MaxingPlayer){
			moveScore.first = numeric_limits<int>::max() - 1;
			return moveScore;
		}
		else {
			moveScore.first = numeric_limits<int>::min() + 1;
			return moveScore;
		}
	}
	int bestValue;
	int currentPlayer;
	int bigNum = 1000000;
	
	if (currentDepth < 1){
		bestValue = HeuristicEval::Heuristic(board, OGplayer);
		moveScore.first = bestValue;	
		return moveScore;
	}

	if(MaxingPlayer){//keep a copy of the current player making a move
		currentPlayer = OGplayer;
	}
	else{
		currentPlayer = (OGplayer == BLACK) ? WHITE : BLACK;
	}
  
	moves.clear();
	
	Board child = board; 

	child.LegalMoves(child.currentPlayer);// generates the legal moves for the board inside this function and populates the moves hashmap
	
	if (child.moves.empty()){//if there is a no move sequenct
		moves.clear();
		if(MaxingPlayer){
			child.currentPlayer = (child.OGplayer == BLACK) ? WHITE : BLACK;
			child.LegalMoves(child.currentPlayer);
		}
		else{
			child.LegalMoves(child.currentPlayer);
		}
		if (child.moves.size() > 0){
			moveScore = alphaBeta(child, maxDepth, currentDepth -1, alpha, beta, !MaxingPlayer, OGplayer, start);
			return moveScore;
		}
		else{
			bestValue = HeuristicEval::Heuristic(board,OGplayer);
			moveScore.first = bestValue;
			return moveScore;
		}
	}

	pair<int,pair<int, list<int>>> tempmoveScore;

	if (MaxingPlayer){
		moveScore.first = -bigNum;
		
		for (auto kv : child.moves){
			Board child = board;//create a scratch board
			int key = kv.first;
			list<int> flipflop = kv.second;//TODO add valid input check//no moves
			child.applyMoveAI(key, flipflop);
			
			tempmoveScore = alphaBeta(child, maxDepth, currentDepth -1, alpha, beta, false, OGplayer, start);
			if (tempmoveScore.first > moveScore.first) {
				moveScore.first = tempmoveScore.first;
				moveScore.second = kv;
			}	
			alpha = max(alpha, moveScore.first);
			if (beta <= alpha){ //alpha beta pruning
				break;
			}
		}
		return moveScore;
	}	
	else{ //if not maxing player
		moveScore.first = bigNum;

		for (auto kv : child.moves){	
			Board child = board; //creating a scratch board
			int key = kv.first;
			list<int> flipflop = kv.second;
			child.applyMoveAI(key,flipflop);
			
			tempmoveScore = alphaBeta(child, maxDepth, currentDepth -1, alpha, beta, true, OGplayer, start);
			if (tempmoveScore.first < moveScore.first) {
				moveScore.first = tempmoveScore.first;
				moveScore.second = kv;
			}				
			beta = min(beta, moveScore.first);
			if (beta <= alpha){ //alpha beta pruning
				break;
			}
		}
		return moveScore;
	}
}
			
