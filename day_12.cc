#include "../utils.h"
#include <regex>

using aoc::split;
using aoc::GetInts;

map<pair<string,vector<int>>, long> cache;

vector<pair<string,vector<int>>> GetInfo() {
  vector<pair<string,vector<int>>> spring_map;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    vector<string> split_info = split(line);
    string damaged_data = split_info[0];
    vector<int> backup = GetInts(split(split_info[1], ","));
    spring_map.emplace_back(damaged_data, backup);
  }
  return spring_map;
}

vector<pair<string,vector<int>>> UnfoldData(vector<pair<string,vector<int>>> orig_map) {
  vector<pair<string,vector<int>>> new_map;
  for (const auto& line : orig_map) {
    pair<string,vector<int>> new_line;
    for (int i = 0; i < 5; ++i) {
      for (char c : line.first) {
        new_line.first += c;
      }
      if (i != 4) {
        new_line.first += '?';
      }
      for (int count : line.second) {
        new_line.second.push_back(count);
      }
    }
    new_map.push_back(new_line);
  }
  return new_map;
}

long GetValidArrangements(const pair<string,vector<int>>& line) {
  if (cache.count(line)) {
    return cache[line];
  }
  long count = 0;
  string pattern = line.first;
  vector<int> backup = line.second;
  int min_length = backup.size() - 1;
  for (int len : backup) {
    min_length += len;
  }
  if (backup.empty()) {
    cache[line] = pattern.find('#') == string::npos;
    return pattern.find('#') == string::npos;
  }
  if (pattern.empty()) {
    return 0;
  }
  if (pattern.length() < min_length) {
    cache[line] = 0;
    return 0;
  }
  if (pattern[0] == '.') {
    return GetValidArrangements({pattern.substr(1), backup});
  }
  else if (pattern[0] == '#') {
    for (int i = 0; i < backup[0]; ++i) {
      if (pattern[i] == '.') {
        cache[line] = 0;
        return 0;
      }
    }
    // Check if pattern is the last backup
    if (pattern.length() == backup[0]) {
      cache[line] = backup.size() == 1;
      return backup.size() == 1;
    }
    if (pattern[backup[0]] != '#') {
      vector<int> new_backup(backup.begin()+1, backup.end());
      return GetValidArrangements({pattern.substr(backup[0] + 1), new_backup});
    }
    cache[line] = 0;
    return 0;
  }
  else if (pattern[0] == '?') {
    pattern[0] = '#';
    count += GetValidArrangements({pattern, backup});
    pattern[0] = '.';
    count += GetValidArrangements({pattern.substr(1), backup});
  }
  cache[line] = count;
  return count;
}

int PartOne(const vector<pair<string,vector<int>>>& spring_map) {
  int answer = 0;
  for (const auto& line : spring_map) {
    answer += GetValidArrangements(line);
  }
  return answer;
}

long PartTwo(const vector<pair<string,vector<int>>>& spring_map) {
  long answer = 0;
  for (const auto& line : spring_map) {
    answer += GetValidArrangements(line);
  }
  return answer;
}

int main() {
  vector<pair<string,vector<int>>> spring_map = GetInfo();
  int answer_one = PartOne(spring_map);
  vector<pair<string,vector<int>>> new_spring_map = UnfoldData(spring_map);
  long answer_two = PartTwo(new_spring_map);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
