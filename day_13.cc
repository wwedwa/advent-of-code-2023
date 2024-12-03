#include "utils.h"

vector<vector<string>> GetInfo() {
  ifstream input("input.txt");
  vector<vector<string>> mirrors;
  string line;
  vector<string> mirror;
  while (getline(input, line)) {
    if (line.empty()) {
      mirrors.push_back(mirror);
      mirror.clear();
    } else {
      mirror.push_back(line);
    }
  }
  mirrors.push_back(mirror);
  return mirrors;
}

vector<int> GetHorizReflection(const vector<string>& mirror) {
  vector<int> rows_above;
  for (int i = 1; i < mirror.size(); ++i) {
    bool match = true;
    for (int j = 1; (i - j >= 0) && (i + j - 1 < mirror.size()); ++j) {
      match = mirror[i - j] == mirror[i + j - 1];
      if (!match) {
        break;
      }
    }
    if (match * i) {
      rows_above.push_back(match * i);
    }
  }
  if (rows_above.empty()) {
    rows_above.push_back(0);
  }
  return rows_above;
}

vector<int> GetVertReflection(const vector<string>& mirror) {
  vector<string> mirror_transpose(mirror[0].length(), string(mirror.size(), '.'));
  for (int i = 0; i < mirror.size(); ++i) {
    for (int j = 0; j < mirror[i].length(); ++j) {
      mirror_transpose[j][i] = mirror[i][j];
    }
  }
  return GetHorizReflection(mirror_transpose);
}

int HandleSmudge(vector<string> mirror) {
  int orig_h_reflection = GetHorizReflection(mirror)[0];
  int orig_v_reflection = GetVertReflection(mirror)[0];
  for (int i = 0; i < mirror.size(); ++i) {
    for (int j = 0; j < mirror[i].length(); ++j) {
      mirror[i][j] = (mirror[i][j] == '.') ? '#' : '.';
      vector<int> new_h_reflection = GetHorizReflection(mirror);
      vector<int> new_v_reflection = GetVertReflection(mirror);
      // Remove orig reflection from new reflection vector. Then return if there is still a valid reflection left
      auto orig_h_loc = find(new_h_reflection.begin(), new_h_reflection.end(), orig_h_reflection);
      if (orig_h_loc != new_h_reflection.end()) {
        new_h_reflection.erase(orig_h_loc);
      }
      if (!new_h_reflection.empty() && new_h_reflection[0] != 0) {
        return new_h_reflection[0] * 100;
      }
      auto orig_v_loc = find(new_v_reflection.begin(), new_v_reflection.end(), orig_v_reflection);
      if (orig_v_loc != new_v_reflection.end()) {
        new_v_reflection.erase(orig_v_loc);
      }
      if (!new_v_reflection.empty() && new_v_reflection[0] != 0) {
        return new_v_reflection[0];
      }
      mirror[i][j] = (mirror[i][j] == '.') ? '#' : '.'; // Switch symbol back
    }
  }
  return 0;
}

int PartOne(const vector<vector<string>>& mirrors) {
  int answer = 0;
  for (const vector<string>& mirror : mirrors) {
    answer += GetVertReflection(mirror)[0] + 100 * GetHorizReflection(mirror)[0];
  }
  return answer;
}

int PartTwo(const vector<vector<string>>& mirrors) {
  int answer = 0;
  for (const vector<string>& mirror : mirrors) {
    answer += HandleSmudge(mirror);
  }
  return answer;
}

int main() {
  vector<vector<string>> mirrors = GetInfo();
  int answer_one = PartOne(mirrors);
  int answer_two = PartTwo(mirrors);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
