#include "utils.h"

int GetCalVal(string line) {
  int first_digit = -1;
  int last_digit = -1;
  for (int i = 0; first_digit < 0 || first_digit > 9; i++) {
    first_digit = line[i] - '0';
  }
  for (int i = line.length() - 1; last_digit < 0 || last_digit > 9; i--) {
    last_digit = line[i] - '0';
  }
  return first_digit * 10 + last_digit;
}

int GetCalVal2(string line) {
  unordered_map<string, int> string_to_num = {{"0",0}, {"1",1}, {"2",2}, {"3",3}, {"4",4}, {"5",5}, {"6",6}, {"7",7}, 
                                  {"8",8}, {"9",9}, {"zero", 0}, {"one",1}, {"two",2}, {"three",3}, {"four",4},
                                  {"five",5}, {"six",6}, {"seven",7}, {"eight",8}, {"nine",9}};
  int found;
  int first_digit_idx = line.length();
  int last_digit_idx = -1;
  int first_digit = -1;
  int last_digit = -1;
  for (auto pair : string_to_num) {
    found = line.find(pair.first);
    if (found != string::npos && found <= first_digit_idx) {
      first_digit_idx = found;
      first_digit = pair.second;
    }
    found = line.rfind(pair.first);
    if (found != string::npos && found > last_digit_idx) {
      last_digit_idx = found;
      last_digit = pair.second;
    }
  }
  return first_digit * 10 + last_digit;
}

int PartOne() {
  ifstream input("input.txt");
  string line;
  int total = 0;
  while (std::getline(input, line)) {
    total += GetCalVal(line);
  }
  return total;
}

int PartTwo() {
  ifstream input("input.txt");
  string line;
  int total = 0;
  while (std::getline(input, line)) {
    total += GetCalVal2(line);
  }
  return total;
}

int main() {
  int answerOne = PartOne();
  int answerTwo = PartTwo();
  cout << "Part One: " << answerOne << endl;
  cout << "Part Two: " << answerTwo << endl;
  return 0;
}