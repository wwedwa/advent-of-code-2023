
#include "utils.h"

using aoc::split;

unordered_map<string, int> maxColors(string line) {
  unordered_map<string, int> maxes = {{"red", -1}, {"green", -1}, {"blue", -1}};
  vector<string> turns = split(line, "; ");
  for (string turn : turns) {
    vector<string> pulls = split(turn, ", ");
    for (string pull : pulls) {
      vector<string> info = split(pull);
      int count = std::stoi(info[0]);
      string color = info[1];
      if (color == "red" && count > maxes["red"]) {
        maxes["red"] = count;
      } else if (color == "green" && count > maxes["green"]) {
        maxes["green"] = count;
      } else if (color == "blue" && count > maxes["blue"]) {
        maxes["blue"] = count;
      }
    }
  }
  return maxes;
}

int PartOne() {
  int answer = 0;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    vector<string> parsed_line = split(line, ": ");
    int idx = std::stoi(parsed_line[0].substr(5));
    unordered_map<string, int> maxes = maxColors(parsed_line[1]);
    if (maxes["red"] <= 12 && maxes["green"] <= 13 && maxes["blue"] <= 14) {
      answer += idx;
    }
  }
  return answer;
}

int PartTwo() {
  int answer = 0;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    vector<string> parsed_line = split(line, ": ");
    unordered_map<string, int> maxes = maxColors(parsed_line[1]);
    answer += maxes["red"] * maxes["green"] * maxes["blue"];
  }
  return answer;
}

int main() {
  int answer_one = PartOne();
  int answer_two = PartTwo();
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}