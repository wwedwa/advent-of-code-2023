#include "utils.h"

using aoc::split;
using aoc::GetInts;

vector<vector<int>> GetInfo() {
  vector<vector<int>> data;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    data.push_back(GetInts(split(line)));
  }
  return data;
}

bool AllZeros(vector<int> nums) {
  for (int num : nums) {
    if (num != 0) {
      return false;
    }
  }
  return true;
}

int GetNextVal(const vector<int>& history) {
  if (AllZeros(history)) {
    return 0;
  }
  vector<int> new_history;
  for (int i = 0; i < history.size() - 1; ++i) {
    new_history.push_back(history[i+1] - history[i]);
  }
  return history[history.size() - 1] + GetNextVal(new_history);
}

int GetPrevVal(const vector<int>& history) {
  if (AllZeros(history)) {
    return 0;
  }
  vector<int> new_history;
  for (int i = 0; i < history.size() - 1; ++i) {
    new_history.push_back(history[i+1] - history[i]);
  }
  return history[0] - GetPrevVal(new_history);
}

int PartOne(vector<vector<int>> data) {
  int answer = 0;
  for (const vector<int>& history : data) {
    answer += GetNextVal(history);
  }
  return answer;
}

int PartTwo(vector<vector<int>> data) {
  int answer = 0;
  for (const vector<int>& history : data) {
    answer += GetPrevVal(history);
  }
  return answer;
}

int main() {
  vector<vector<int>> data = GetInfo();
  int answer_one = PartOne(data);
  int answer_two = PartTwo(data);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
