#include "../utils.h"

typedef pair<int,int> coordinate;
typedef pair<coordinate,coordinate> startEnd;

tuple<map<startEnd, int>, set<coordinate>, set<coordinate>> GetInfo() {
  // Returns a map of numbers and their coords, the set of valid coords, and the set of gear coords
  ifstream input("input.txt");
  map<startEnd, int> num_coords;
  set<coordinate> valid_coords;
  set<coordinate> gear_coords;
  string line;
  int y = 0;
  while (getline(input, line)) {
    startEnd start_end_coords;
    bool is_digit = false;
    int num = 0;
    for (int x = 0; x < line.length(); ++x) {
      char c = line[x];
      if (isdigit(c) && !is_digit) {
        is_digit = true;
        num = c - '0';
        start_end_coords.first = {x,y};
      } else if (c - '0' <= 9 && c - '0' >= 0 && is_digit) {
        num = 10 * num + c - '0';
      }
      if ((!isdigit(c) || x == line.length() - 1) && is_digit) {
        is_digit = false;
        start_end_coords.second = {x-1,y};
        num_coords[start_end_coords] = num;
      } 
      if (!isdigit(c) && c != '.') {
        for (int i = x - 1; i <= x + 1; ++i) {
          for (int j = y - 1; j <= y + 1; ++j) {
            valid_coords.emplace(i,j);
          }
        }
        if (c == '*') {
          gear_coords.emplace(x,y);
        }
      }
    }
    ++y;
  }
  return std::make_tuple(num_coords, valid_coords, gear_coords);
}

int PartOne(map<startEnd, int> num_coords, set<coordinate> valid_coords) {
  int answer = 0;
  for (const auto& num_info : num_coords) {
    coordinate start = num_info.first.first;
    coordinate end = num_info.first.second;
    // Nums are at most three digits long so if the middle is touching a symbol, then so are the end points
    if (valid_coords.count(start) || valid_coords.count(end)) {
      answer += num_info.second;
    }
  }
  return answer;
}

int PartTwo(map<startEnd, int> num_coords, set<coordinate> gear_coords) {
  int answer = 0;
  map<coordinate, vector<int>> gear_heat_map;
  for (const auto& num_info : num_coords) {
    coordinate start = num_info.first.first;
    coordinate end = num_info.first.second;
    for (int x = start.first - 1; x <= end.first + 1; ++x) {
      for (int y = start.second - 1; y <= start.second + 1; ++y) {
        gear_heat_map[{x,y}].push_back(num_info.second);
      }
    }
  }
  for (const auto& gear : gear_coords) {
    if (gear_heat_map[gear].size() == 2) {
      answer += gear_heat_map[gear][0] * gear_heat_map[gear][1];
    }
  }
  return answer;
}

int main() {
  auto [num_coords, valid_coords, gear_coords] = GetInfo();
  int answer_one = PartOne(num_coords, valid_coords);
  int answer_two = PartTwo(num_coords, gear_coords);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
