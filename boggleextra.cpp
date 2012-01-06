/*
 *  boggleextra.cpp
 *  Boggle
 *
 *  Created by Gregory Cohan on 1/6/12.
 *  Copyright 2012 Brown University. All rights reserved.
 *
 */

#include "boggleextra.h"


void PrintBoard(Grid<string> &board) {
	for (int i = 0; i < board.numRows(); i++) {
		for (int j = 0; j < board.numCols(); j++) {
			cout << board(i,j);
		}
		cout << endl;
	}
}


Vector<locationT> WordCanBeFound(string word, Grid<string> & board, Vector<locationT> locations) {
	//	if (word.size() == 0) {
	//		return locations;
	//	} else {
	//		if (FindLetterOnBoard(word.substr(0,1), board).size() == 0) {
	//			return locations;
	//		} else if (!CanGetToNextLetter(word.substr(0,1), word.substr(1,1), board)) {
	//			return locations;
	//		} else {
	//			locations.add
}

/*
 Vector<locationT> WordFindable(Grid<string> &board, string word, int rowIndex, int colIndex, Vector<locationT> locations) {
 if (word.size() == 0) return locations;
 
 if (rowIndex >= board.numRows() && colIndex >= board.numCols()) {
 return locations;
 }
 
 for (int i = 0; i < board.numRows(); i++) {
 for (int j = 0; j < board.numCols(); j++) {
 if (board(i,j) = word.substr(0,1)) {
 Vector<locationT> locationsFound = WordFindable
 
 
 Vector<locationT> FindLetterOnBoard(string letter, Grid<string> & board) {
 Vector<locationT> locations;
 for (int i = 0; i < board.numRows(); i++) {
 for (int j = 0; j < board.numCols(); j++) {
 if (board(i,j) == letter) {
 locationT location;
 location.numRow = i;
 location.numCol = j;
 locations.add(location);
 }
 }
 }
 return locations;
 }
 
 */
