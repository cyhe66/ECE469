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
	int mvchoice;
	while(true){
		b.LegalMoves(b.currentPlayer, b.moves);	
		int choice = b.Print(b.moves);
		cout <<"Which move do you wish to pick? Pick (1-"<<choice<<")"<<endl;
		cin >> mvchoice;	// int 1,2,3,4,...etc. find in tuples list availableMoves
		while (mvchoice < 0 || mvchoice > choice){
			cout<<"Not a valid move. Which move do you wish to pick? Pick (1-"<<choice<<")"<<endl;	
			cin >> mvchoice;	
		}
		b.applyMove(mvchoice, b.moves);
		b.clear(b.moves);
}	

//i want to match mvchoice with a tuple value,


}
