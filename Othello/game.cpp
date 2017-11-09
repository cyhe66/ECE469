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
	string userInput; 
	int mvchoice;

	while(!b.TerminalTest(b.moves)){
		b.clear(b.moves);
		b.LegalMoves(b.currentPlayer, b.moves);	
		int choice = b.Print(b.moves);
		if ( choice == 0) {
			cout << "No Valid Moves. Turn Skipped! Please press 0" <<endl;

//			getline(cin, userInput);
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
		cout <<"Which move do you wish to pick? Pick (1-"<<choice<<")"<<endl;
		cout <<"-----------------------------------------------------"<<endl;
		cout <<"Picked Move: ";
		cin >> mvchoice;
		while ( mvchoice <= 0 || mvchoice > choice ||cin.fail()){
			cout<<"Input is not a valid move. Which move do you wish to pick? Pick (1-"<<choice<<")"<<endl;	
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> mvchoice;	
		}
		b.applyMove(mvchoice, b.moves);	
	}	
}
