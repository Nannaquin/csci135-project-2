
#include <iostream>
#include <fstream>
#include <climits>
#include <cstring>

#include "puzzle.h"
#include "solve.h"

using namespace std;


int main(int argc, char *argv[]) {

  // Prepare the puzzle
  string report_file;
  if (argc >= 4) {
    report_file = argv[3];
  }

  if (argc < 2 || (argc >= 2 && strlen(argv[1]) == 1 && argv[1][0] == '-')) {
    load(cin, report_file);
  }
  else {
    ifstream input(argv[1]);
    load(input, report_file);
    input.close();
  }
 
  Puzzle puzzle;

  // Run the puzzle solver
  solve(puzzle);
  
  report();
}
