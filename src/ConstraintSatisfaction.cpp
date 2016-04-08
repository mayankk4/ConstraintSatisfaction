//============================================================================
// Name        : ConstraintSatisfaction.cpp
// Author      : Mayank Kandpal
// Version     : 0.1
// Copyright   : 2016
// Description : Implementing a backtracking algorithm for a Constraint
//               Satisfaction Problem.
//============================================================================

/**
 * Backtracking, CSP.
 *
 * Crossword design
 *   There is a board of size: n x m and a set of words.
 *   We need to find such a way of writing words on the board such that one cell includes at most
 *   one letter and every continuous sequence of letters read horizontally and vertically is a word
 *   from a subset of the size k of the set S acquired in the following  way -
 *
 *   a) download the list of English lemmas extracted from the British National Corpus prepared by
 *   	Prof. Adam Kilgarriff: https://www.kilgarriff.co.uk/bnc-readme.html
 *
 *   b) select from the list only: adverbs (adv), verbs (v), adjectives (a) and nouns (n)
 *
 *
 * Formulating the problem as CSP -
 *
 * Let us assume we are give na board of size NxM. This means there are N columns and M rows.
 *
 * We download the list of English lemmas and extract the list of adverbs, verbs, adjectives
 * and nouns. This is set S.
 *
 * We then randomly choose a predefined number (k) of elements from S. This is set K.
 *
 * Variables-
 * Each row and each column is considered as a variable for the CSP. SO, we have m + n variables.
 * Let us call them R-1 ... R-M and C-1 ... C-N
 * Let us represent letters in the row-variables as R-i[1] ... R-i[N]
 * Let us represent letters in the row-variables as C-i[1] ... C-i[M]
 *
 * Domain-
 * The domain for each row-variable R-1 ... R-M is all combinations of words from set K such that
 * total length of words is <= N. Words are separated by any number of whitespace.
 * The domain for each column-variable C-1 ... C-N is all combinations of words from set K such that
 * total length of words is <= M. Words are separated by any number of whitespace.
 *
 * Constraints-
 * 1. R-i[j] = C-j[i]
 * 2. 1 <= i < M
 * 3. 1 <= j <= M
 * 4. Every continuous sequence of letters read horizontally and vertically is a word from set K.
 *
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::stringstream;
using std::ifstream;
using std::unordered_map;
using std::unordered_set;

void out(string s) {
    cout << s << endl;
}

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}

int main() {
	// Configuration Params.
	string file_path = "src/lemma.al.txt";
	int word_limit = 10;  // number of words in the puzzle
	int board_rows = 15;
	int board_columns = 15;

	out("Reading the file containing english lemmas.");
	ifstream lemma_file(file_path);
	if(lemma_file.fail()) {
		out("Could not open the lemma file.");
	}

	// Process the file into an inmemory datastructure.
	// Select from the list only: adverbs (adv), verbs (v), adjectives (a) and nouns (n)
	string line;
	vector<string> words;
	while (getline(lemma_file, line)) {
		vector<string> parts = split(line, ' ');
		if (parts[3] == "adv" || parts[3] == "v" || parts[3] == "a" || parts[3] == "n") {
			words.push_back(parts[2]);
		}
	}
	out("Finished processing the file.");

	cout << "Using board size " << board_columns << "x" << board_rows <<
			" and " << words.size() << " words." << endl;


	// Randomly select k words from words vector.
	vector<string> words_selected;




	// Map the words by their size.
	unordered_map<int, unordered_set<string> > word_size_map;
	for (string word : words_selected) {
		word_size_map[word.size()].insert(word);
	}

	// Find domain for all row variables
	// Combination of whitespace(s) separated words of length <= board_columns
	vector<string> row_variable_domain;

	// Find domain for all column variables
	// Combination of whitespace(s) separated words of length <= board_rows
	vector<string> column_variable_domain;

	// The first 3 constraints are satisfied by representing the board as a 2D array.
    char board[board_rows][board_columns];


	return 0;
}
