#ifndef UTILS_H
#define UTILS_H

// All of this is included just so I don't have to include it separately in every day
#include<iostream>
#include<fstream>
#include <sstream>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include<vector>
#include<queue>
#include<deque>
#include<list>
#include<utility>
#include<tuple>
#include<algorithm>
#include <climits>
#include <numeric>

// Bad practice to do this in header file but I need most of
// these functions every day so it makes it easier
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::getline;
using std::string;
using std::stoi;
using std::stol;
using std::stoll;
using std::unordered_map;
using std::unordered_set;
using std::map;
using std::set;
using std::vector;
using std::queue;
using std::deque;
using std::priority_queue;
using std::list;
using std::pair;
using std::tuple;

namespace aoc {

// Split line into vector of string based on given delimiter
vector<string> split(string line, string delim=" ", bool include_empty_lines=false, bool remove_leading_space=false);

// Checks if a given string is a number (specifically int)
bool IsInt(string num);

// Gets all ints from a vector of strings
vector<int> GetInts(vector<string> strings);

// Gets all longs from a vector of strings
vector<long> GetLongs(vector<string> strings);

// Gets all long longs from a vector of strings
vector<long long> GetLLongs(vector<string> strings);

// Calculate gcd with Euclid's Algorithm
long gcd(long a, long b);

// Calculate lcm for a vector of ints
long lcm(vector<int> nums);

int mod(int m, int n);
}

#endif // UTILS_H