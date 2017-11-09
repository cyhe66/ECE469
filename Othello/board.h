/*
	Board Class Declaration
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>

#define WHITE 1
#define BLACK 2

#define BOARDSIZE 8

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"

using namespace std;

class Board{
	public:
		Board();

		typedef unordered_map<int, list<int>> hashmap;	
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
	private:
		int board[BOARDSIZE][BOARDSIZE];
		
		//bool playerPassed;
};


#endif

