#include "utils.h"

using aoc::split;
using aoc::GetInts;

struct Coord {
  int x;
  int y;
  int z;

  Coord() {
    x = y = z = 0;
  }

  Coord(int x_, int y_, int z_) {
    x = x_;
    y = y_;
    z = z_;
  }

  bool operator<(const Coord& c) const {
    if (z != c.z) {
      return z < c.z;
    }
    if (y != c.y) {
      return y < c.y;
    }
    if (x != c.x) {
      return x < c.x;
    }
    return false;
  }
};

struct Brick {
  Coord start;
  Coord end;
  int id;
  int min_x;
  int max_x;
  int min_y;
  int max_y;

  Brick(Coord s, Coord e, int i) {
    if (s.z > e.z) {
      start = e;
      end = s;
    } else {
      start = s;
      end = e;
    }
    id = i;
    min_x = std::min(start.x, end.x);
    max_x = std::max(start.x, end.x);
    min_y = std::min(start.y, end.y);
    max_y = std::max(start.y, end.y);
  }

  bool Fall(const map<pair<int,int>, int>& elevations) {
    for (int x = min_x; x <= max_x; ++x) {
      for (int y = min_y; y <= max_y; ++y) {
        if (elevations.at({x,y}) >= start.z - 1) {
          return false;
        }
      }
    }
    --start.z;
    --end.z;
    return true;
  }

  bool operator<(const Brick& b) const {
    return start < b.start;
  }
};

// Get mins and maxes for x,y so we can initialize elevation map
vector<Brick> GetInfo() {
  vector<Brick> bricks; // Ordered by z, so the ones closest to the ground will be first
  ifstream input("input.txt");
  string line;
  int curr_id = 1;
  while (getline(input, line)) {
    vector<string> coords = split(line, "~");
    vector<int> s = GetInts(split(coords[0], ","));
    vector<int> e = GetInts(split(coords[1], ","));
    Coord start = Coord(s[0], s[1], s[2]);
    Coord end = Coord(e[0], e[1], e[2]);
    bricks.emplace_back(start, end, curr_id);
    ++curr_id;
  }
  return bricks;
}

pair<pair<int,int>, pair<int,int>> GetMinsMaxes(const vector<Brick>& bricks) {
  pair<int,int> x_min_max = {INT_MAX, -1};
  pair<int,int> y_min_max = {INT_MAX, -1};
  for (const Brick& brick : bricks) {
    if (brick.min_x < x_min_max.first)
      x_min_max.first = brick.min_x;
    if (brick.max_x > x_min_max.second)
      x_min_max.second = brick.max_x;
    if (brick.min_y < y_min_max.first)
      y_min_max.first = brick.min_y;
    if (brick.max_y > y_min_max.second)
      y_min_max.second = brick.max_y;
  }
  return {x_min_max, y_min_max};
}

int ChainReaction(const unordered_map<int, unordered_set<int>>& supported_by, const unordered_map<int, unordered_set<int>>& supporting, int to_remove) {
  queue<int> to_check;
  unordered_set<int> to_fall;
  if (!supporting.count(to_remove)) {
    return 0;
  }
  for (int id : supporting.at(to_remove)) {
    to_check.push(id);
  }
  to_fall.insert(to_remove);
  while (!to_check.empty()) {
    int falling_support_count = 0;
    to_remove = to_check.front();
    to_check.pop();
    for (int support : supported_by.at(to_remove)) {
      if (to_fall.count(support)) {
      }
      falling_support_count += to_fall.count(support);
    }
    if (falling_support_count == supported_by.at(to_remove).size()) {
      to_fall.insert(to_remove);
    }
    if (supporting.count(to_remove)) {
      for (int supported : supporting.at(to_remove)) {
        to_check.push(supported);
      }
    }
  }
  return to_fall.size() - 1; //-1 since to_remove actually isn't included in the count
}

int PartOne(vector<Brick> bricks, pair<int,int> x_range, pair<int,int> y_range) {
  std::sort(bricks.begin(), bricks.end()); // Sort so lower blocks fall first
  map<pair<int,int>, int> elevations;
  map<Coord, int> settled_bricks;
  unordered_map<int, unordered_set<int>> supported_by;
  for (int x = x_range.first; x <= x_range.second; ++x) {
    for (int y = y_range.first; y <= y_range.second; ++y) {
      elevations[{x,y}] = 0;
    }
  }
  bool still_falling = true;
  while (still_falling) {
    still_falling = false;
    for (Brick& brick : bricks) {
      if (brick.Fall(elevations)) {
        still_falling = true;
      } else {
        for (int x = brick.min_x; x <= brick.max_x; ++x) {
          for (int y = brick.min_y; y <= brick.max_y; ++y) {
            elevations[{x,y}] = brick.end.z;
            for (int z = brick.start.z; z <= brick.end.z; ++z) {
              Coord coord(x,y,z);
              settled_bricks[coord] = brick.id;
            }
            Coord under(x,y,brick.start.z-1);
            if (settled_bricks.count(under)) {
              supported_by[brick.id].insert(settled_bricks[under]);
            }
          }
        }
      }
    }
  }
  unordered_set<int> load_bearing_ids;
  for (const Brick& brick : bricks) {
    unordered_set<int> supporting_ids = supported_by[brick.id];
    if (supporting_ids.size() == 1) {
      load_bearing_ids.insert(*supporting_ids.begin());
    }
  }
  return bricks.size() - load_bearing_ids.size();
}

int PartTwo(vector<Brick> bricks, pair<int,int> x_range, pair<int,int> y_range) {
  std::sort(bricks.begin(), bricks.end()); // Sort so lower blocks fall first
  map<pair<int,int>, int> elevations;
  map<Coord, int> settled_bricks;
  unordered_map<int, unordered_set<int>> supported_by;
  for (int x = x_range.first; x <= x_range.second; ++x) {
    for (int y = y_range.first; y <= y_range.second; ++y) {
      elevations[{x,y}] = 0;
    }
  }
  bool still_falling = true;
  while (still_falling) {
    still_falling = false;
    for (Brick& brick : bricks) {
      if (brick.Fall(elevations)) {
        still_falling = true;
      } else {
        for (int x = brick.min_x; x <= brick.max_x; ++x) {
          for (int y = brick.min_y; y <= brick.max_y; ++y) {
            elevations[{x,y}] = brick.end.z;
            for (int z = brick.start.z; z <= brick.end.z; ++z) {
              Coord coord(x,y,z);
              settled_bricks[coord] = brick.id;
            }
            Coord under(x,y,brick.start.z-1);
            if (settled_bricks.count(under)) {
              supported_by[brick.id].insert(settled_bricks[under]);
            }
          }
        }
      }
    }
  }
  unordered_map<int, unordered_set<int>> supporting;
  for (const Brick& brick : bricks) {
    if (supported_by.count(brick.id)) {
      for (int id : supported_by[brick.id]) {
        supporting[id].insert(brick.id);
      }
    } else {
      // 0 represents ground, so if something isnt supported by any blocks, its on the ground
      supported_by[brick.id].insert(0);
    }
  }
  int answer = 0;
  for (const Brick& brick : bricks) {
    answer += ChainReaction(supported_by, supporting, brick.id);
  }
  return answer;
}

int main() {
  vector<Brick> bricks = GetInfo();
  pair<pair<int,int>, pair<int,int>> mins_maxes = GetMinsMaxes(bricks);
  int answer_one = PartOne(bricks, mins_maxes.first, mins_maxes.second);
  int answer_two = PartTwo(bricks, mins_maxes.first, mins_maxes.second);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
