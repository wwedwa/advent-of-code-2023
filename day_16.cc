#include "utils.h"

typedef pair<int,int> Coord;

vector<string> GetInfo() {
  vector<string> mirror_map;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    mirror_map.push_back(line);
  }
  return mirror_map;
}

int PartOne(const vector<string>& mirror_map, Coord loc = {0,-1}, Coord dir = {0,1}) {
  list<pair<Coord, Coord>> curr_beams; // first coord is loc, second is dir
  vector<pair<Coord, Coord>> prev_beams; // to avoid looping
  curr_beams.push_back({loc, dir});
  set<Coord> energized_locs;
  while (!curr_beams.empty()) {
    for (auto iter = curr_beams.begin(); iter != curr_beams.end(); ++iter) {
      if (find(prev_beams.begin(), prev_beams.end(), (*iter)) != prev_beams.end()) {
        iter = curr_beams.erase(iter);
      } else {
        pair<Coord, Coord>& beam = (*iter);
        prev_beams.push_back(beam);
        energized_locs.insert(beam.first);
        Coord& loc = beam.first;
        Coord& dir = beam.second;
        loc.first += dir.first;
        loc.second += dir.second;
        if (loc.first < 0 || loc.first >= mirror_map.size() || loc.second < 0 || loc.second >= mirror_map[0].length()) {
          iter = curr_beams.erase(iter);
        }
        if (mirror_map[loc.first][loc.second] == '/') {
          int temp = dir.first;
          dir.first = (dir.first == 0) ? -dir.second : 0;
          dir.second = (dir.second == 0) ? -temp : 0;
        } else if (mirror_map[loc.first][loc.second] == '\\') {
          int temp = dir.first;
          dir.first = (dir.first == 0) ? dir.second : 0;
          dir.second = (dir.second == 0) ? temp : 0;
        } else if (mirror_map[loc.first][loc.second] == '|' && dir.second != 0) {
          curr_beams.push_back({loc, {1,0}});
          curr_beams.push_back({loc, {-1,0}});
          iter = curr_beams.erase(iter);
        } else if (mirror_map[loc.first][loc.second] == '-' && dir.first != 0) {
          curr_beams.push_back({loc, {0,1}});
          curr_beams.push_back({loc, {0,-1}});
          iter = curr_beams.erase(iter);
        }
      }
    }
  }
  return energized_locs.size() - 1; //Subtract one since we start off the map (this is bc my input started on a /)
}

int PartTwo(const vector<string>& mirror_map) {
  int answer = -1;
  for (int i = 0; i < mirror_map.size(); ++i) {
    int energized_locs = PartOne(mirror_map, {i, -1}, {0, 1});
    if (energized_locs > answer) {
      answer = energized_locs;
    }
    energized_locs = PartOne(mirror_map, {i, mirror_map[i].length()}, {0, -1});
    if (energized_locs > answer) {
      answer = energized_locs;
    }
  }
  for (int i = 0; i < mirror_map[0].length(); ++i) {
    int energized_locs = PartOne(mirror_map, {-1, i}, {1,0});
    if (energized_locs > answer) {
      answer = energized_locs;
    }
    energized_locs = PartOne(mirror_map, {mirror_map.size(), i}, {-1, 0});
    if (energized_locs > answer) {
      answer = energized_locs;
    }
  }
  return answer;
}

int main() {
  vector<string> mirror_map = GetInfo();
  int answer_one = PartOne(mirror_map);
  int answer_two = PartTwo(mirror_map);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
