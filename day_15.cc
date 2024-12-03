#include "../utils.h"

using aoc::split;

vector<string> GetInfo() {
  ifstream input("input.txt");
  string line;
  getline(input, line);
  return split(line, ",");
}

int Hash(string value) {
  int curr_val = 0;
  for (char c : value) {
    curr_val += c;
    curr_val *= 17;
    curr_val %= 256;
  }
  return curr_val;
}

int PartOne(const vector<string>& values) {
  int answer = 0;
  for (const string& val : values) {
    answer += Hash(val);
  }
  return answer;
}

int PartTwo(const vector<string>& values) {
  int answer = 0;
  unordered_map<int, vector<pair<int, string>>> boxes;
  for (const string& val : values) {
    if (val.find('=') != string::npos) {
      vector<string> label_lense = split(val, "=");
      string label = label_lense[0];
      int lense_num = stoi(label_lense[1]);
      int box_num = Hash(label);
      bool found = false;
      for (pair<int, string>& lense : boxes[box_num]) {
        if (lense.second == label) {
          lense.first = lense_num;
          found = true;
          break;
        }
      }
      if (!found) {
        boxes[box_num].emplace_back(lense_num, label);
      }
    } else {
      string label = val.substr(0, val.length() - 1);
      int box_num = Hash(label);
      auto iter = boxes[box_num].begin();
      for (; iter != boxes[box_num].end(); ++iter) {
        if ((*iter).second == label) {
          boxes[box_num].erase(iter);
          break;
        }
      }
    }
  }
  for (const auto& box : boxes) {
    int lense_slot = 1;
    for (const pair<int,string>& lense : box.second) {
      answer += lense_slot * (box.first + 1) * lense.first;
      lense_slot++;
    }
  }
  return answer;
}

int main() {
  vector<string> values = GetInfo();
  int answer_one = PartOne(values);
  int answer_two = PartTwo(values);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
