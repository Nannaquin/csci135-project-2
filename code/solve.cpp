/* Name: Steven Santana
   Course: {135 Shankar, 136}
   Instructor: X. Zhang
   Assignment: Proj 2 Final Submission
*/	
/*

THE PROCESS
------------

	1) Create dynamic bool array in solve function to keep track of all words from Puzzle
	
	2) Search for horizontal and vertical patterns, send them to patternHunter()
	
		a) Assembling a dynamic bool array corresponding to the locations of words in puzzle.words[],
		with only words equal to the patterns length set to true
		
		b) Check pattern string for any found letters to disqualify words from the patternHunter() list ahead of guessing
		
		c) Using letFinder(), take a tally for the letters of the valid word list members at the given position. 
			The most frequent letter is returned.
		
		d) Said letter is attempted in a guess
		 
			i) if /false/, disqualify all words on local list that have failed char at the current position,
			then try until a correct entry is found based upon remaining words
			
			ii)once /true/ is returned from a character guess, disqualify all words in the local list that
			do NOT have this correct character at the current position, and continue until word/pattern is solved
		
		e) Shut down the solved word in the outermost bool array (the one that resides in void solve()).
	
	3) Repeat Step 2 until all spaces are successfuly solved.
*/

#include "solve.h"

using namespace std;

// Bool Pointer (bPtr) used for bool dyn array, to disqualify words as it goes along
typedef bool* bPtr;

enum Direction {HORIZ, VERT}; // for orientation of patterns/words.

/** Changes from its original (supplied) condition.
	> default brute force pick function removed.
	> solve function modified to utilize the above approach. 
*/



/** Creates a temporary boolean array as to not interfere with
	later word solving attempts. Only validates words as long as the
	found pattern.
	@pre  A pattern has been found.
	@post  A boolean array corresponding to valid words to guess is created.
	@param  &puzzle  A reference to the puzzle, for its words.
			&inArr  A reference to the main array that denotes what words are
					valid to use in the current solving attempt.
			patLen  The length of the pattern being solved.  */
bPtr loadBool(Puzzle &puzzle, bPtr &inArr, int patLen) {
	bPtr lBtr = new bool[puzzle.numWords];
	for(int i = 0; i < puzzle.numWords; i++) {
		if((puzzle.words[i].length() == patLen)&&(inArr[i]!=false)) {
			lBtr[i] = true;
		}
		else lBtr[i] = false;	
	}
	return lBtr; 
}

/** Disqualifies a correctly guessed word from being guessed in the future.
@pre  A word has been solved fully.
@post  The first instance of that word found in the list of words will no longer
	be eligible for guessing.
@param  wHold  The word to be removed.
		&bArr A reference to the outer array of booleans corresponding to the puzzle's
			words.
		&puzzle  A reference to the puzzle for its words and number of words.*/
void postDisqual(string wHold, bPtr &bArr, Puzzle &puzzle) {
	for(int i = 0; i < puzzle.numWords; i++) 	
		if((puzzle.words[i] == wHold)&&(bArr[i]!=false)){
			bArr[i] = false;
			return;
		}	

}

/** Disqualifies words having the given incorrect letter at the given position, OR
	disqualifies words that do not have the given correct letter at the given
	position.
	@pre  A guess attempt has been made.
	@post  Words will no longer be attempted in this round. 
	@param  letHold  The letter in question.
			pos  The position of the letter in question.
			&tempCount  This round's collection of boolean values corresponding to words
				eligible to be guessed.
			&puzzle  A reference to the puzzle and its data.
			isWin  If corresponding guess was correct or not. */ 
void disqualWords(char letHold, int pos, bPtr &tempCount, Puzzle &puzzle, bool isWin){

	for(int i = 0; i < puzzle.numWords; i++) {
		if(isWin == false)  {
			if((puzzle.words[i][pos] == letHold)&&(tempCount[i]!=false))
				tempCount[i] = false;
		}
		else { 
			if((puzzle.words[i][pos] != letHold)&&(tempCount[i]!=false))
				tempCount[i] = false;
		}
	}
}

/** Determines what letter would be most likely to provide a successful guess.
	@pre  The process of solving a word is ongoing.
	@post  A (hopefully) correct letter will be provided.
	@param  pos  What position in the word to guess for.
			tmpArr  The array of booleans that determines what words
				can be used to arrive to the answer.
			&puzzle  A ref to the puzzle for its words and max words value.
	@return  The function's guess to the correct letter. */
char letFinder(int pos, bPtr tmpArr, Puzzle &puzzle) {


	const int ENG_ABC_LIMIT = 26; // somewhat self explanitory
	char cArr[ENG_ABC_LIMIT]; // Representative of every letter of the english alphabet, in lower case.
	int tArr[ENG_ABC_LIMIT]; // An array for tallying, corresponding to the above array.
	
	//Initial filling
	for(int i = 0; i < ENG_ABC_LIMIT; i++) {
		cArr[i] = (char) ((int) 'a' + i);
		tArr[i] = 0;
	}
	
	// the tallying process for most popular character at position pos among eligable words
	for(int i = 0; i < puzzle.numWords; i++) 
		if(tmpArr[i] == true)  // ideally will not register disqualified words
			for(int j = 0; j < ENG_ABC_LIMIT; j++) 
				//Tally for letters
				if(puzzle.words[i][pos] == (char) ((int)'a' + j)) 
					tArr[j]++;
				
			
		
	
	// Determining the most popular character based on previous tallying.
	int theBig = 0; // Most freq character for the entry to guess out of remaining words.
	for(int i = 0; i < ENG_ABC_LIMIT; i++) 
		if(tArr[i] > tArr[theBig]) 
			theBig = i;

	return cArr[theBig];
}



