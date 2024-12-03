#include "utils.h"
#include<random>
#include<time.h>

using aoc::split;

struct SuperNode {
  string node;

  SuperNode() {}

  SuperNode(string n) {
    node = n;
  }

  bool operator==(const SuperNode& super_node) const {
    return node == super_node.node;
  }

  bool operator!=(const SuperNode& super_node) const {
    return node != super_node.node;
  }

  bool operator<(const SuperNode& super_node) const {
    return node < super_node.node;
  }

  string toString() const {
    return node;
  }
};

typedef pair<SuperNode,SuperNode> Edge;

pair<vector<SuperNode>, vector<Edge>> GetInfo() {
  vector<SuperNode> wires;
  vector<Edge> edges;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    vector<string> info = split(line, ": ");
    SuperNode new_node(info[0]);
    if (find(wires.begin(), wires.end(), new_node) == wires.end()) {
      wires.push_back(new_node);
    }
    vector<string> connections = split(info[1], " ");
    for (string node : connections) {
      SuperNode connection(node);
      if (find(wires.begin(), wires.end(), connection) == wires.end()) {
        wires.push_back(connection);
      }
      Edge edge1 = {new_node, connection};
      Edge edge2 = {connection, new_node};
      if (find(edges.begin(), edges.end(), edge1) == edges.end() && find(edges.begin(), edges.end(), edge2) == edges.end()) {
        edges.push_back(edge1);
      }
    }
  }
  return {wires, edges};
}

void Join(map<SuperNode, string>& parents, string parent1, string parent2) {
  string parent = min(parent1, parent2);
  for (auto& node : parents) {
    if (node.second == parent1 || node.second == parent2) {
      node.second = parent;
    }
  }
}

pair<map<SuperNode, string>, int> KargersAlgorithm(vector<SuperNode> wires, vector<Edge> edges) {
  int size = wires.size();
  map<SuperNode, string> parents;
  for (SuperNode node : wires) {
    parents[node] = node.node;
  }
  while (size > 2) {
    int idx = rand() % edges.size();
    Edge edge = edges[idx];
    if (parents[edge.first] == parents[edge.second]) {
      continue;
    }
    Join(parents, parents[edge.first], parents[edge.second]);
    --size;
    vector<SuperNode> new_connections;
  }
  int answer = 0;
  for (Edge edge : edges) {
    if (parents[edge.first] != parents[edge.second]) {
      ++answer;
    }
  }
  return {parents, answer};
}

int PartOne(vector<SuperNode> wires, vector<Edge> edges) {
  map<SuperNode, string> parents;
  int edge_count = 4;
  while (edge_count > 3) {
    auto info = KargersAlgorithm(wires, edges);
    parents = info.first;
    edge_count = info.second;
  }
  string parent1 = parents[wires[0]];
  int count1 = 0;
  int count2 = 0;
  for (auto node : parents) {
    if (node.second == parent1) {
      ++count1;
    } else {
      ++count2;
    }
  }
  return count1 * count2;
}

string PartTwo() {
  return "MERRY CHRISTMAS!";
}

int main() {
  srand (time(0));
  auto [wires, edges] = GetInfo();
  int answer_one = PartOne(wires, edges);
  string answer_two = PartTwo();
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
