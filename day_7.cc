#include "../utils.h"

using aoc::split;

struct Hand {
  string cards;
  int primary_order;
  int bet;
  int j_val; // Joker or jack (default jack)

  Hand(string c, int b, int j = 11) {
    cards = c;
    bet = b;
    j_val = j;
    if (j == 11) {
      SetPrimaryOrder();
    } else {
      SetJokerPrimaryOrder();
    }
  }

  void SetPrimaryOrder() {
    vector<char> card_types = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
    primary_order = 0;
    for (char type : card_types) {
      int matches = std::count(cards.begin(), cards.end(), type);
      if (matches == 2) {
        primary_order += 1;
      } else if (matches == 3) {
        primary_order += 3;
      } else if (matches == 4) {
        primary_order += 5;
        return;
      } else if (matches == 5) {
        primary_order += 6;
        return;
      }
    }
    return;
  }

  void SetJokerPrimaryOrder() {
    vector<char> card_types = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
    primary_order = 0;
    int jokers = std::count(cards.begin(), cards.end(), 'J');
    for (char joker_type : card_types) {
      string cards_copy = cards;
      int order = 0;
      if (joker_type != 'J') {
        for (int i = 0; i < jokers; ++i) {
          cards_copy[cards_copy.find('J')] = joker_type;
        }
      }
      for (char type : card_types) {
        int matches = std::count(cards_copy.begin(), cards_copy.end(), type);
        if (matches == 2) {
          order += 1;
        } else if (matches == 3) {
          order += 3;
        } else if (matches == 4) {
          order += 5;
        } else if (matches == 5) {
          order += 6;
        }
      }
      primary_order = std::max(primary_order, order);
    }
    return;
  }

  bool operator<(const Hand& hand2) const {
    unordered_map<char, int> card_weights = {{'A', 14}, {'K', 13}, {'Q', 12}, {'J', j_val}, {'T', 10}};
    if (primary_order != hand2.primary_order) {
      return primary_order < hand2.primary_order;
    }
    for (int i = 0; i < 5; i++) {
      int card1 = (card_weights.count(cards[i])) ? card_weights[cards[i]] : cards[i] - '0';
      int card2 = (card_weights.count(hand2.cards[i])) ? card_weights[hand2.cards[i]] : hand2.cards[i] - '0';
      if (card1 != card2) {
        return card1 < card2;
      }
    }
    return true;
  }
};

pair<set<Hand>, set<Hand>> GetInfo() {
  set<Hand> hands;
  set<Hand> joker_hands;
  ifstream input("input.txt");
  string line;
  while (getline(input, line)) {
    vector<string> hand_data = split(line, " ");
    hands.emplace(hand_data[0], stoi(hand_data[1]));
    joker_hands.emplace(hand_data[0], stoi(hand_data[1]), 1);
  }
  return {hands, joker_hands};
}

int PartOne(set<Hand> hands) {
  int answer = 0;
  int i = 1;
  for (const Hand& hand : hands) {
    answer += i * hand.bet;
    ++i;
  }
  return answer;
}

int PartTwo(set<Hand> hands) {
  int answer = 0;
  int i = 1;
  for (const Hand& hand : hands) {
    answer += i * hand.bet;
    ++i;
  }
  return answer;
}

int main() {
  auto [hands, joker_hands] = GetInfo();
  int answer_one = PartOne(hands);
  int answer_two = PartTwo(joker_hands);
  cout << "Part One: " << answer_one << endl;
  cout << "Part Two: " << answer_two << endl;
  return 0;
}
