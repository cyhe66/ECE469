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
		int Print();
		void PrintSolo();
		list<int> pastMoves;	
		list<int> flipMoves;	
		int pieceCounter;
		
		bool legalChoice(int y, int x);
		void LegalMoves(int player);
		void GenerateViableMoves(int Direction[], int curPlayer, int X, int Y, list<int> flip);
		void applyMove(int mvchoice);
		void applyMoveAI(int key);
		void clear();
		bool TerminalTest();	
		void LoadBoard(string pathname);
		void SaveBoard(string pathname);
		void HumanMove();
		void AIMove();		
		int randomAI();
		pair<int, list<int>> AIv_One();
		int get_board();
		pair<int, pair<int, list<int>>> alphaBeta(int tempBoard[BOARDSIZE][BOARDSIZE], int maxDepth, int depth, int alpha, int beta, bool MaxingPlayer);
		int OGplayer;
	private:
		void PrintEndScreen();	
		int board[BOARDSIZE][BOARDSIZE];
		//bool playerPassed;
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
		static int Heuristic(int board[][BOARDSIZE], int player);	
	
	private:
		static int simpleBoardWeightHeuristic(int board [][BOARDSIZE], int player);	
};
#endif

