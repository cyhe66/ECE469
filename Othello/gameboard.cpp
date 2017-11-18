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
}


int Board::Print(){
	//print out the board UI
	int choice = 1;
	PrintSolo();//calls the print function
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
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
		return;
	}
	if(mvchoice == 0){//if the hash table is empty, dont even apply any moves and just skip
		cout << "No moves, player : " << currentPlayer <<"  skipped. " <<endl;
		pass[currentPlayer] = 1;
		currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
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

void Board::AIMove(Board &boardgame){
	PrintSolo();//just print the State of the Board
	int ai_mvchoice;
	clear();// clear the board hash table
	flipMoves.clear();
	Board child = boardgame;//make a copy of the board
	int bigNum = 1000000;	
	pair<int,list<int>> moveToMake;
	int alpha = -bigNum;
	int beta = bigNum;
	OGplayer = currentPlayer;

	for ( int depth = 1; depth < 3; depth++){
		int bestVal = -bigNum;
		cout <<"Searching at depth: " <<depth<<endl;	
		clear();
		flipMoves.clear();		
		pair<int, pair<int,list<int>>> moveScore = alphaBeta(child, depth, depth, alpha, beta, true, OGplayer);
		if( moveScore.first > bestVal){	
			bestVal = moveScore.first; // of type int -- is the weighted value
			moveToMake = moveScore.second;// of type pair<int, list<int>>
		}
		cout << "bestVal: "<<bestVal;
		child.flipMoves = moveToMake.second;
		cout<<endl;
	}
	if (child.flipMoves.size() != 0){	
		//apply the result gotten from AIv_One();
		int oppositePlayer = (currentPlayer == WHITE) ? BLACK : WHITE;				
		int ykey = moveToMake.first / 10;
		int xkey = moveToMake.first % 10; 
		cout << "New Piece:	 "<<alphabet[ykey] << xkey<<endl;
		boardgame.board[ykey][xkey] = currentPlayer;
		pieceCounter++;
		score[currentPlayer]++;


		//apply all flips
		cout<<"flipmoves = ";
		for (int n : child.flipMoves){
			int y = n/10;
			int x = n%10;
			cout <<alphabet[y]<<x;
			boardgame.board[y][x] = currentPlayer;
		}
		cout<<endl;
		score[currentPlayer] += child.flipMoves.size();
		score[oppositePlayer] -= child.flipMoves.size();
		pass[child.currentPlayer] = 0;
		currentPlayer = oppositePlayer;
		return;
	}
	if (flipMoves.size() == 0){
		cout << " The AI has no moves to make: "<< currentPlayer <<" skipped. "<< endl;
		pass[currentPlayer] = 1;
		currentPlayer = (currentPlayer = WHITE) ? BLACK : WHITE;
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
	return	simpleBoardWeightHeuristic(board, Player);
} 		


int HeuristicEval::simpleBoardWeightHeuristic(Board board, int Player){
	int score = 0;
	vector<int> BoardWeight = {	 40, -3,  2,  2,  2,  2, -3,  40,
								-3, -4, -1, -1, -1, -1, -4, -3,
								 2, -1,  1,  0,  0,  1, -1,  2,
								 2, -1,  0,  1,  1,  0, -1,  2,
								 2, -1,  0,  1,  1,  0, -1,  2,
								 2, -1,  1,  0,  0,  1, -1,  2,
								-3, -4, -1, -1, -1, -1, -4, -3,
								 40, -3,  2,  2,  2,  2, -3,  40,
						      };
	//if you reach the corner, then the quadrant is no longer worth much
	/*
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
	*/	

	for( int ii = 0; ii < BOARDSIZE; ii++){
		for (int jj = 0; jj < BOARDSIZE; jj++){
			if (board.board[ii][jj] == Player){
				score += 1000*(BoardWeight[ii*8+jj]) +rand() % 10;// making it such that a board with more pieces is worth more than a board with less pieces?? greedy
			} 		
		}
	}
	return score;
}

/**********************************************************************
*MINIMAX WITH ALPHABETA PRUNING
***********************************************************************/
	//returns an int which is the key, as well a hash table set
	
pair<int, pair<int, list<int>>> Board::alphaBeta (Board board, int maxDepth, int currentDepth, int alpha, int beta, bool MaxingPlayer, int OGplayer){
	pair<int,pair<int, list<int>>> moveScore;
	//board.clear();// clear the board hash table
	//board.flipMoves.clear();
	int bestValue;
	int currentPlayer;
	int bigNum = 1000000;
	
	if (currentDepth < 1){
		bestValue = HeuristicEval::Heuristic(board, OGplayer);
		moveScore.first = bestValue;	
		cout<<"bestvalue = "<<bestValue<<endl; //" for move: " <<alphabet[moveScore.second.first/10]<<moveScore.second.first%10<<endl;
		return moveScore;
	}

	if(MaxingPlayer){//keep a copy of the current player making a move
		currentPlayer = OGplayer;
	}
	else{
		currentPlayer = (OGplayer == BLACK) ? WHITE : BLACK;
	}
	LegalMoves(currentPlayer);
	for (auto kv : moves){//just want to print out the legal moves
		cout<<"Legal move = ";
		list<int> flipp = kv.second;
		cout<< alphabet[kv.first/10]<<kv.first%10<< " ";
		for (auto mv : flipp){
			cout<<" -"<<alphabet[mv/10]<<mv%10;
		}
	cout<< endl;
	}	
/*
	Board child = board; //copy the board??

	child.LegalMoves(child.currentPlayer);// generates the legal moves for the board inside this function and populates the moves hashmap
	//print the legal moves for child 
		for (auto kv : child.moves){
			int key = kv.first;
			list<int> flipp = kv.second;
			cout<<"Legal move = "<< alphabet[key/10]<<key%10;
			for (auto mv : flipp){
				cout<<" -"<< alphabet[mv/10]<<mv%10<<endl;
			}
		}
*/
	pair<int,pair<int, list<int>>> tempmoveScore;

	if (MaxingPlayer){
		cout<<"I am maxing"<<endl;
		moveScore.first = -bigNum;
		
		for (auto kv : moves){
			Board child = board;//create a scratch board
			cout<<currentDepth<< "Applying move: "<<alphabet[kv.first/10]<<kv.first%10<<endl;
			int key = kv.first;
			list<int> flipflop = kv.second;//TODO add valid input check//no moves
			child.applyMoveAI(key, flipflop);
			
			tempmoveScore = alphaBeta(child, maxDepth, currentDepth -1, alpha, beta, false, OGplayer);
			if (tempmoveScore.first > moveScore.first) {
				moveScore.first = tempmoveScore.first;
				moveScore.second = kv;
			}	
			cout<<"Alpha is:"<<alpha<<endl;
			alpha = max(alpha, moveScore.first);
			if (beta <= alpha){ //alpha beta pruning
				cout<<"ALPHA PRUNE THAT SHIT!"<<endl;
				break;
			}
		}
		return moveScore;
	}	
	else{ //if not maxing player
		cout<<"I am minning"<<endl;
		moveScore.first = bigNum;

		for (auto kv : moves){	
			Board child = board; //creating a scratch board
			cout<<currentDepth<< "Applying move: "<<alphabet[kv.first/10]<<kv.first%10<<endl;
			int key = kv.first;
			list<int> flipflop = kv.second;
			child.applyMoveAI(key,flipflop);
			
			tempmoveScore = alphaBeta(child, maxDepth, currentDepth -1, alpha, beta, true, OGplayer);
			if (tempmoveScore.first < moveScore.first) {
				moveScore.first = tempmoveScore.first;
				moveScore.second = kv;
			}				
			cout<<"Beta is: "<< beta<< endl;
			beta = min(beta, moveScore.first);
			if (beta <= alpha){ //alpha beta pruning
				cout<<"BETA PRUNE THAT SHIT!"<<endl;
				break;
			}
		}
		return moveScore;
	}
}
			

	
/*
function minimax(node * parent)

if parent depth == maxdepth
	set parent value = to heuristic
	return

if in max
set bool max = true
set parent value to intmax

if in min
set bool max = false
set parent value to intmax

create temp node

for (loop through legal moves)
set board of parent = board of temp
make the move on temp
set all values for temp based on that move
set temp as a child of parent

call minimax(child)

if ( max && parent->value < child -> value)
swap the value of the parent with the child
(do alphabeta check)

if ( !max && parent -> value > child -> value)
do swap
do alpha-beta check

*/	


