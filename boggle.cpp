/* File: boggle.cpp
 * ----------------
 * This program plays the board game Boggle. 
 * Generally speaking, it uses a Grid container to
 * represent the board internally, while using
 * a graphical display for the user.
 */
 
#include "genlib.h"
#include "simpio.h"
#include <iostream>
#include "extgraph.h"
#include "grid.h"
#include "random.h"
#include "lexicon.h"
#include "gboggle.h"
#include "strutils.h"
#include "set.h"


/* Constants
 * ---------
 */

string StandardCubes[16]  = 
{"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
	"DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};

string BigBoggleCubes[25]  = 
{"AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN", "AFIRSY", 
	"BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT", "DHHLOR", 
	"DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};


/* Structures
 * ----------
 */

/* Struct: locationT
 * ----------------
 * Condenses a row and column into one struct
 */

struct locationT {
	int numRow;
	int numCol;
};

/* Part 1: Instructions
 * -------------------
 */

void GiveInstructions()
{
    cout << endl << "The boggle board is a grid onto which I will randomly distribute " 
	 << "cubes. These 6-sided cubes have letters rather than numbers on the faces, " 
	 << "creating a grid of letters on which you try to form words. You go first, " 
	 << "entering all the words you can find that are formed by tracing adjoining " 
	 << "letters. Two letters adjoin if they are next to each other horizontally, " 
	 << "vertically, or diagonally. A letter can only be used once in the word. Words "
	 << "must be at least 4 letters long and can only be counted once. You score points "
	 << "based on word length: a 4-letter word is worth 1 point, 5-letters earn 2 "
	 << "points, and so on. After your puny brain is exhausted, I, the super computer, "
	 << "will find all the remaining words and double or triple your paltry score." << endl;
	
    cout << "\nHit return when you're ready...";
    GetLine();
}

static void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
	 << "The good news is that you might improve your vocabulary a bit.  The "
	 << "bad news is that you're probably going to lose miserably to this little "
	 << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/* Part 2: Set up board:
 * --------------------
 */


/* Function: InitializeBoard
 * ---------
 * This function initialized the board. It first assigns a random letter from each
 * cube to a spot on the board. It then randomly swaps all the letters so that
 * the same cube is not in the same place every time. Finally, it updates
 * the graphics display.
 */

void InitializeBoard(Grid<string> &board) {
	//assign each location a letter
	for (int i = 0; i < board.numRows(); i++) {
		for (int j = 0; j < board.numCols(); j++) {
			int RandNum = RandomInteger(0,5);									//change if big board
			board(i,j) = StandardCubes[i * board.numCols() + j][RandNum];		//pick a random letter from each of the 16 cubes
		}
	}
	//swap letters randomly so that each letter is swapped at least once
	for (int i = 0; i < board.numRows(); i++) {						
		for (int j = 0; j < board.numCols(); j++) {
			int RandRow = RandomInteger(0, board.numRows() - 1);
			int RandCol = RandomInteger(0, board.numCols() - 1);
			string s1 = board(i,j);
			string s2 = board(RandRow, RandCol);
			board(i,j) = s2;
			board(RandRow, RandCol) = s1;
		}
	}
	//update graphics
	for (int i = 0; i < board.numRows(); i++) {
		for (int j = 0; j < board.numCols(); j++) {
			LabelCube(i, j, board(i,j)[0]);
		}
	}
	
}

/* Function: UserConfigureBoard
 * ---------------------------
 * This function allows the user to input the letters they want to use in the board.
 * It does so by asking for 16 letters, checking to make sure the configuration has
 * at least that many letters. It then places the letters in the grid and
 * updates the display.
 */

void UserConfigureBoard(Grid<string> &board) {
	string config;
	while (true) {
		cout << "Please enter your configuration. It must be 16 letters: " << endl;
		config = ConvertToUpperCase(GetLine());
		if (config.length() > 15) break;
		cout << "String too short. Enter another string" << endl;
	}
	if (config.length() > 16) { //truncate string to 16 letters
		config = config.substr(0, 16);
	}
	for (int i = 0; i < board.numRows(); i++) { //assign each letter to proper space in board
		for (int j = 0; j < board.numCols(); j++) {
			board(i,j) = config[i * board.numCols() + j];
		}
	}
	for (int i = 0; i < board.numRows(); i++) { //update display
		for (int j = 0; j < board.numCols(); j++) {
			LabelCube(i, j, board(i,j)[0]);
		}
	}
}


/* Part 3: Player's turn
 * ---------------------
 */

/* Function: AreNeighbors
 * ---------------------------
 * This function checks to see if two locations on the board are neighbors.
 * It returns true if the two neighbors are immediately adjacent on the board.
 */

bool AreNeighbors(int row1, int col1, int row2, int col2) {
	if (row2 == -1 && col2 == -1) {							//sentinel values passed for the first cube
		return true;
	} else if (row1 - row2 > 1 || row2 - row1 > 1 || col1 - col2 > 1 || col2 - col1 > 1) { // if any dimensions are more than
		return false;																	   // one apart, return false
	} else if (row1 == row2 && col1 == col2) {
		return false;										//if the cube is checking itself, return false
	} else {
		return true;
	}
}


/* Function: NotDuplicated
 * ---------------------------
 * This function checks to see if a letter has already been used.
 * It checks a given row and column against a list of all the locations
 * already used for that word. 
 */

bool NotDuplicated(int row, int col, Vector<locationT> & locations) {
	for (int i = 0; i < locations.size(); i++) {
		if (locations[i].numRow == row && locations[i].numCol == col) {
			return false;
		}
	}
	return true;
}

/* Function: Findable
 * ---------------------------
 * This function checks to see if a word entered by the user can be found in the puzzle. 
 * It works by checking that each successive letter in the word is both a neighbor to the
 * last letter and has not been used before. In the process, it keeps track of the location
 * of each letter so that it can highlight the word on the board. There is an additional 
 * vector that keeps track of solutions because there are often multiple ways to get a word.
 */

void Findable(Grid<string> & board, string word, int row, int col, Vector<locationT> locations, Vector<Vector<locationT> > & answers) {
	word = ConvertToUpperCase(word);
	if (word.size() == 0) {							//every letter has been found, so add the locations to the set of answers
		answers.add(locations);
	}
	for (int i = 0; i < board.numRows(); i++) {
		for (int j = 0; j < board.numCols(); j++) {
			if (board(i,j) == word.substr(0,1) &&	//checks that the next letter is at a location, that it is a neighbor of the last letter
				AreNeighbors(i,j,row,col) && NotDuplicated(i,j, locations)) {			//and that it hasn't been used before
				locationT location;
				location.numRow = i;
				location.numCol = j;
				locations.add(location);
				Findable(board, word.substr(1), i, j, locations, answers);		//recurs with a truncated word and updated location
				if (word.size() != 0) {
					locations.removeAt(locations.size() - 1);			//word wasn't found going down this path, so remove the letter from 
				}														//the list of letters already seen
			}
		}
	}
}


/* Function: WordIsValid
 * ---------------------------
 * This function checks to see if a word is valid based on four criteria:
 * it is more than 3 letters, it is a real word, it hasn't been found before,
 * and it can be found in the puzzle.
 */

bool WordIsValid(string word, Grid<string> & board, Lexicon & lex, Set<string> & wordsSeen) {
	if (word.length() < 4) {
		return false;
	} else if (wordsSeen.contains(word)) {		//already seen the word
		return false;	
	} else if (!lex.containsWord(word)) {		//not a word according to the dictionary
		return false;
	} else {
		Vector<locationT> empty;
		Vector<Vector<locationT> > results;
		Findable(board, word, -1, -1, empty, results);
		if (results.size() == 0) {							//no paths were found for the word, so word can't be found
			return false;
		} else {
			Vector<locationT> answer = results[0];			//at least one path was found, so return true and highlight the cubes
			for (int i = 0; i < answer.size(); i++) {
				HighlightCube(answer[i].numRow, answer[i].numCol, true);
			}
			Pause(.5);
			for (int i = 0; i < answer.size(); i++) {		//then un-highlight the cubes
				HighlightCube(answer[i].numRow, answer[i].numCol, false);
			}
			return true;
		}
	}
}

/* Function: putWordOnBoard
 * ---------------------------
 * This function adds a word to the board for the human player and
 * updates the list of words that have been found already
 */

void putWordOnBoard(Grid<string> & board, string word, Set<string> & wordsSeen) {
	wordsSeen.add(word);
	RecordWordForPlayer(word, Human);
}


/* Function: PlayerTurn
 * ---------------------------
 * This function controls the flow of the player's turn. The
 * player can enter words for as long as they want, pressing enter
 * when they want to finish their turn. The program checks to see
 * if the word is valid, and if it is it puts the word on the board
 * and updates the player's score.
 */

void PlayerTurn(Grid<string> & board, Lexicon & lex, Set<string> & wordsSeen) {
	while (true) {
		cout << "Please enter a word found in the puzzle (ENTER to finish): ";
		string word = GetLine();
		word = ConvertToUpperCase(word);
		if (word == "") break;
		if (WordIsValid(word, board, lex, wordsSeen)) {
			putWordOnBoard(board, word, wordsSeen);
		} else {
			cout << "Sorry, that word is invalid. ";
		}
	}
}


/* Part 4: Computer's Turn
 * -----------------------
 * The computer finds all the remaining words in the puzzle
 */

/* Function: FindAllWords
 * ---------------------------
 * This function finds all the remaining words in the puzzle. It starts in the top left of the puzzle
 * and finds every word that can be found from each successive position, calling itself recursively. 
 * The function bottoms out if the dictionary doesn't contain any words with the prefix that is 
 * being explored or when the computer has run off the grid. 
 */

void FindAllWords(int row, int col, Grid<string> & board, Lexicon & lex, Set<string> & wordsSeen, string soFar, Vector<locationT> visited) {
	locationT here;
	here.numRow = row;
	here.numCol = col;
	visited.add(here);							
	soFar += board(row,col);					
	if (!lex.containsPrefix(soFar)) {			//return if this is a dead end
		return;
	}
	if (lex.containsWord(soFar) && (!wordsSeen.contains(soFar)) && soFar.size() > 3) {
		RecordWordForPlayer(soFar, Computer);			//found a word, so record it
		wordsSeen.add(soFar);
	}
	for (int i = 0; i < board.numRows(); i++) {
		for (int j = 0; j < board.numCols(); j++) {
			if (AreNeighbors(row, col, i, j) && NotDuplicated(i, j, visited)) {		//recur on the rest of the puzzle for all 
				FindAllWords(i, j, board, lex, wordsSeen, soFar, visited);			//neighbors that haven't already been seen
			}
		}
	}
}

/* Function: ComputerTurn
 * ---------------------------
 * This function completes the computer's turn by finding every word starting
 * from each position on the board.
 */
	
void ComputerTurn(Grid<string> & board, Lexicon & lex, Set<string> & wordsSeen) {	
	for (int i = 0; i < board.numRows(); i++) {
		for (int j = 0; j < board.numCols(); j++) {
			Vector<locationT> empty;
			FindAllWords(i, j, board, lex, wordsSeen, "", empty);
		}
	}
}
			
/* Part 5: Main Function
 * --------------------
 */

/* Function: main
 * ---------------------------
 * This function controls the flow of the game. It initializes the dictionary
 * and the board, asking the user if he wants to configure the board. It then
 * allows the user to play before having the computer go and find the rest
 * of the words. It then asks the user if he wants to play again.
 */

int main()
{
	while (true) {
		//initialize
		Randomize();
		Set<string> wordsSeen;
		Lexicon lex("lexicon.dat");
		Grid<string> board(4,4);			//changes for 5x5
		SetWindowSize(9, 5);
		InitGraphics();
		DrawBoard(4,4);						//changes for 5x5
		Welcome();
		GiveInstructions();
		
		//either set up the board automatically or let the user set it up
		cout << "Would you like to configure the board? ";
		string response = GetLine();
		response = ConvertToUpperCase(response);
		if (response == "YES") {
			UserConfigureBoard(board);
		} else {
			InitializeBoard(board);
		}
		
		//have the player play, then the computer
		PlayerTurn(board, lex, wordsSeen);
		ComputerTurn(board, lex, wordsSeen);
		
		//check if the user wants to play again
		cout << "Would you like to play again? ";
		response = GetLine();
		response = ConvertToUpperCase(response);
		if (response != "YES") break;
	}
	return 0;
}



