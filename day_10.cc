#include "utils.h"

pair<vector<string>, pair<int,int>> GetInfo() {
  vector<string> pipes;
  pair<int, int> start_idx;
  ifstream input("input.txt");
  string line;
  int i = 0;
  while (getline(input, line)) {
    int found = line.find('S');
    if (found !=string::npos) {
      start_idx = {i, found};
    }
    pipes.push_back(line);
    ++i;
  }
  return {pipes, start_idx};
}

pair<int, vector<pair<int,int>>> PartOne(vector<string> pipes, pair<int,int> start_idx) {
  int answer = 0;
  char curr_pipe = 'L'; // Known from looking at input. CHANGE BASED ON INPUT
  pair<int, int> curr_idx = start_idx;
  pair<int, int> prev_idx = start_idx;
  vector<pair<int,int>> loop_idxs;
  while (curr_pipe != 'S') {
    pair<int, int> temp_idx = curr_idx;
    if (curr_pipe == '|') {
      (curr_idx.first < prev_idx.first) ? (--curr_idx.first) : (++curr_idx.first);
    } else if (curr_pipe == '-') {
      (curr_idx.second > prev_idx.second) ? (++curr_idx.second) : (--curr_idx.second);
    } else if (curr_pipe == 'L') {
      (curr_idx.first == prev_idx.first) ? (--curr_idx.first) : (++curr_idx.second);
    } else if (curr_pipe == 'J') {
      (curr_idx.first == prev_idx.first) ? (--curr_idx.first) : (--curr_idx.second);
    } else if (curr_pipe == '7') {
      (curr_idx.first == prev_idx.first) ? (++curr_idx.first) : (--curr_idx.second);
    } else if (curr_pipe == 'F') {
      (curr_idx.first == prev_idx.first) ? (++curr_idx.first) : (++curr_idx.second);
    }
    ++answer;
    loop_idxs.push_back(curr_idx);
    prev_idx = temp_idx;
    curr_pipe = pipes[curr_idx.first][curr_idx.second];
  }
  return {answer / 2, loop_idxs};
}

// For the given point, look in each direction up, down, left, and right
// count intersections with loop in that direction. Number of intersections determines if point is on inside or outside
// For the special cases where the pipes turn tangent to the looking path, handle with defined map
bool IsInteriorPoint(pair<int, int> point, vector<string> pipes, vector<pair<int,int>> loop_idxs) {
  // Special cases when the pipe turns on the looking path
  unordered_map<string, int> exceptions = {{"LJ", 0}, {"JL", 0}, {"F7", 0}, {"7F", 0},
                                           {"L7", 1}, {"7L", 1}, {"FJ", 1}, {"JF", 1},
                                           {"FL", 0}, {"LF", 0}, {"7J", 0}, {"J7", 0}};
  // Check if point in a part of the loop
  if (find(loop_idxs.begin(), loop_idxs.end(), point) != loop_idxs.end()) {
    return false;
  }
  int end_x = pipes[0].length();
  int end_y = pipes.size();
  int x = point.second + 1;
  int y = point.first;
  int intersections = 0;
  string turn_type = "";
  // Looking right
  while (x < end_x) {
    pair<int,int> curr_pair = {y,x};
    if (find(loop_idxs.begin(), loop_idxs.end(), curr_pair) == loop_idxs.end()) {
      ++x;
      continue;
    }
    if (pipes[y][x] == '|') {
      ++intersections;
    } else if (pipes[y][x] != '-') {
      turn_type += pipes[y][x];
      if (turn_type.length() == 2) {
        intersections += exceptions[turn_type];
        turn_type = "";
      }
    }
    ++x;
  }
  if (intersections % 2 == 0) {
    return false;
  }
  // Looking left
  x = point.second - 1;
  y = point.first;
  intersections = 0;
  while (x >= 0) {
    pair<int,int> curr_pair = {y,x};
    if (find(loop_idxs.begin(), loop_idxs.end(), curr_pair) == loop_idxs.end()) {
      --x;
      continue;
    }
    if (pipes[y][x] == '|') {
      ++intersections;
    } else if (pipes[y][x] != '-') {
      turn_type += pipes[y][x];
      if (turn_type.length() == 2) {
        intersections += exceptions[turn_type];
        turn_type = "";
      }
    }
    --x;
  }
  if (intersections % 2 == 0) {
    return false;
  }
  // Looking up
  x = point.second;
  y = point.first - 1;
  intersections = 0;
  while (y >= 0) {
    pair<int,int> curr_pair = {y,x};
    if (find(loop_idxs.begin(), loop_idxs.end(), curr_pair) == loop_idxs.end()) {
      --y;
      continue;
    }
    if (pipes[y][x] == '-') {
      ++intersections;
    } else if (pipes[y][x] != '|') {
      turn_type += pipes[y][x];
      if (turn_type.length() == 2) {
        intersections += exceptions[turn_type];
        turn_type = "";
      }
    }
    --y;
  }
  if (intersections % 2 == 0) {
    return false;
  }
  // Looking down
  x = point.second;
  y = point.first + 1;
  intersections = 0;
  while (y < end_y) {
    pair<int,int> curr_pair = {y,x};
    if (find(loop_idxs.begin(), loop_idxs.end(), curr_pair) == loop_idxs.end()) {
      ++y;
      continue;
    }
    if (pipes[y][x] == '-') {
      ++intersections;
    } else if (pipes[y][x] != '|') {
      turn_type += pipes[y][x];
      if (turn_type.length() == 2) {
        intersections += exceptions[turn_type];
        turn_type = "";
      }
    }
    ++y;
  }
  return intersections % 2;
}

int PartTwo(vector<string> pipes, vector<pair<int,int>> loop_idxs) {
  int answer = 0;
  for (int i = 0; i < pipes.size(); ++i) {
    for (int j =  0; j < pipes[0].length(); ++j) {
      answer += IsInteriorPoint({i, j}, pipes, loop_idxs);
    }
  }
  return answer;
}

int main() {
  auto [pipes, start_idx] = GetInfo();
  auto [answer_one, loop_idxs] = PartOne(pipes, start_idx);
  // Replace starting point with its pipe since next part doesnt need start
  pipes[start_idx.first][start_idx.second] = 'L';
  int answer_two = PartTwo(pipes, loop_idxs);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
