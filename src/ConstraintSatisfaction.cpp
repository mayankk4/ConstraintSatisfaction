//============================================================================
// Name        : ConstraintSatisfaction.cpp
// Author      : Mayank Kandpal
// Version     : 0.1
// Copyright   : 2016
// Description : Implementing a backtracking algorithm for a Constraint
//               Satisfaction Problem.
//============================================================================

/**
 * Crossword design
 *   There is a board of size: n x m and a set of words.
 *   We need to find such a way of writing words on the board such that one cell includes at most
 *   one letter and every continuous sequence of letters read horizontally and vertically is a word
 *   from a subset of the size k of the set S.
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
#include <exception>
#include <iterator>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::stringstream;
using std::ifstream;
using std::unordered_map;
using std::unordered_set;
using std::exception;
using std::strcat;

// Configuration Params.
#define word_limit 5
#define board_rows 10
#define board_columns 20

// Path to the input text file.
string file_path = "src/lemma.al.txt";

// Randomly select k words from words file.
vector<string> words_selected;

// The CROSSWORD design board.
char board[board_rows][board_columns];

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

void print_board() {
	for (int i = 0; i < board_rows; ++i) {
		for (int j = 0; j < board_columns; ++j) {
			cout << (board[i][j]);
		}
		cout << endl;
	}
}

// Example get_word_all_positions("hello", 15);
// Output:
//  hello#####
//	#hello####
//	##hello###
//	###hello##
//	####hello#
//	#####hello
vector<string> get_word_all_positions(string word, int size) {
	vector<string> output;
	if (size < word.size()) {
		return output;
	}

	int padding_size = size - word.size();
	string padding;
	padding.insert(padding.end(), padding_size, '#');

	for (int i = 0; i <= padding_size; ++i) {
		string tmp = padding;
		output.push_back(tmp.insert(i, word));
	}

	return output;
}

// Assigns a word to a row in the board. The word is properly padded
// with whitespaces before passing it to this function.
void set_board_row(int row_number, string word) {
	for (int i = 0; i < board_columns; ++i) {
	  try{
		  board[row_number][i] = word.at(i);
	  } catch (exception& e) {
		cout << "An exception occurred." << endl;
	  }
	}
}

// Extracts words for a column upto a given row.
// Ignore words of size 1.
vector<string> get_words_for_column_upto_row(int column_number, int row_number) {
	string column_value_till_row;
	for (int i = 0; i <= row_number; ++i) {
		column_value_till_row += board[i][column_number];
	}

	vector<string> words;
	int i = 0;
	while (1) {
		// clear the leading group of #
		while((i <= row_number) && (column_value_till_row.at(i) == '#')) {
			++i;
		}

		if (i > row_number) {
			break;
		}

		string word = "";
		while((i <= row_number) && (column_value_till_row.at(i) != '#')) {
			word += column_value_till_row.at(i);
			++i;
		}

		// Means that we didnt find # till row = row_number.
		if (i > row_number) {
			break;
		}

		// This means word ended and character at current i is '#'
		if (word.size() > 1) {
			words.push_back(word);
		}
	}

	return words;
}

// Scans columns and returns the completed words found.
vector<string> get_all_column_words_upto_row(int row_number) {
	vector<string> words;
	for (int i = 0; i < board_columns; ++i) {
		for (auto word : get_words_for_column_upto_row(i, row_number)) {
			words.push_back(word);
		}
	}

	return words;
}

// Updates new remaining words after removing the words found in columns. If we
// find a word which is not present in the word_selected vector, we return false.
bool check_and_remove_column_words(vector<string>* new_remaining_words, int row_number) {

	vector<string> column_words = get_all_column_words_upto_row(row_number);
	for (string word : column_words) {
	    vector<string>::iterator iter = find(
	    		new_remaining_words->begin(), new_remaining_words->end(), word);
	    if (iter == new_remaining_words->end()) {
	    	return false;
	    } else {
	    	new_remaining_words->erase(iter);
	    }
	}

	return true;
}

bool backtrack_design_crossword(vector<string> remaining_words, int row_number) {
	// TODO: There is an edge-case bug here i think.
	if (row_number >= board_rows) {
		return false;
	}

	if (remaining_words.size() == 0) {
		string whitespace_row;
		whitespace_row.insert(whitespace_row.end(), board_columns, '#');
		set_board_row(row_number, whitespace_row);
		// Check columns.
		vector<string> empty;
		return check_and_remove_column_words(&empty, row_number);
	}

	int num_words_left = remaining_words.size();
	for (int i = 0; i < num_words_left; ++i) {  // try to fit each remianing word in this position.
		string word = remaining_words.at(i);
		vector<string> new_remaining_words = remaining_words;
		new_remaining_words.erase(new_remaining_words.begin() + i);

		vector<string> word_padded_all_positions = get_word_all_positions(word, board_columns);
		for (auto padded_word : word_padded_all_positions) {
			// Set the word as row
			set_board_row(row_number, padded_word);

			// Remove some more remaining words based on columns.
			if (!check_and_remove_column_words(&new_remaining_words, row_number)) {
				continue;
			}
			if (backtrack_design_crossword(new_remaining_words, (row_number + 1))) {
				return true;
			}
		}  // try the next padding combination of the word

	}  // try the next word in the row.

	// Try skipping the row - Set the row as ##########
	string whitespace_row;
	whitespace_row.insert(whitespace_row.end(), board_columns, '#');
	set_board_row(row_number, whitespace_row);
	// Check columns to remove words
	vector<string> remaining_words_with_whitespace_row = remaining_words;
	if (!check_and_remove_column_words(&remaining_words_with_whitespace_row, row_number)) {
		return false;
	}
	return (backtrack_design_crossword(remaining_words_with_whitespace_row, row_number + 1));
}

int main() {
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

	cout  << endl << "Using board size " << board_columns << "x" << board_rows <<
			" and " << words.size() << " words." << endl << endl;


	// Select k random words.
    srand (time (nullptr));
    for (int i = 0; i < word_limit; ++i) {
    	int random = rand() % words.size();
    	words_selected.push_back(words.at(random));
    }

	// Map the words by their size.
	unordered_map<int, unordered_set<string> > word_size_map;
	out("The words chosen are:");
	for (string word : words_selected) {
		word_size_map[word.size()].insert(word);
		cout << word << " ";
	}
	cout << endl;

	// The first 3 constraints are satisfied by representing the board as a 2D array.
	for (int i = 0; i < board_rows; ++i) {
		for (int j = 0; j < board_columns; ++j) {
			board[i][j] = '#';
		}
	}

	// Backtracking implementation.
	// For each row, start at index 0 and try to fit next available word at 0.
	// First finish off each index to fit the word using whitespaces
	// Then try next word
	// While passing over to next row, check if column consistency exists and some words
	// can be removed.
	// End condition - no words left or
	if (backtrack_design_crossword(words_selected, 0)) {
		out("\nSuccessfully designed crossword!");
		print_board();
	} else {
		out("\nCould not design crossword!");
	}

	return 0;
}