/** Disqualifies words ahead of guess based upon discovered words and cross sections
	@pre  A word solving attempt has begun.
	@post  A smaller selection of words to derive guesses from will be made.
	@param  pat  The pattern itself.
			&tArr  The array that tells what words are valid to use.
			&puzzle  The puzzle, to use its word collectiom and max words value. */
void preDisqual(string pat, bPtr &tArr, Puzzle &puzzle)  
{	
	for(int i = 0; i < pat.length(); i++) 
		if(pat[i]!='?') 
			for(int j = 0; j < puzzle.numWords; j++) 
				if(puzzle.words[j][i] != pat[i]) 
					tArr[j] = false;
}
					
/** Solves an entire pattern/word before moving on.
	@pre  An eligible pattern has been found.
	@post  The pattern will be solved.
	@param  &puzzle  A ref to the actual puzzle.
			&pat  The pattern discovered. 
			&r  "Row" coordinate
			&c  "Column" coordinate.
			&bArr  Ref to the main bool array.
			words[]  Effectively no longer serves a purpose.
			dir  What direction to process the pattern. */
void patternHunter (Puzzle &puzzle, string &pat, const int &r, const int &c, bPtr &bArr, string words[], Direction dir) {
	
	// True for any word of same length as pat that hasnt been solved.
	bPtr tempCount = loadBool(puzzle, bArr, pat.length());
	preDisqual(pat, tempCount, puzzle); // reducing chances for mistakes
	
	int pos = 0; // utilized in guess and disqualification process
	string wHold; // holds words until end, then disqualifies the correct one from the overlist	
	
	while(pos < pat.length()) {
		
		if (pos >= pat.length()) break;
		
//		char letHolder; // Holds letters to be attempted via puzzle.guess
		bool boolHold = false; //The flag for telling if its time to move to next position
		
		while (boolHold == false) { 
			
			char letHolder = letFinder (pos, tempCount, puzzle); 

			//Where the guesswork is done
			if(dir == HORIZ) 
				boolHold = puzzle.guess(r, c+pos, letHolder); 
			else {
				boolHold = puzzle.guess(r+pos, c, letHolder); 
			}
			
			disqualWords(letHolder, pos, tempCount, puzzle, boolHold);
			
			if(boolHold == true) {
				pos++;
				wHold += letHolder;
			}
		} 
		
	}
	//Removing correct word from outer list
	postDisqual(wHold, bArr, puzzle);
		
	delete tempCount;
	tempCount = NULL;
}


/** *************** **
 ** OUTER FUNCTIONS **
 ** *************** **/

/** Generates an array of boolean values corresponding to the used portion of
	the array of words from the puzzle.
	@pre  The solving process has begun.
	@post  A master list of boolean values corresponding to the words in the puzzle
		will be created.
	@param  arrSize  The amount of words in the puzzle. 
	@return  The newly created array of boolean values. */
bPtr generateBool(int arrSize) {
	bPtr bArr = new bool[arrSize];
	for(int i = 0; i < arrSize; i++) {
		bArr[i] = true;
	}
	return bArr;
}

/** Determines the length of a potential pattern by copying the pattern itself.
	@pre  A part of a pattern has been detected.
	@post  For the given Direction, if a valid sequence is found, it will
		be returned.
	@param  &puzzle  A reference to the puzzle itself.
			r  The y value coordinate of the detected segment.
			c  The x value coordinate of the detected segment.
			dir  The assumed orientation of the detected segment.
	@return  A non-empty string if a pattern is detected, otherwise an empty string. */
string searchPattern(Puzzle &puzzle, int r, int c, Direction dir) {
	string s = "";
	if(dir == HORIZ) {
		 // if character to the left is blank, it could be a start of a pattern
	    if (puzzle.get(r, c-1) == ' ') {
	        // scan all characters until we find a blank square
	        while(puzzle.get(r, c) != ' ') s += puzzle.get(r, c++);
	    } 	
	}
	else {
	    // if character to the north is blank, it could be a start of a pattern
	    if (puzzle.get(r-1, c) == ' ') {
	        // scan all characters until end of segment is found
	        while(puzzle.get(r, c) != ' ') s += puzzle.get(r++, c); 
	    } 
	}
    // return the pattern (or empty string if there is no pattern starting at (c,r))
    return s;
}

/* Essentially main() */
void solve(Puzzle &puzzle) {

	bPtr bArr = generateBool(puzzle.numWords);
	puzzle.print();  
	for (int r = 0; r < puzzle.height; r++) {
		for (int c = 0; c < puzzle.width; c++)
			if (puzzle.get(r,c) != ' ') { 
				 
				string str; // holding to be discovered "pattern"
				str = searchPattern(puzzle, r, c, HORIZ);
				if(str.length() > 1) 
					patternHunter(puzzle, str, r, c, bArr, puzzle.words, HORIZ);

				str = "";
				str = searchPattern(puzzle, r, c, VERT);
				if(str.length() > 1)
					patternHunter(puzzle, str, r, c, bArr, puzzle.words, VERT);

				// print the picture
				puzzle.print();
      }  
  }
  puzzle.print(); // my edit
}


