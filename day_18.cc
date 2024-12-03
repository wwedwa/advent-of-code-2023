#include "../utils.h"
#include<fstream>

using std::ofstream;
using aoc::split;

typedef pair<long,long> Coord;

tuple<vector<Coord>, vector<Coord>, long> GetInfo() {
  vector<Coord> walls;
  vector<Coord> vertices = {};
  ifstream input("input.txt");
  string line;
  Coord curr_coord = {0,0};
  Coord curr_vertex = {0,0};
  vector<Coord> dirs = {{0,1}, {1,0}, {0,-1}, {-1,0}};
  long total_hex_length = 0;
  while (getline(input, line)) {
    vector<string> info = split(line, " ");
    char char_dir = info[0][0];
    int length = stoi(info[1]);
    string color = info[2].substr(2, info[2].length() - 3);
    int hex_dir = color[color.length() - 1] - '0';
    int hex_length = stol(color.substr(0, color.length() - 1), nullptr, 16);
    total_hex_length += hex_length;
    int dir_num;
    if (char_dir == 'R')
      dir_num = 0;
    else if (char_dir == 'D')
      dir_num = 1;
    else if (char_dir == 'L')
      dir_num = 2;
    else 
      dir_num = 3;
    for (int i = 0; i < length; ++i) {
      curr_coord = {curr_coord.first + dirs[dir_num].first, curr_coord.second + dirs[dir_num].second};
      walls.push_back(curr_coord);
    }
    curr_vertex = {curr_vertex.first + dirs[hex_dir].first*hex_length, curr_vertex.second + dirs[hex_dir].second*hex_length};
    vertices.push_back(curr_vertex);
  }
  return {walls, vertices, total_hex_length};
}

pair<Coord, Coord> GetMinsMaxes(const vector<Coord>& walls) {
  int min_row = INT_MAX;
  int min_col = INT_MAX;
  int max_row = 0;
  int max_col = 0;
  for (const Coord& node : walls) {
    if (node.first < min_row)
      min_row = node.first;
    if (node.first > max_row)
      max_row = node.first;
    if (node.second < min_col)
      min_col = node.second;
    if (node.second > max_col)
      max_col = node.second;
  }
  return {{min_row, min_col}, {max_row, max_col}};
}

vector<Coord> GetNeighbors(const Coord& loc, const vector<Coord>& walls) {
  vector<Coord> neighbors;
  for (int i = -1; i <=1; i += 2) {
    Coord horiz_neighbor = {loc.first, loc.second + i};
    Coord vert_neighbor = {loc.first + i, loc.second};
    if (find(walls.begin(), walls.end(), horiz_neighbor) == walls.end()) {
      neighbors.push_back(horiz_neighbor);
    }
    if (find(walls.begin(), walls.end(), vert_neighbor) == walls.end()) {
      neighbors.push_back(vert_neighbor);
    }
  }
  return neighbors;
}

int FloodFill(const Coord& start, const vector<Coord>& walls) {
  queue<Coord> to_visit;
  set<Coord> explored;
  to_visit.push(start);
  explored.insert(start);
  while (!to_visit.empty()) {
    Coord curr_node = to_visit.front();
    to_visit.pop();
    vector<Coord> neighbors = GetNeighbors(curr_node, walls);
    for (const Coord& neighbor : neighbors) {
      if (!explored.count(neighbor)) {
        to_visit.push(neighbor);
        explored.insert(neighbor);
      }
    }
  }
  return explored.size();
}

int PartOne(const vector<Coord>& walls) {
  auto [mins, maxes] = GetMinsMaxes(walls);
  int min_row = mins.first;
  int min_col = mins.second;
  int max_col = maxes.second;
  Coord start = {-1, -1};
  for (int i = min_col; i <= max_col; ++i) {
    Coord to_find = {min_row, i};
    Coord poss_start = {min_row + 1, i};
    bool is_wall = find(walls.begin(), walls.end(), to_find) != walls.end();
    bool is_inside = find(walls.begin(), walls.end(), poss_start) == walls.end();
    if (is_wall && is_inside) {
      start = poss_start;
      break;
    }
  }
  return FloodFill(start, walls) + walls.size();
}

// Shoelace formula
long GetArea(vector<Coord> vertices) {
  long running_sum = 0;
  for (int i = 0; i < vertices.size(); ++i) {
    long xy = vertices[i].first * vertices[(i+1) % vertices.size()].second;
    long yx = vertices[i].second * vertices[(i+1) % vertices.size()].first;
    running_sum += xy - yx;
  }
  return abs(running_sum/2);
}

long PartTwo(vector<Coord> vertices, long total_length) {
  // This is actually picks theorem (solved for (I+B) instead of A) but I derived it
  // myself through just looking at how far GetArea was off from the example's solution
  // Found out about Picks Theorem after finishing part 2 and looking through wikipedia
  return GetArea(vertices) + total_length/2 + 1; 
}

int main() {
  auto [walls, vertices, total_length] = GetInfo();
  int answer_one = PartOne(walls);
  long answer_two = PartTwo(vertices, total_length);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
