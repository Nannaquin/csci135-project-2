
#include "puzzle.h"
#include <climits>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <fstream>

using namespace std;

namespace {
  bool loaded = false;
  int w = 0;
  int h = 0;
  int guess_count = 0;
  int correct_count = 0;
  int mistake_count = 0;

  /* board statistics */
  int revealed_cells = 0;
  int hidden_cells = 0;
  int total_cells = 0;
  
  vector < vector <char> > answer;
  vector < vector <char> > board;

  /* reporting */
  string reporting_file;

  vector<string> generate_word_list(){
    vector<string> v;
    for(int r = 0; r < h; r++) {
      for(int c = 0; c < w; c++) {

        // horizontal
        if (c == 0 || answer[r][c-1] == ' ') {
          string s = "";
          int dc = 0;
          while(c + dc < w && answer[r][c + dc] != ' ') {
            s += answer[r][c + dc];
            dc ++;
          }
          if (s.size() > 1) v.push_back(s);
        }

        // vertical
        if (r == 0 || answer[r-1][c] == ' ') {
          string s = "";
          int dr = 0;
          while(r + dr < h && answer[r + dr][c] != ' ') {
            s += answer[r + dr][c];
            dr ++;
          }
          if (s.size() > 1) v.push_back(s);
        }

      }
    }

    sort(v.begin(), v.end());

    return v;
  }

  void report_message(string msg) {

    if (reporting_file.empty() || reporting_file == "-") {
      cout << msg;
    }
    else {
      ofstream output(reporting_file.c_str());
      if (!output.fail()) {
        output << msg;
      }
      else {
        cout << msg;
      }
      output.close();
    }
  }

  void check_if_finished() { 

    if (hidden_cells == 0) {
      // confirm
      assert(revealed_cells == total_cells);
      for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
          assert(board[r][c] != '?');
          assert(board[r][c] == answer[r][c]);
        }
      }
      report();
      exit(0);
    }
    else if (guess_count > total_cells*26 && hidden_cells > 0) {
      report();
      exit(0);
    }

  }

}

void load(istream &in, string report_file) {
 
  if (loaded) {
    return;
  }

  loaded = true;

  reporting_file = report_file;

  vector<string> raw;
  int min_row = INT_MAX;
  int min_col = INT_MAX;
  int max_row = -1;
  int max_col = -1;
  bool non_empty = false;
  {
    int r = 0;

    string s;
    while(getline(in, s)) {
      bool found_nsp = false;
      for(size_t c = 0; c < s.size(); c++) {
        if (!isspace(s[c])) {
          found_nsp = true;
          non_empty = true;
          if ((int)c < min_col) min_col = c;
          if ((int)c > max_col) max_col = c;
        }
      }

      if (found_nsp) {
        if (r < min_row) min_row = r;
        if (r > max_row) max_row = r;
      }
      
      raw.push_back(s);
      ++r;
    }
  }
 
  if (non_empty) {
    h = max_row - min_row + 1;
    w = max_col - min_col + 1;

    answer.resize(h);
    board.resize(h);
    for(int r = 0; r < h; r++){
      answer[r].resize(w);
      board[r].resize(w);
      
      for(int c = 0; c < w; c++) {
        char ch = ' ';
        if ((size_t)min_col + c < raw[min_row + r].size())
          ch = raw[min_row + r][min_col + c];

        answer[r][c] = ch;

        if (ch == '?') ch = '-'; // if actual crossword contains a ? mark
                                 // replace it with dash
        
        // fill the board with '?' and increament the board counters
        if (isspace(ch)) 
          board[r][c] = ' ';
        else {
          board[r][c] = '?';
          hidden_cells += 1;
          total_cells += 1;
        }
      }
    }
  }
  else {
    w = 0;
    h = 0;
  }

}
  
void report() {
  stringstream ss;
  
  if (hidden_cells == 0) {
    ss << "Success" << endl;
  }
  else {
    ss << "Failure" << endl;
  }

  string sep = " ";
  ss << "Letters: " << total_cells << sep;
  ss << "Guesses: " << guess_count << sep;
  ss << "Correct: " << correct_count << sep;
  ss << "Mistakes: " << mistake_count << sep;
  ss << endl;

  report_message(ss.str());
}


Puzzle::Puzzle() : width(w), height(h), numWords(generate_word_list().size()) { 
  vector<string> ws (generate_word_list());
  for(size_t i = 0; i < CAPACITY; i++) {
    string x = "abracadabra";
    int var = x.size() - 1;
    int j = i % var;
    words[i] = x.substr(j, x.size() - j);
  }
  for(size_t i = 0; i < ws.size(); i++) {
    words[i] = ws[i];
  }
}

void Puzzle::print() {
  cout << endl;

  cout << "Guess count: " << guess_count << " (Correct: " << correct_count << ", Mistakes: " << mistake_count << ")" << endl;
  
  cout << endl;
  for(size_t r = 0; r < board.size(); r++) { 
    cout << ' ';
    for(size_t c = 0; c < board[r].size(); c++) {
      cout << board[r][c] << ' ';
    }
    cout << endl;
  }
  cout << endl;
}

bool Puzzle::guess(int r, int c, char ch_guess) {

  char ch = ' ';

  if (r < 0 || c < 0 || r >= h || c >= w)
    // if out ot range
    ch = ' ';
  else if (board[r][c] != '?')
    // if already known
    ch = board[r][c];
  else {
    // otherwise, if guessing at a question mark
    ch = answer[r][c];
    guess_count ++;
    if (ch_guess == ch) {
      // when guess is correct
      board[r][c] = ch;
      correct_count ++;

      revealed_cells ++;
      hidden_cells --;
    }
    else {
      // when guess is not correct
      mistake_count ++;
    }
    // since it's a guess, check if it's the end of the game  
    check_if_finished();
  }

  return (ch == ch_guess);
}

char Puzzle::get(int r, int c) {
  if (r < 0 || c < 0 || r >= h || c >= w)
    return ' ';
  else 
    return board[r][c];
}

