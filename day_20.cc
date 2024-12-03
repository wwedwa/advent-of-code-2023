#include "utils.h"

using aoc::split;

struct Module {
  char type;
  string name;
  bool state;
  bool has_recieved_low_pulse;
  long low_pulses;
  long high_pulses;
  queue<pair<int,bool>> pulses;
  vector<Module*> out;
  unordered_map<string, bool> in;

  Module(char t, string n) {
    type = t;
    name = n;
    state = (type == '&');
    low_pulses = 0;
    high_pulses = 0;
    has_recieved_low_pulse = false;
  }

  // For conjunctions to remember their inputs
  void InitializeIn(vector<string> inputs) {
    if (type != '&') {
      return;
    }
    for (string input : inputs) {
      in[input] = 0;
    }
  }

  void AddDestination(Module* module) {
    out.push_back(module);
  }

  void Reset() {
    while (!pulses.empty()) {
      pulses.pop();
    }
    state = (type == '&');
    low_pulses = 0;
    high_pulses = 0;
    has_recieved_low_pulse = false;
    for (auto& input : in) {
      input.second = 0;
    }
  }

  void SendPulse(int tick_num, bool pulse, string n) {
    if (pulse == 0) {
      has_recieved_low_pulse = true;
    }
    pulses.emplace(tick_num, pulse);
    if (type == '&') {
      in[n] = pulse;
    }
  }

  void ProcessNextPulse(int tick_num) {
    if (pulses.empty()) {
      return;
    }
    pair<int,int> next_pulse = pulses.front();
    while (next_pulse.first == tick_num - 1) {
      bool pulse_level = next_pulse.second;
      if (type == 'b') {
        for (Module* dest : out) {
          dest->SendPulse(tick_num, pulse_level, name);
        }
        low_pulses += out.size() * (pulse_level == 0);
        high_pulses += out.size() * (pulse_level == 1);
      } else if (type == '%' && !pulse_level) {
        state = !state;
        for (Module* dest : out) {
          dest->SendPulse(tick_num, state, name);
        }
        low_pulses += out.size() * (state == 0);
        high_pulses += out.size() * (state == 1);
      } else if (type== '&') {
        for (const auto& input : in) {
          state = 0;
          if (!input.second) {
            state = 1;
            break;
          }
        }
        for (Module* dest : out) {
          dest->SendPulse(tick_num, state, name);
        }
        low_pulses += out.size() * (state == 0);
        high_pulses += out.size() * (state == 1);
      }
      pulses.pop();
      next_pulse = pulses.front();
    }
  }

  bool IsEmpty() const {
    return pulses.empty() || type == '.';
  }

  string GetString() const {
    string to_return = type + name + " -> ";
    for (const Module* module : out) {
      to_return += module->type + module->name + ", ";
    }
    return to_return;
  }
};

pair<Module*, vector<Module*>> GetInfo() {
  ifstream input("input.txt");
  string line;
  unordered_map<string, Module*> module_names;
  unordered_map<string, vector<string>> module_dests;
  unordered_map<string, vector<string>> module_inputs;
  while (getline(input, line)) {
    vector<string> info = split(line, " -> ");
    string name = info[0];
    char type;
    if (name == "broadcaster") {
      type = 'b';
    } else {
      type = name[0];
      name = name.substr(1);
    }
    vector<string> dests = split(info[1], ", ");
    for (string dest : dests) {
      module_inputs[dest].push_back(name);
    }
    module_names[name] = new Module(type, name);
    module_dests[name] = dests;
  }
  for (const auto& module : module_dests) {
    string module_name = module.first;
    for (string dest : module.second) {
      if (module_names.count(dest)) {
        module_names[module_name]->AddDestination(module_names[dest]);
      } else {
        module_names[dest] = new Module('.', dest);
        module_names[module_name]->AddDestination(module_names[dest]);
      }
    }
  }
  for (const auto& module : module_inputs) {
    module_names[module.first]->InitializeIn(module.second);
  }
  vector<Module*> all_modules;
  Module* broadcaster = module_names["broadcaster"];
  for (auto& module : module_names) {
    all_modules.push_back(module.second);
  }
  return {broadcaster, all_modules};
}

long PartOne(Module* broadcaster, vector<Module*> modules) {
  int tick = 0;
  int low = 0;
  int high = 0;
  for (int i = 0; i < 1000; ++i) {
    broadcaster->SendPulse(tick, 0, "button");
    ++low;
    ++tick;
    bool empty = false;
    while (!empty) {
      empty = true;
      for (Module* module : modules) {
        module->ProcessNextPulse(tick);
      }
      for (Module* module : modules) {
        if (!module->IsEmpty()) {
          empty = false;
          break;
        }
      }
      ++tick;
    }
  }
  for (Module* module : modules) {
    low += module->low_pulses;
    high += module->high_pulses;
  }
  return low * high;
}

long PartTwo(Module* broadcaster, vector<Module*> modules) {
  int tick = 0;
  long pushes = 0;
  // cl is what feeds into rx. Need everything feeding into
  // it to be 1 since it is of type &. So we calculate the cycle
  // lengths of every module feeing into cl and take the product
  // (since all cycle lengths are prime we do not need LCM)
  Module* cl;
  for (Module* module : modules) {
    if (module->name == "cl") {
      cl = module;
    }
  }
  vector<string> in;
  for (auto input : cl->in) {
    in.push_back(input.first);
  }
  unordered_map<string, int> cycles;
  while (cycles.size() != in.size()) {
    broadcaster->SendPulse(tick, 0, "button");
    ++pushes;
    ++tick;
    bool empty = false;
    while (!empty) {
      empty = true;
      for (Module* module : modules) {
        module->ProcessNextPulse(tick);
        if ((find(in.begin(), in.end(), module->name) != in.end()) && module->state && !cycles.count(module->name) && pushes != 1) {
          cycles[module->name] = pushes;
        }
      }
      for (Module* module : modules) {
        if (!module->IsEmpty()) {
          empty = false;
          break;
        }
      }
      ++tick;
    }
  }
  long answer = 1;
  for (auto cycle : cycles) {
    answer *= cycle.second;
  }
  return answer;
}

int main() {
  auto [broadcaster, modules] = GetInfo();
  int answer_one = PartOne(broadcaster, modules);
  // For some reason, part 2 wasnt working with the old data
  // even when I reset each module, so I just get the data again
  auto [broadcaster2, modules2] = GetInfo();
  long answer_two = PartTwo(broadcaster2, modules2);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  // Delete to avoid memory leaks
  for (auto pointer : modules) {
    delete pointer;
  }
  for (auto pointer : modules2) {
    delete pointer;
  }
  return 0;
}
