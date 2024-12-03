#include "utils.h"

namespace aoc {
  
vector<string> split(string line, string delim, bool include_empty_lines, bool remove_leading_space) {
  vector<string> parsed_line;
  while (line.find(delim) != string::npos) {
    string sub_line = line.substr(0, line.find(delim));
    if (remove_leading_space) {
      while (sub_line.length() > 0 && (sub_line[0] == ' ' || sub_line[0] == '\t')) {
        sub_line = sub_line.substr(1);
      }
    }
    if (sub_line != "" || include_empty_lines) {
      parsed_line.push_back(sub_line);
    }
    line = line.substr(line.find(delim) + delim.length());
  }
  if (remove_leading_space) {
    while (line.length() > 0 && (line[0] == ' ' || line[0] == '\t')) {
      line = line.substr(1);
    }
  }
  if (line != "" || include_empty_lines) {
    parsed_line.push_back(line);
  }
  return parsed_line;
}

bool IsInt(string str) {
  for (int i = 0; i < str.length(); ++i) {
    if (!isdigit(str[i]) && (i != 0 || str[i] != '-')) {
      return false;
    }
  }
  return true;
}

vector<int> GetInts(vector<string> strings) {
  vector<int> ints;
  for (string s : strings) {
    if (IsInt(s)) {
      ints.push_back(stoi(s));
    }
  }
  return ints;
}

vector<long> GetLongs(vector<string> strings) {
  vector<long> longs;
  for (string s : strings) {
    if (IsInt(s)) {
      longs.push_back(stol(s));
    }
  }
  return longs;
}

vector<long long> GetLLongs(vector<string> strings) {
  vector<long long> longs;
  for (string s : strings) {
    if (IsInt(s)) {
      longs.push_back(stoll(s));
    }
  }
  return longs;
}

long gcd(long a, long b) {
  if (b == 0)
    return a;
  return gcd(b, a % b);
}
 
long lcm(vector<int> nums) {
  long lcm = nums[0];
  for (size_t i = 1; i < nums.size(); i++) {
    lcm = (((nums[i] * lcm)) / (gcd(nums[i], lcm)));
  }
  return lcm;
}

int mod(int a, int b) {
  return a - b * (a / b);
}

}