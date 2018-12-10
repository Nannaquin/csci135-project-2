
#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <vector>

class Puzzle {
  public:
  const int width;
  const int height;

  const static int CAPACITY = 4000;
  std::string words [CAPACITY];
  const int numWords; 

  // Print current state of the grid
  void print();

  // Attempts to guess if the character at position (row, col) is ch.
  // Returns true if yes, or false otherwise
  bool guess(int row, int col, char ch);
  
  // Returns the character at the position (row, col), which can be
  // a letter 'a'-'z', a question mark '?', or a space ' ' for blank squares.
  char get(int row, int col);
  
  Puzzle();
};

void load(std::istream &in, std::string s);
void report();

#endif
