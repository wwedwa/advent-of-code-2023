#include "utils.h"

using aoc::split;

vector<pair<vector<string>, vector<string>>> GetInfo() {
  ifstream input("input.txt");
  string line;
  vector<pair<vector<string>, vector<string>>> ticket_nums;
  while (getline(input, line)) {
    string all_nums = split(line, ": ")[1];
    vector<string> split_nums = split(all_nums, " | ");
    vector<string> win_nums = split(split_nums[0], " ");
    vector<string> elf_nums = split(split_nums[1], " ");
    ticket_nums.emplace_back(win_nums, elf_nums);
  }
  return ticket_nums;
}

int PartOne(vector<pair<vector<string>, vector<string>>> ticket_nums) {
  int answer = 0;
  for (auto num_pair : ticket_nums) {
    int matching = 0;
    for (string win_num : num_pair.first) {
      auto iterator = find(num_pair.second.begin(), num_pair.second.end(), win_num);
      if (iterator != num_pair.second.end()) {
        ++matching;
      }
    }
    if (matching) {
      answer += pow(2, matching - 1);
    }
  }
  return answer;
}

int PartTwo(vector<pair<vector<string>, vector<string>>> ticket_nums) {
  int answer = 0;
  unordered_map<int, int> ticket_count;
  // Create map to store the number of tickets we have
  for (int i = 0; i < ticket_nums.size(); ++i) {
    ticket_count[i] = 1;
  }
  for (int i = 0; i < ticket_nums.size(); ++i) {
    int matching = 0;
    for (string win_num : ticket_nums[i].first) {
      auto iterator = find(ticket_nums[i].second.begin(), ticket_nums[i].second.end(), win_num);
      if (iterator != ticket_nums[i].second.end()) {
        ++matching;
      }
    }
    for (int j = i + 1; j < i + 1 + matching; ++j) {
      ticket_count[j] += ticket_count[i];
    }
  }
  for (int i = 0; i < ticket_nums.size(); ++i) {
    answer += ticket_count[i];
  }
  return answer;
}

int main() {
  auto ticket_nums = GetInfo();
  int answer_one = PartOne(ticket_nums);
  int answer_two = PartTwo(ticket_nums);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
