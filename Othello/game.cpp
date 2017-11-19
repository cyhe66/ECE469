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
	//string userInput; 
	string loadInput;
	int saveGame;
	//int mvchoice;
	int playAs;
	int set_time;
	int time_to_set;	
	
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
		b.blackIsHuman = (playAs == 0) ? true : false;
	}
	else {	//b.gameMode == 2A
		b.whiteIsHuman = false;
		b.blackIsHuman = false;
	}
	/*********************************************************
	Set AI TIME LIMIT
	*********************************************************/
		cout<<"Do you want to set the time limit for the AI? Default 5 seconds. Enter 1 to specify or 0 to continue - "<<endl;
		cin>> set_time;	

		while (set_time < 0 || set_time > 1 || cin.fail()){
			cout<< "Input not recognized. Do you want to set the time limit for the AI? Enter 1 to specify or 0 to continue -"<<endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> set_time;
		}
		if (set_time == 1){
			cout<<" How many seconds do you want the A.I. to have to make a move? "<<endl;
			cin >>	time_to_set; 
			while (set_time < 0 || set_time > 60 || cin.fail()){
				cout<< "Input not recognized. Do you wish to play as Black or White? Pick 0 for Black and 1 for White:"<<endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin >> time_to_set;
			}
			b.AI_timelimit = time_to_set;
		}


	/*****************************************************
	Option to LOAD in a saved gamestate
	*****************************************************/
	cout <<"Do you wish to load in a saved game state? Enter 1 to load a saved game, else press 0: "<< endl;

	cin >> saveGame;
	while (saveGame < 0 || saveGame > 1 || cin.fail()){
		cout<< "Input not recognized. Do you wish to load in a saved game state? Enter 0 to continue, or 1 to load a saved game:"<<endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> saveGame;
	}
	if (saveGame == 1){
		cout<< " Enter the name of the file which you want to load the board from: ";
		cin >> loadInput;	
		b.LoadBoard(loadInput);
	}

	/*****************************************************
	GAME LOOP
	*****************************************************/	

	while(!b.TerminalTest()){
		if (b.currentPlayer == BLACK){
			if(b.blackIsHuman){
				b.HumanMove();
			}
			else{
				b.AIMove(b);
			}
		}
		else if (b.currentPlayer == WHITE){
			if(b.whiteIsHuman){
				b.HumanMove();
			}
			else{
				b.AIMove(b);
			}
		}
	}
}

