#include "../utils.h"

using aoc::split;
using aoc::GetInts;

vector<pair<int,int>> GetInfo() {
  vector<pair<int,int>> race_info;
  ifstream input("input.txt");
  string line;
  getline(input, line);
  vector<int> times = GetInts(split(split(line, ":")[1], " "));
  getline(input, line);
  vector<int> distances = GetInts(split(split(line, ":")[1], " "));
  for (int i = 0; i < times.size(); ++i) {
    race_info.emplace_back(times[i], distances[i]);
  }
  return race_info;
}

int CalculateWinNum(int time, long distance) {
  int root1 = ceil(0.5 * (time - sqrt(pow(time, 2) - 4 * distance)));
  int root2 = floor(0.5 * (time + sqrt(pow(time, 2) - 4 * distance)));
  // Make sure the solutions to the roots arent the actual distance
  // since we want to beat it, not match it
  if (root1 * (time - root1) == distance)
    root1++;
  if (root2 * (time - root2) == distance)
    root2--;
  return root2 - root1 + 1;
}

int PartOne(vector<pair<int,int>> race_info) {
  int answer = 1;
  for (pair<int,int> race : race_info) {
    answer *= CalculateWinNum(race.first, race.second);
  }
  return answer;
}

int PartTwo(vector<pair<int,int>> race_info) {
  string time_str;
  string distance_str;
  for (pair<int,int> race : race_info) {
    time_str += std::to_string(race.first);
    distance_str += std::to_string(race.second);
  }
  int time = stoi(time_str);
  long distance = stol(distance_str);
  return CalculateWinNum(time, distance);
}

int main() {
  vector<pair<int,int>> race_info = GetInfo();
  int answer_one = PartOne(race_info);
  int answer_two = PartTwo(race_info);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
