#include "utils.h"

typedef pair<int,int> Coord;

tuple<Coord, vector<Coord>, int, int> GetInfo() {
  vector<Coord> garden;
  Coord start;
  ifstream input("input.txt");
  string line;
  int rows = 0;
  int cols = 0;
  while (getline(input, line)) {
    cols = 0;
    for (char c : line) {
      if (c != '#') {
        garden.emplace_back(rows, cols);
      }
      if (c == 'S') {
        start = {rows, cols};
      }
      ++cols;
    }
    ++rows;
  }
  return {start, garden, rows, cols};
}

vector<Coord> GetNeighbors(Coord coord, const vector<Coord>& garden) {
  vector<Coord> dirs = {{0,1},{0,-1},{1,0},{-1,0}};
  vector<Coord> neighbors;
  for (const Coord& dir : dirs) {
    Coord neighbor = {coord.first + dir.first, coord.second + dir.second};
    Coord wrapped_neighbor = {coord.first + dir.first, coord.second + dir.second};
    if (find(garden.begin(), garden.end(), neighbor) != garden.end()) {
      neighbors.push_back(neighbor);
    }
  }
  return neighbors;
}

int BFS(Coord start, Coord goal, const map<Coord, vector<Coord>>& garden_map) {
  // Int is for keeping track of steps from start
  queue<pair<Coord,int>> to_visit;
  set<Coord> explored;
  to_visit.emplace(start, 0);
  explored.insert(start);
  while (!to_visit.empty()) {
    pair<Coord,int> curr_node_info = to_visit.front();
    Coord curr_node = curr_node_info.first;
    int distance = curr_node_info.second;
    if (curr_node == goal) {
      return distance;
    }
    to_visit.pop();
    for (const Coord& neighbor : garden_map.at(curr_node)) {
      if (neighbor == goal) {
        return distance + 1;
      }
      if (!explored.count(neighbor)) {
        to_visit.emplace(neighbor, distance + 1);
        explored.insert(neighbor);
      }
    }
  }
  return -1;
}

// Returns map of coord and their minimum distances from the start
int GetStepCount(Coord start, const map<Coord, vector<Coord>>& garden_map, int steps = 64) {
  set<Coord> to_visit;
  set<Coord> visited;
  visited.insert(start);
  for (int i = 0; i < steps; ++i) {
    for (auto iter = visited.begin(); iter != visited.end(); ++iter) {
      Coord curr_coord = *iter;
      for (const Coord& coord : garden_map.at(curr_coord)) {
        to_visit.insert(coord);
      }
    }
    visited.clear();
    visited.swap(to_visit);
  }
  return visited.size();
}

map<Coord,int> GetAllDistances(Coord start, const map<Coord, vector<Coord>>& garden_map) {
  map<Coord,int> all_distances;
  for (const auto& node : garden_map) {
    int distance = BFS(start, node.first, garden_map);
    if (distance != -1) {
      all_distances[node.first] = distance;
    }
  }
  return all_distances;
}

int FindStablePoint(Coord start, const map<Coord, vector<Coord>>& garden_map) {
  set<Coord> to_visit;
  set<Coord> visited;
  int steps = 270;
  visited.insert(start);
  for (int i = 0; i < steps; ++i) {
    for (auto iter = visited.begin(); iter != visited.end(); ++iter) {
      Coord curr_coord = *iter;
      for (const Coord& coord : garden_map.at(curr_coord)) {
        to_visit.insert(coord);
      }
    }
    visited.clear();
    visited.swap(to_visit);
    cout << visited.size() << " possible locations on step " << (i + 1) << endl;
  }
  return visited.size();
}

int PartOne(map<Coord,int> all_distances) {
  int answer = 0;
  for (const auto& node : all_distances) {
    if (node.second <= 64 && node.second % 2 == 0) {
      ++answer;
    }
  } 
  return answer;
}

// This solution is inspired by a particular post from Reddit
// as I could not come up with how to utilize the patterns in the data to find a solution
// and this is a mathematically elegant way to solve it.
long PartTwo(map<Coord,int> all_distances, int rows) {
  long answer = 0;
  int steps = 26501365;
  int n = steps / rows;
  cout << n << endl;
  int even_corner_count = 0;
  int odd_corner_count = 0;
  int even_square_count = 0;
  int odd_square_count = 0;
  for (const auto& node : all_distances) {
    int dist = node.second;
    if (dist > 65 && dist % 2 == 0) {
      ++even_corner_count;
    } else if (dist > 65 && dist % 2 == 1) {
      ++odd_corner_count;
    }
    if (dist % 2 == 0) {
      ++even_square_count;
    } else if(dist % 2 == 1) {
      ++odd_square_count;
    }
  } 
  answer = (long)(n+1)*(n+1)*odd_square_count + (long)n*n*even_square_count - (long)(n+1)*odd_corner_count + (long)n*even_corner_count;
  return answer;
}

int main() {
  auto [start, garden, rows, cols] = GetInfo();
  map<Coord, vector<Coord>> garden_map;
  for (const Coord& coord : garden) {
    garden_map[coord] = GetNeighbors(coord, garden);
  }
  map<Coord,int> all_distances = GetAllDistances(start, garden_map);
  int answer_one = PartOne(all_distances);
  long answer_two = PartTwo(all_distances, rows);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
