#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include "board.h"
using namespace std;

int main(int argc, char** argv){
	Board b;
	//Player p;
	string userInput; 
	int saveGame;
	int mvchoice;
	int playAs;
	
	
	cout << "Welcome to Othello!" << endl;
	
	/**********************************************************
	Set the Game Mode
	**********************************************************/
	cout << "Choose a Game Mode (0-2): " << endl << " 0 - Player vs. Player " <<endl <<" 1 - Player vs. AI " << endl << " 2 - AI vs AI " <<endl;
	cin >> b.gameMode;
	while (b.gameMode < 0 || b.gameMode > 2 || cin.fail()){
		cout<< "Input not recognized. Please choose a Game Mode (0-2)"<<endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> b.gameMode;
	}
	/*********************************************************
	Set Player Logic (Human and A.I)
	*********************************************************/
	if (b.gameMode == 0){	//player vs. player
		b.whiteIsHuman = true;
		b.blackIsHuman = true;
	}

	else if (b.gameMode == 1){
		cout<< "Do you wish to play as Black or White? Pick 0 for Black and 1 for White:" << endl;
		cin >> playAs;
		while (playAs < 0 || playAs > 1 || cin.fail()){
			cout<< "Input not recognized. Do you wish to play as Black or White? Pick 0 for Black and 1 for White:"<<endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> playAs;
		}
		b.whiteIsHuman = (playAs == 1) ? true : false;
		b.blackIsHuman = (playAs == 2) ? true : false;
	}
	else {	//b.gameMode == 2A
		b.whiteIsHuman = false;
		b.blackIsHuman = false;
	}
	/*****************************************************
	Option to load in a saved gamestate
	*****************************************************/
	cout <<" Do you wish to load in a saved game state? Enter 0 to continue, or 1 to load a saved game: ";
	cin >> saveGame;
	while (saveGame < 0 || saveGame > 1 || cin.fail()){
		cout<< "Input not recognized. Do you wish to play as Black or White? Pick 0 for Black and 1 for White:"<<endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> saveGame;
	}
		





	/*****************************************************
	GAME LOOP
	*****************************************************/	

	while(!b.TerminalTest(b.moves)){
		b.clear(b.moves);
		b.LegalMoves(b.currentPlayer, b.moves);	
		/*********************************************************************
		for HUMAN CONSUMPTION
		**********************************************************************/
		int choice = b.Print(b.moves);
		if ( choice == 0) {
			cout << "No Valid Moves. Turn Skipped! Please press 0" <<endl;

			cin >> mvchoice; 
			while (mvchoice != 0){
				cout << "No Valid Moves. Turn Skipped! Please press 0" <<endl;
				cin >> mvchoice;
			}
			if (mvchoice == 0){
				b.applyMove(mvchoice, b.moves);
				b.clear(b.moves);
			}
			continue;
		}
		cout <<"Which move do you wish to pick? Pick (1-"<<choice<<")";
		cout <<", or pick (99) to save the game to a File." << endl;
		cout <<"-----------------------------------------------------"<<endl;
		cout <<"User picks: ";
		cin >> mvchoice;
		if (mvchoice == 99){//user wants to save a game
			cout << " Enter the filepath at which you want to store the board: ";
			cin >> userInput;
			b.SaveBoard(userInput);
			continue;
		}
		while (mvchoice <= 0 || mvchoice > choice || cin.fail()){
			cout<<"Input is not a valid move. Which move do you wish to pick? Pick (1-"<<choice<<")"<<endl;	
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> mvchoice;	
			if (mvchoice == 99){//user wants to save a game
				cout << " Enter the filepath at which you want to store the board: ";
				cin >> userInput;
				b.SaveBoard(userInput);
				continue;
			}
		}
		b.applyMove(mvchoice, b.moves);	
	}
}

