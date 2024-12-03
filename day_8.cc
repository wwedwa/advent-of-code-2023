#include "../utils.h"

pair<string, unordered_map<string, pair<string,string>>> GetInfo() {
  unordered_map<string, pair<string,string>> network;
  string dirs;
  ifstream input("input.txt");
  getline(input, dirs);
  string line;
  getline(input, line); // skip whitespace
  while (getline(input, line)) {
    string curr_node = line.substr(0,3);
    string left = line.substr(7,3);
    string right = line.substr(12,3);
    network[curr_node] = {left, right};
  }
  return {dirs, network};
}

int PartOne(string dirs, unordered_map<string, pair<string,string>> network) {
  int answer = 0;
  string curr_node = "AAA";
  while (curr_node != "ZZZ") {
    char dir = dirs[answer % dirs.length()];
    if (dir == 'L') {
      curr_node = network[curr_node].first;
    } else if (dir == 'R') {
      curr_node = network[curr_node].second;
    }
    ++answer;
  }
  return answer;
}

int FindLoopLength(string node, string dirs, unordered_map<string, pair<string,string>> network) {
  int answer = 0;
  string curr_node = node;
  while (curr_node[2] != 'Z') {
    char dir = dirs[answer % dirs.length()];
    if (dir == 'L') {
      curr_node = network[curr_node].first;
    } else if (dir == 'R') {
      curr_node = network[curr_node].second;
    }
    ++answer;
  }
  return answer;
}

// Using the fact that the steps from a starting node to an ending node is the same
// number of steps it takes to reach that ending node a second time (and third time, fourth time, etc.)
long PartTwo(string dirs, unordered_map<string, pair<string,string>> network) {
  vector<int> cycle_lengths;
  for (const auto& node_info : network) {
    if (node_info.first[2] == 'A') {
      cycle_lengths.push_back(FindLoopLength(node_info.first, dirs, network));
    }
  }
  return aoc::lcm(cycle_lengths);
}

int main() {
  auto [dirs, network] = GetInfo();
  int answer_one = PartOne(dirs, network);
  long answer_two = PartTwo(dirs, network);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
