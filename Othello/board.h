/*
	Board Class Declaration
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
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

		//gameMode optino parsing
		int gameMode;
		bool whiteIsHuman;
		bool blackIsHuman;
		int playAs; 

		hashmap moves;
		int currentPlayer;
		int score[3];
		int pass[3];
		int Print(hashmap &moves);
		bool legalChoice(int y, int x);
		void LegalMoves(int player, hashmap &moves);
		void GenerateViableMoves(int Direction[], int curPlayer, int X, int Y, list<int> flip, hashmap &flippablePieces);
		void applyMove(int mvchoice, hashmap &moves);
		void clear(hashmap &moves);
		list<int> pastMoves;	
		int pieceCounter;
		list<int> flipMoves;	
		bool TerminalTest(hashmap &moves);	
		void LoadBoard(string pathname);
		void SaveBoard(string pathname);
		void HumanMove();
		void AIMove();		
		int randomAI(hashmap &moves);
	private:
		int board[BOARDSIZE][BOARDSIZE];
		void PrintEndScreen();	
		//bool playerPassed;
};
/*
class Player{
	public:
	Player();
		
	int BlacktPlayer;
	int WhitePlayer;

}

*/
#endif

