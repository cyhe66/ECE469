/*
	Board Class Declaration
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include <limits.h>
#include <unordered_map>
#include <list>
#include <tuple>
#include <string.h>
#define WHITE 1
#define BLACK 2

#define BOARDSIZE 8

#define RESET	"\033[4;0m"
#define fBLACK	"\033[4;30m"
#define fRED	"\033[31m"
#define fGREEN	"\033[4;32m"
#define fYELLOW	"\033[33m"
#define fBLUE	"\033[34m"
#define fWHITE	"\033[4;37m"
#define bGREEN	"\033[4;42m"
#define bWHITE	"\033[47m"

using namespace std;

class Board{
	public:
		Board();	
		Board(Board &b);
		typedef unordered_map<int, list<int>> hashmap;	

		//gameMode option parsing
		int gameMode;
		bool whiteIsHuman;
		bool blackIsHuman;
		int playAs; 
		
		hashmap moves;
		int currentPlayer;
		int score[3];
		int pass[3];
		list<int> flipMoves;	
		int pieceCounter;
		int board[BOARDSIZE][BOARDSIZE];

		list<int> pastMoves;	
		

		//member functions	
		int Print();//print UI
		void PrintSolo();//print only screen 
		bool legalChoice(int y, int x); //bounds of the board
		void LegalMoves(int player);
		pair<int, pair<int, list<int>>> alphaBeta(Board board, int maxDepth, int depth, int alpha, int beta, bool MaxingPlayer, int OGplayer);
		void GenerateViableMoves(int Direction[], int curPlayer, int X, int Y, list<int> flip);
		void applyMove(int mvchoice);
		void applyMoveAI(int key, list<int> flipflop);
		void clear();
		bool TerminalTest();	
		void LoadBoard(string pathname);
		void SaveBoard(string pathname);
		void HumanMove();
		void AIMove(Board &b);		
		//int randomAI();
		pair<int, list<int>> AIv_One();
		int get_board();
		int OGplayer;
	private:
		void PrintEndScreen();	
};



/*
class Player{
	public:
		Player();
		int OGplayer;
	private:
	//	int bigNum = INT_MAX /2;
*/		

class HeuristicEval{//heuristic subclass
	public:
		HeuristicEval();
		static int Heuristic(Board board, int player);	
	
	private:
		static int simpleBoardWeightHeuristic(Board board, int player);	
};
#endif

