#include "PackOfCards.h"
#include <random>
#include <stdexcept>
#include <algorithm>

namespace state {

  PackOfCards::PackOfCards (int size,CardType type) : type(type) {
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
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 g(rd()); // seed the generator
    std::shuffle(listOfCards.begin(), listOfCards.end(), g);
  }

  Card PackOfCards::draw () {
    // NOT IMPLEMENTED : if deck is empty, reshuffle discard pile into deck
    if (listOfCards.size() > 0) {
      Card topCard = listOfCards.front();
      listOfCards.erase(listOfCards.begin());
      return topCard;
    }
    else {
      return Card::NONE;
    }
  }

  void PackOfCards::discard (Card card) {
    // NOT IMPLEMENTED
    discardPile.push_back(card);
  }

};