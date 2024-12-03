#include "../utils.h"

map<vector<string>, int> cache;

vector<string> GetInfo() {
  vector<string> rock_map;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    rock_map.push_back(line);
  }
  return rock_map;
}

void TiltNorth(vector<string>& rock_map) {
  for (int i = 0; i < rock_map.size(); ++i) {
    for (int j = 0; j < rock_map[i].length(); ++j) {
      if (rock_map[i][j] == 'O') {
        int row = i;
        while (row != 0 && rock_map[row - 1][j] == '.') {
          --row;
        }
        rock_map[i][j] = '.';
        rock_map[row][j] = 'O';
      }
    }
  }
}

void TiltSouth(vector<string>& rock_map) {
  for (int i = rock_map.size() - 1; i >= 0; --i) {
    for (int j = 0; j < rock_map[i].length(); ++j) {
      if (rock_map[i][j] == 'O') {
        int row = i;
        while (row != rock_map.size() - 1 && rock_map[row + 1][j] == '.') {
          ++row;
        }
        rock_map[i][j] = '.';
        rock_map[row][j] = 'O';
      }
    }
  }
}

void TiltEast(vector<string>& rock_map) {
  for (int j = rock_map[0].length() - 1; j >= 0; --j) {
    for (int i = 0; i < rock_map.size(); ++i) {
      if (rock_map[i][j] == 'O') {
        int col = j;
        while (col != rock_map[0].length() - 1 && rock_map[i][col + 1] == '.') {
          ++col;
        }
        rock_map[i][j] = '.';
        rock_map[i][col] = 'O';
      }
    }
  }
}

void TiltWest(vector<string>& rock_map) {
  for (int j = 0; j < rock_map[0].length(); ++j) {
    for (int i = 0; i < rock_map.size(); ++i) {
      if (rock_map[i][j] == 'O') {
        int col = j;
        while (col != 0 && rock_map[i][col - 1] == '.') {
          --col;
        }
        rock_map[i][j] = '.';
        rock_map[i][col] = 'O';
      }
    }
  }
}

int PartOne(vector<string> rock_map) {
  int answer = 0;
  TiltNorth(rock_map);
  for (int i = 0; i < rock_map.size(); ++i) {
    int rock_num = count(rock_map[i].begin(), rock_map[i].end(), 'O');
    answer += rock_num * (rock_map.size() - i);
  }
  return answer;
}

int PartTwo(vector<string> rock_map) {
  int answer = 0;
  int cycle_length = 0;
  bool cycle = false;
  vector<string> old_map = rock_map;
  while (!cycle) {
    vector<string> old_map = rock_map;
    TiltNorth(rock_map);
    TiltWest(rock_map);
    TiltSouth(rock_map);
    TiltEast(rock_map);
    cache[old_map] = cycle_length;
    cycle = cache.count(rock_map);
    ++cycle_length;
  }
  int cycle_start = cache[rock_map];
  cycle_length -= cycle_start;
  for (int i = 0; i < (1000000000 - cycle_start) % cycle_length; ++i) {
    TiltNorth(rock_map);
    TiltWest(rock_map);
    TiltSouth(rock_map);
    TiltEast(rock_map);
  }
  for (int i = 0; i < rock_map.size(); ++i) {
    int rock_num = count(rock_map[i].begin(), rock_map[i].end(), 'O');
    answer += rock_num * (rock_map.size() - i);
  }
  return answer;
}

int main() {
  vector<string> rock_map = GetInfo();
  int answer_one = PartOne(rock_map);
  int answer_two = PartTwo(rock_map);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
