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

typedef struct {
	int x;
	int y;
}Piece;

class Board{
	public:
		Board();

		typedef unordered_map<int, list<int>> hashmap;	
		hashmap moves;
		int currentPlayer;
		int score[3];
		void Print(hashmap &moves);
		bool legalChoice(int y, int x);
		void LegalMoves(int player, hashmap &moves);
		void GenerateViableMoves(int Direction[], int curPlayer, int X, int Y, list<int> flip, hashmap &flippablePieces);
		void applyMove(int mvchoice, hashmap &moves);

		list<int> pastMoves;	
		int pieceCounter;
		list<int> flipMoves;	
//		void Print(unordered_map<int, list<int>> moves, int player);
		//void LegalMoves(int player, unordered_map<int, list<int>> &moves);
		//bool NextPlayer(bool currentPlayer);

		//int currentPlayer;
		//list<int> pastMoves;//list of moves in a game;
		//int pieceCounter;
		
	private:
		int board[BOARDSIZE][BOARDSIZE];
		
		//bool playerPassed;
		//int score[3];
};


#endif

