#include "utils.h"

using aoc::split;
using aoc::GetInts;
using aoc::GetLLongs;

typedef long long llong;

template<typename T>
struct Coord {
  T x;
  T y;
  T z;

  Coord() {
    x = y = z = 0;
  }

  Coord(T x_, T y_, T z_) {
    x = x_;
    y = y_;
    z = z_;
  }

  Coord(vector<T> coord) {
    x = coord[0];
    y = coord[1];
    z = coord[2];
  }

  string toString() const {
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
  }
};

struct Hail {
  Coord<llong> p;
  Coord<int> v;

  Hail(Coord<llong> pos, Coord<int> vel) {
    p = pos;
    v = vel;
  }

  // Return line function (in point slope form) evaluated at x
  float f(llong x) const {
    return ((float)v.y / (float)v.x) * (x - p.x) + p.y;
  }

  float g(llong y) const {
    return ((float)v.x / (float)v.y) * (y - p.y) + p.x;
  }

  // Make the hail's velocity relative to given velocity
  void AdjustVel(Coord<int> vel) {
    v.x -= vel.x;
    v.y -= vel.y;
    v.z -=vel.z;
  }

  string toString() const {
    return p.toString() + " @ " + v.toString();
  }
};

vector<Hail> GetInfo() {
  vector<Hail> hail;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    vector<string> info = split(line, " @ ");
    vector<llong> pos = GetLLongs(split(info[0], ", ", false, true));
    vector<int> vel = GetInts(split(info[1], ", ", false, true));
    Coord<llong> p(pos);
    Coord<int> v(vel);
    hail.emplace_back(p, v);
  }
  return hail;
}

bool OppositeSign(llong num1, llong num2) {
  return ((num1 <= 0 && num2 >= 0) || (num1 >= 0 && num2 <= 0));
}

// Using intermediate value theorem (ivt) with some fancy bounds checking to make sure paths start before intersection occurs
int PartOne(const vector<Hail>& hail) {
  int answer = 0;
  llong start = 200000000000000;
  llong end = 400000000000000;
  for (int i = 0; i < hail.size() - 1; ++i) {
    for (int j = i + 1; j < hail.size(); ++j) {
      llong ivt_startx = hail[i].f(start) - hail[j].f(start);
      llong ivt_endx = hail[i].f(end) - hail[j].f(end);
      bool correct_start1x;
      if (hail[i].v.x <= 0) {
        correct_start1x = OppositeSign(ivt_startx, hail[i].f(hail[i].p.x) - hail[j].f(hail[i].p.x));
      } else {
        correct_start1x = OppositeSign(ivt_endx, hail[i].f(hail[i].p.x) - hail[j].f(hail[i].p.x));
      }
      bool correct_start2x;
      if (hail[j].v.x <= 0) {
        correct_start2x = OppositeSign(ivt_startx, hail[i].f(hail[j].p.x) - hail[j].f(hail[j].p.x));
      } else {
        correct_start2x = OppositeSign(ivt_endx, hail[i].f(hail[j].p.x) - hail[j].f(hail[j].p.x));
      }
      bool in_xrange = OppositeSign(ivt_startx, ivt_endx) && correct_start1x && correct_start2x;
      llong ivt_starty = hail[i].g(start) - hail[j].g(start);
      llong ivt_endy = hail[i].g(end) - hail[j].g(end);
      bool correct_start1y;
      if (hail[i].v.y <= 0) {
        correct_start1y = OppositeSign(ivt_starty, hail[i].g(hail[i].p.y) - hail[j].g(hail[i].p.y));
      } else {
        correct_start1y = OppositeSign(ivt_endy, hail[i].g(hail[i].p.y) - hail[j].g(hail[i].p.y));
      }
      bool correct_start2y;
      if (hail[j].v.y <= 0) {
        correct_start2y = OppositeSign(ivt_starty, hail[i].g(hail[j].p.y) - hail[j].g(hail[j].p.y));
      } else {
        correct_start2y = OppositeSign(ivt_endy, hail[i].g(hail[j].p.y) - hail[j].g(hail[j].p.y));
      }
      bool in_yrange = OppositeSign(ivt_starty, ivt_endy) && correct_start1y && correct_start2y;
      if (in_xrange && in_yrange) {
        ++answer;
      }
    }
  }
  return answer;
}

int main() {
  vector<Hail> hail = GetInfo();
  int answer_one = PartOne(hail);
  cout << "Part One: " << answer_one << endl;
  return 0;
}
