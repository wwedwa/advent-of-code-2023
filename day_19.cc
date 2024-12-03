#include "utils.h"
#include<fstream>

using std::ofstream;

using aoc::split;

typedef vector<pair<string,string>> Workflow;
typedef unordered_map<string, Workflow> Workflows;
typedef unordered_map<char,int> Part;
typedef vector<Part> Parts;

pair<Workflows, Parts> GetInfo() {
  Workflows workflows;
  Parts parts;
  ifstream input("input.txt");
  string line;
  // Read rules
  while (getline(input, line) && !line.empty()) {
    line = line.substr(0, line.length() - 1);
    vector<string> info = split(line, "{");
    string name = info[0];
    vector<string> rules = split(info[1], ",");
    Workflow workflow;
    for (int i = 0; i < rules.size(); ++i) {
      if (i == rules.size() - 1) {
        workflow.emplace_back("default", rules[i]);
      } else {
        vector<string> workflow_info = split(rules[i], ":");
        workflow.emplace_back(workflow_info[0], workflow_info[1]);
      }
    }
    workflows[name] = workflow;
  }
  // Read parts
  while (getline(input, line)) {
    line = line.substr(1, line.length() - 2);
    vector<string> info = split(line, ",");
    Part part;
    part['x'] = stoi(info[0].substr(2));
    part['m'] = stoi(info[1].substr(2));
    part['a'] = stoi(info[2].substr(2));
    part['s'] = stoi(info[3].substr(2));
    parts.push_back(part);
  }
  return {workflows, parts};
}

bool EvaluateRule(string rule, Part part) {
  if (rule == "default") {
    return true;
  }
  char c = rule[0];
  int rule_val = stoi(rule.substr(2));
  int part_val = part[c];
  if (rule.find('<') != string::npos) {
    return part_val < rule_val;
  } else {
    return part_val > rule_val;
  }
}
// takes all workflows, and the name of a workflow and the conditional in it that accepts a part
// (Since some workflows have various conditionals which accept a part)
pair<vector<string>,vector<string>> GetAcceptedPath(const Workflows& workflows, string name, string condition) {
  vector<string> conds_to_succeed = {condition};
  vector<string> conds_to_fail;
  // First, get the conds that we need to fail from the current workflow  
  for (const pair<string,string>& rule : workflows.at(name)) {
    if (rule.first != condition) {
      conds_to_fail.push_back(rule.first);
    } else {
      break;
    }
  }
  while (name != "in") {
    for (const auto& name_workflow : workflows) {
      string curr_name = name_workflow.first;
      vector<string> tentative_to_fail;
      bool found = false;
      for (const pair<string,string>& rule : name_workflow.second) {
        if (rule.second == name) {
          found = true;
          conds_to_succeed.push_back(rule.first);
          break;
        } else {
          tentative_to_fail.push_back(rule.first);
        }
      }
      if (found) {
        name = curr_name;
        conds_to_fail.insert(conds_to_fail.end(), tentative_to_fail.begin(), tentative_to_fail.end());
        break;
      }
    }
  }
  return {conds_to_succeed, conds_to_fail};
}

vector<pair<vector<string>, vector<string>>> GetAcceptedPaths(const Workflows& workflows) {
  vector<pair<vector<string>, vector<string>>> all_paths;
  for (const auto& name_workflow : workflows) {
    string name = name_workflow.first;
    vector<string> conds_to_fail;
    for (const pair<string,string>& rule : name_workflow.second) {
      string condition = rule.first;
      string go_to = rule.second;
      if (go_to == "A") {
        all_paths.push_back(GetAcceptedPath(workflows, name, condition));
      }
    }
  }
  return all_paths;
}

int PartOne(const Workflows& workflows, const Parts& parts) {
  int answer = 0;
  vector<Part> accepted;
  for (const Part& part : parts) {
    string curr_name = "in";
    while (curr_name != "A" && curr_name != "R") {
      for (const pair<string,string>& rule : workflows.at(curr_name)) {
        if (EvaluateRule(rule.first, part)) {
          curr_name = rule.second;
          break;
        }
      }
    }
    if (curr_name == "A") {
      accepted.push_back(part);
    }
  }
  for (Part p : accepted) {
    answer += p['x'] + p['m'] + p['a'] + p['s'];
  }
  return answer;
}

long PartTwo(const Workflows& workflows) {
  long answer = 0;
  vector<unordered_map<char, pair<int,int>>> all_ranges;
  vector<pair<vector<string>, vector<string>>> all_paths = GetAcceptedPaths(workflows);
  for (pair<vector<string>, vector<string>> conds : all_paths) {
    unordered_map<char, pair<int,int>> ranges = {{'x', {1,4000}}, {'m', {1,4000}}, {'a', {1,4000}}, {'s', {1,4000}}};
    vector<string> to_succeed = conds.first;
    vector<string> to_fail = conds.second;
    for (string cond : to_succeed) {
      if (cond.find('>') != string::npos) {
        char char_bound = cond[0];
        int new_lbound = stoi(cond.substr(2)) + 1;
        if (ranges[char_bound].first < new_lbound) {
          ranges[char_bound].first = new_lbound;
        }
      } else if (cond.find('<') != string::npos) {
        char char_bound = cond[0];
        int new_ubound = stoi(cond.substr(2)) - 1;
        if (ranges[char_bound].second > new_ubound) {
          ranges[char_bound].second = new_ubound;
        };
      }
    }
    for (string cond : to_fail) {
      if (cond.find('>') != string::npos) {
        char char_bound = cond[0];
        int new_ubound = stoi(cond.substr(2));
        if (ranges[char_bound].second > new_ubound) {
          ranges[char_bound].second = new_ubound;
        }
      } else if (cond.find('<') != string::npos) {
        char char_bound = cond[0];
        int new_lbound = stoi(cond.substr(2));
        if (ranges[char_bound].first < new_lbound) {
          ranges[char_bound].first = new_lbound;
        };
      }
    }
    all_ranges.push_back(ranges);
  }
  ofstream output("output.txt");
  for (const auto& ranges : all_ranges) {
    string chars = "xmas";
    long possibilities = 1;
    for (char c : chars) {
      output << c << ":[" << ranges.at(c).first << "," << ranges.at(c).second << "]\n";
      possibilities *= ranges.at(c).second - ranges.at(c).first + 1;
    }
    answer += possibilities;
  }
  output.close();
  return answer;
}

int main() {
  auto [workflows, parts] = GetInfo();
  int answer_one = PartOne(workflows, parts);
  long answer_two = PartTwo(workflows);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
