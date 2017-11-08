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
		b.Print(b.moves);
		cout <<"Which move do you wish to pick?"<<endl;
		cin >> mvchoice;	// int 1,2,3,4,...etc. find in tuples list availableMoves
		b.applyMove(mvchoice, b.moves);
}	

//i want to match mvchoice with a tuple value,


}
