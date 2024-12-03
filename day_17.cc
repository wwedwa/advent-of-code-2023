#include "utils.h"

using std::get;

typedef pair<int,int> Coord;
typedef tuple<Coord,int, int> Node; // (coordinates, direction, length travelled in curr_direction)

vector<vector<int>> GetInfo() {
  vector<vector<int>> int_map;
  map<Node, map<Node, int>> city_map;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    vector<int> map_line;
    for (char c : line) {
      map_line.push_back(c - '0');
    }
    int_map.push_back(map_line);
  }
  return int_map;
}

int Heuristic(Coord curr_loc, Coord end) {
  return abs(curr_loc.first - end.first) + abs(curr_loc.second - end.second);
}

int AStarSearch(const vector<vector<int>> city_map, Coord start, Coord goal, int shortest_path, int longest_path) {
  vector<Coord> dirs = {{0,1}, {1,0}, {0,-1}, {-1,0}};
  Node start_node = {start, 0, 0};
  set<pair<int, Node>> open_set;
  map<Node, int> g_score; // Distances from curr_node to start
  g_score[start_node] = 0;
  map<Node, int> f_score; // g + h
  f_score[start_node] = Heuristic(start, goal);
  map<Node, Node> came_from;
  open_set.insert({f_score[start_node], start_node});
  while(!open_set.empty()) {
    pair<int, Node> current = *open_set.begin();
    Coord coord = get<0>(current.second);
    int dir = get<1>(current.second);
    if (coord == goal) {
      return current.first;
    }
    open_set.erase(open_set.begin());
    int poss_dirs[2];
    poss_dirs[0] = (current.second == start_node) ? 0 : (dir + 3) % 4;
    poss_dirs[1] = (current.second == start_node) ? 1 : (dir + 1) % 4;
    for (int d : poss_dirs) {
      Node prev = current.second;
      for (int i = 1; i < longest_path + 1; ++i) {
        Coord new_dir = dirs[d];
        if (coord.first + i*new_dir.first < 0 ||
            coord.first + i*new_dir.first >= city_map.size() ||
            coord.second + i*new_dir.second < 0 ||
            coord.second + i*new_dir.second >= city_map[0].size()) {
          break;
        }
        Node neighbor = {{coord.first + i*new_dir.first, coord.second + i*new_dir.second}, d, i};
        int tentative_g_score = g_score[prev] + city_map[get<0>(neighbor).first][get<0>(neighbor).second];
        if (!g_score.count(neighbor) || tentative_g_score < g_score[neighbor]) {
          came_from[neighbor] = prev;
          g_score[neighbor] = tentative_g_score;
          f_score[neighbor] = tentative_g_score + Heuristic(get<0>(neighbor), goal);
          if (i >= shortest_path) {
            open_set.insert({f_score[neighbor], neighbor});
          }
        }
        prev = neighbor;
      }
    }
  }
  return -1;
}

int PartOne(vector<vector<int>> city_map) {
  Coord start = {0,0};
  Coord goal = {city_map.size() - 1, city_map[0].size() - 1};
  return AStarSearch(city_map, start, goal, 1, 3);
}

int PartTwo(vector<vector<int>> city_map) {
  Coord start = {0,0};
  Coord goal = {city_map.size() - 1, city_map[0].size() - 1};
  return AStarSearch(city_map, start, goal, 4, 10);
}

int main() {
  vector<vector<int>> city_map = GetInfo();
  int answer_one = PartOne(city_map);
  int answer_two = PartTwo(city_map);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
