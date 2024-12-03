#include "../utils.h"

using aoc::split;
using aoc::GetLongs;

typedef vector<vector<vector<long>>> Almanac;
typedef vector<vector<long>> Page;

pair<Almanac, vector<long>> GetInfo() {
  ifstream input("input.txt");
  string line;
  getline(input, line);
  string seeds = split(line, ": ")[1];
  vector<long> seed_ids = GetLongs(split(seeds, " "));
  int section = -1;
  Almanac almanac;
  while (getline(input, line)) {
    if (line.find(':') != string::npos) {
      Page almanac_page;
      section++;
      almanac.push_back(almanac_page);
    } else if (line.length() != 0) {
      vector<long> info = GetLongs(split(line, " "));
      almanac[section].push_back(info);
    }
  }
  return {almanac, seed_ids};
}

long TranslateNumber(Page page, long num) {
  for (const vector<long>& entry : page) {
    long dest_start = entry[0];
    long src_start = entry[1];
    long length = entry[2];
    if (num >= src_start && num <= src_start + length - 1) {
      return dest_start + num - src_start;
    }
  }
  return num;
}

// Expects pairs in the form: {start, length}
pair<long,long> GetOverlap(pair<long,long> range1, pair<long,long> range2) {
  pair<long,long> lower_range = (range1.first < range2.first) ? range1 : range2;
  pair<long,long> upper_range = (range1.first < range2.first) ? range2 : range1;
  long lower_end = lower_range.first + lower_range.second - 1;
  long upper_end = upper_range.first + upper_range.second - 1;
  // lower_range starts before upper_range. Three cases:
  if (lower_end >= upper_range.first && lower_end < upper_end) {
    // beginning of upper_range overlaps with lower_range
    return {upper_range.first, lower_end - upper_range.first + 1};
  } else if (lower_end >= upper_range.first && lower_end >= upper_end) {
    // lower_range fully includes upper_range
    return upper_range;
  } else {
    // No overlap
    return {0, 0};
  }
}

vector<pair<long,long>> TranslateRange(Page page, pair<long,long> range) {
  vector<pair<long,long>> ranges;
  vector<pair<long,long>> overlaps;
  for (const vector<long>& entry : page) {
    long dest_start = entry[0];
    long src_start = entry[1];
    long length = entry[2];
    pair<long,long> src_range = {src_start, length};
    pair<long,long> overlap = GetOverlap(src_range, range);
    // Only add if the overlap actually exists
    if (overlap.second) {
      overlaps.push_back(overlap);
      ranges.emplace_back(dest_start + overlap.first - src_start, overlap.second);
    }
  }
  // Find all gaps and add to ranges
  std::sort(overlaps.begin(), overlaps.end());
  long begin = range.first;
  if (overlaps.size() == 0) {
    ranges.push_back(range);
  }
  for (int i = 0; i < overlaps.size(); ++i) {
    if (overlaps[i].first > begin) {
      ranges.emplace_back(begin, overlaps[i].first - begin);
    }
    begin = overlaps[i].first + overlaps[i].second;
    if (i == overlaps.size() - 1 && begin < range.first + range.second) {
      ranges.emplace_back(begin, range.second - overlaps[i].second);
    }
  }
  return ranges;
}

long PartOne(Almanac almanac, vector<long> seed_ids) {
  long answer = -1;
  for (long id : seed_ids) {
    long new_id = id;
    for (const Page& page : almanac) {
      new_id = TranslateNumber(page, new_id);
    }
    if (answer > new_id || answer == -1) {
      answer = new_id;
    }
  }
  return answer;
}

long PartTwo(Almanac almanac, vector<long> seed_ids) {
  long answer = -1;
  unordered_set<long> all_seeds;
  for (int i = 0; i < seed_ids.size()-1; i += 2) {
    pair<long,long> seed_range = {seed_ids[i], seed_ids[i+1]};
    vector<pair<long,long>> seed_ranges = {seed_range};
    for (const Page& page : almanac) {
      vector<pair<long,long>> new_ranges;
      for (pair<long,long> seed_range : seed_ranges) {
        vector<pair<long,long>> translated_ranges = TranslateRange(page, seed_range);
        new_ranges.insert(new_ranges.end(), translated_ranges.begin(), translated_ranges.end());
      }
      seed_ranges = new_ranges;
    }
    for (pair<long,long> range : seed_ranges) {
      if (range.first < answer || answer == -1) {
        answer = range.first;
      }
    }
  }
  return answer;
}

int main() {
  auto [almanac, seed_ids] = GetInfo();
  long answer_one = PartOne(almanac, seed_ids);
  long answer_two = PartTwo(almanac, seed_ids);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
