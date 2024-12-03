#include "utils.h"

tuple<vector<string>, unordered_map<int, bool>, unordered_map<int, bool>> GetInfo() {
  vector<string> universe;
  unordered_map<int, bool> empty_cols;
  unordered_map<int, bool> empty_rows;
  ifstream input("input.txt");
  string line;
  int row_idx = 0;
  while (getline(input, line)) {
    universe.push_back(line);
    for (int i = 0; i < line.length(); ++i) {
      if (empty_cols.count(i)) {
        empty_cols[i] = empty_cols[i] && line[i] == '.';
      } else {
        empty_cols[i] = line[i] == '.';
      }
    }
    empty_rows[row_idx] = line.find('#') == string::npos;
    ++row_idx;
  }
  return {universe, empty_rows, empty_cols};
}

void AddEmptyCols(vector<string>& universe, const unordered_map<int, bool>& empty_cols) {
  int length = universe[0].length();
  for (size_t i = 0; i < universe.size(); ++i) {
    int inserted_num = 0;
    for (size_t j = 0; j < length; ++j) {
      if (empty_cols.at(j)) {
        universe[i].insert(j + inserted_num, ".");
        ++inserted_num;
      }
    }
  }
}

vector<pair<long,long>> GetGalaxies(const vector<string>& universe) {
  int length = universe[0].length();
  vector<pair<long,long>> galaxies;
  for (size_t i = 0; i < universe.size(); ++i) {
    for (size_t j = 0; j <length; ++j) {
      if (universe[i][j] == '#') {
        galaxies.emplace_back(i, j);
      }
    }
  }
  return galaxies;
}

long SumDistances(const vector<pair<long,long>>& galaxies) {
  long distances = 0;
  for (size_t i = 0; i < galaxies.size() - 1; ++i) {
    for (size_t j = i + 1; j < galaxies.size(); ++j) {
      pair<int,int> loc1 = galaxies[i];
      pair<int,int> loc2 = galaxies[j];
      distances += abs(loc1.first - loc2.first) + abs(loc1.second - loc2.second);
    }
  }
  return distances;
}

vector<pair<long, long>> ScaleGalaxies(vector<pair<long,long>> galaxies,
                                      const unordered_map<int, bool>& empty_rows,
                                      const unordered_map<int, bool>& empty_cols,
                                      int scale_factor) {
  for (int i = 0; i < galaxies.size(); ++i) {
    int empty_col_num = 0;
    for (int j = 0; j < galaxies[i].second; ++j) {
      empty_col_num += empty_cols.at(j);
    }
    galaxies[i].second += (scale_factor - 1) * empty_col_num;
  }
  for (int i = 0; i < galaxies.size(); ++i) {
    int empty_row_num = 0;
    for (int j = 0; j < galaxies[i].first; ++j) {
      empty_row_num += empty_rows.at(j);
    }
    galaxies[i].first += (scale_factor - 1) * empty_row_num;
  }
  return galaxies;
}

long PartOne(vector<pair<long,long>> galaxies,
            const unordered_map<int, bool>& empty_rows,
            const unordered_map<int, bool>& empty_cols) {
  vector<pair<long,long>> scaled_galaxies = ScaleGalaxies(galaxies, empty_rows, empty_cols, 2);
  return SumDistances(scaled_galaxies);
}

long PartTwo(vector<pair<long,long>> galaxies,
            const unordered_map<int, bool>& empty_rows,
            const unordered_map<int, bool>& empty_cols) {
  vector<pair<long,long>> scaled_galaxies = ScaleGalaxies(galaxies, empty_rows, empty_cols, 1000000);
  return SumDistances(scaled_galaxies);
}

int main() {
  auto [universe, empty_rows, empty_cols] = GetInfo();
  vector<pair<long,long>> galaxies = GetGalaxies(universe);
  long answer_one = PartOne(galaxies, empty_rows, empty_cols);
  long answer_two = PartTwo(galaxies, empty_rows, empty_cols);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
