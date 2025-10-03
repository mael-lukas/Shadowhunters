#include "PackOfCards.h"
#include <random>
#include <stdexcept>
#include <algorithm>

namespace state {

  PackOfCards::PackOfCards (int size,CardType type) : packSize(size), type(type) {
    if (type == WHITE) {
      for (int i = 0; i < WHITECOUNT; i++) {
        Card c = static_cast<Card>(i);
        listOfCards.push_back(c);
      }
    } else if (type == DARK) {
      for (int i = WHITECOUNT+1; i < DARKCOUNT; i++) {
        Card c = static_cast<Card>(i);
        listOfCards.push_back(c);
      }
    } else if (type == HERMIT) {
      for (int i = DARKCOUNT+1 ; i < HERMITCOUNT; i++) {
        Card c = static_cast<Card>(i);
        listOfCards.push_back(c);
      }
    }
  }

  void PackOfCards::shuffle () {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(listOfCards.begin(), listOfCards.end(), g);
  }

  Card PackOfCards::draw () {
    if (listOfCards.empty()) {
      throw std::out_of_range("No cards left to draw");
    }
    Card topCard = listOfCards.front();
    listOfCards.erase(listOfCards.begin());
    return topCard;
  }

  void PackOfCards::discard () {
  }

};