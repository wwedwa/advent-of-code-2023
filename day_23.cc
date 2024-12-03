#include "../utils.h"

typedef pair<int,int> Coord;

map<Coord, vector<Coord>> neighbor_cache;

tuple<vector<Coord>, map<Coord,char>, Coord, Coord> GetInfo() {
  vector<Coord> open_space;
  map<Coord, char> slopes;
  Coord start;
  Coord end;
  ifstream input("input.txt");
  string line;
  int row = 0;
  while (getline(input, line)) {
    int col = 0;
    for (char c : line) {
      if (c == '.') {
        open_space.emplace_back(row, col);
        if (row == 0) {
          start = {row, col};
        }
        end = {row, col};
      } else if (c != '#') {
        slopes[{row,col}] = c;
      }
      ++col;
    }
    ++row;
  }
  return {open_space, slopes, start, end};
}

vector<Coord> GetNeighbors(const vector<Coord>& open_space, const map<Coord,char>& slopes, Coord node, bool check_slope = true) {
  vector<Coord> dirs = {{0,1}, {0,-1}, {1,0}, {-1,0}};
  if (slopes.count(node) && check_slope) {
    char dir = slopes.at(node);
    if (dir == '>')
      return {{node.first, node.second + 1}};
    if (dir == 'v')
      return {{node.first + 1, node.second}};
  }
  vector<Coord> neighbors;
  for (const Coord& dir : dirs) {
    Coord neighbor = {node.first + dir.first, node.second + dir.second};
    if (find(open_space.begin(), open_space.end(), neighbor) != open_space.end() || slopes.count(neighbor)) {
      neighbors.push_back(neighbor);
    }
  }
  return neighbors;
}

vector<Coord> GetDirs(const vector<Coord>& open_space, Coord node) {
  vector<Coord> dirs = {{0,1}, {0,-1}, {1,0}, {-1,0}};
  vector<Coord> poss_dirs = vector<Coord>();
  vector<Coord> neighbors;
  for (const Coord& dir : dirs) {
    Coord neighbor = {node.first + dir.first, node.second + dir.second};
    if (find(open_space.begin(), open_space.end(), neighbor) != open_space.end()) {
      poss_dirs.push_back(dir);
    }
  }
  return poss_dirs;
}

void MaxPathRecursive(const vector<Coord>& open_space, const map<Coord,char>& slopes, deque<Coord>& visited, Coord node, Coord end, int length, int& max) {
  visited.push_back(node);
  if (node == end) {
    max = std::max(max, length);
    visited.pop_back();
    return;
  }
  ++length; // Increment after check since we want to count steps, not visited squares
  if (!neighbor_cache.count(node)) {
    neighbor_cache[node] = GetNeighbors(open_space, slopes, node);
  }
  for (Coord neighbor : neighbor_cache[node]) {
    if (find(visited.begin(), visited.end(), neighbor) == visited.end()) {
      MaxPathRecursive(open_space, slopes, visited, neighbor, end, length, max);
    }
  }
  visited.pop_back();
}

int MaxPath(const vector<Coord>& open_space, const map<Coord,char>& slopes, Coord start, Coord end) {
  deque<Coord> visited;
  int max = -1;
  int length = 0;
  MaxPathRecursive(open_space, slopes, visited, start, end, length, max);
  return max;
}

// Returns the next intersection, and the distance to it
pair<Coord, int> FindNextIntersection(vector<Coord> open_space, Coord node, Coord dir) {
  vector<Coord> dirs;
  dirs.push_back(dir);
  int steps = 0;
  while (dirs.size() == 1) {
    dir = dirs[0];
    Coord opp_dir = {-dir.first, -dir.second};
    node = {node.first + dir.first, node.second + dir.second};
    ++steps;
    dirs = GetDirs(open_space, node);
    auto iter = find(dirs.begin(), dirs.end(), opp_dir);
    if (iter != dirs.end()) {
      dirs.erase(iter);
    }
  }
  return {node, steps};
}

int PartOne(const vector<Coord>& open_space, const map<Coord,char>& slopes, Coord start, Coord end) {  
  int answer = MaxPath(open_space, slopes, start, end);
  return answer;
}

// For finding the max path with the intersections
void IntersMaxPathRecursive(const map<Coord, map<Coord,int>>& intersections, deque<Coord>& visited, Coord node, Coord end, int length, int& max) {
  visited.push_back(node);
  if (node == end) {
    max = std::max(max, length);
    visited.pop_back();
    return;
  }
  for (const auto& inter : intersections.at(node)) {
    if (find(visited.begin(), visited.end(), inter.first) == visited.end()) {
      length += inter.second;
      IntersMaxPathRecursive(intersections, visited, inter.first, end, length, max);
      length -= inter.second;
    }
  }
  visited.pop_back();
}

int IntersMaxPath(const map<Coord, map<Coord,int>>& intersections, Coord start, Coord end) {
  deque<Coord> visited;
  int max = -1;
  int length = 0;
  IntersMaxPathRecursive(intersections, visited, start, end, length, max);
  return max;
}

int PartTwo(vector<Coord> open_space, map<Coord,char> slopes, Coord start, Coord end) {
  for (const auto& slope : slopes) {
    open_space.push_back(slope.first);
    neighbor_cache[slope.first] = GetNeighbors(open_space, slopes, slope.first, false);
  }
  slopes.clear();
  map<Coord, map<Coord,int>> intersections;
  deque<Coord> to_visit;
  to_visit.push_back(start);
  while (!to_visit.empty()) {
    Coord intersection = to_visit.front();
    to_visit.pop_front();
    vector<Coord> dirs = GetDirs(open_space, intersection);
    for (const Coord& dir : dirs) {
      pair<Coord,int> next_i = FindNextIntersection(open_space, intersection, dir);
      if (!intersections[intersection].count(next_i.first) || intersections[intersection][next_i.first] < next_i.second) {
        intersections[intersection][next_i.first] = next_i.second;
      }
      if (!intersections.count(next_i.first) && next_i.first != intersection && find(to_visit.begin(), to_visit.end(), next_i.first) == to_visit.end()) {
        to_visit.push_back(next_i.first);
      }
    }
  }
  return IntersMaxPath(intersections, start, end);
}

int main() {
  auto [open_space, slopes, start, end] = GetInfo();
  int answer_one = PartOne(open_space, slopes, start, end);
  int answer_two = PartTwo(open_space, slopes, start, end);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
