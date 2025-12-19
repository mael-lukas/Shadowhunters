#include "PackOfCards.h"
#include <random>
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace state {

  PackOfCards::PackOfCards (int size,CardType type) : type(type) {
    if (type == WHITE) {
      for (int i = 0; i < WHITECOUNT; i++) {
        Card c = static_cast<Card>(i);
        listOfCards.push_back(new CardClass(c,WHITE, INSTANT));
      }
    } else if (type == DARK) {
      for (int i = WHITECOUNT+1; i < DARKCOUNT; i++) {
        Card c = static_cast<Card>(i);
        listOfCards.push_back(new CardClass(c,DARK, INSTANT));
      }
    } else if (type == HERMIT) {
      for (int i = DARKCOUNT+1 ; i < HERMITCOUNT; i++) {
        Card c = static_cast<Card>(i);
        listOfCards.push_back(new CardClass(c,HERMIT, INSTANT));
      }
    }
  }

  void PackOfCards::shuffle () {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 g(rd()); // seed the generator
    std::shuffle(listOfCards.begin(), listOfCards.end(), g);
  }

  CardClass* PackOfCards::draw () {
    if (listOfCards.size() > 0) {
      CardClass* topCard = listOfCards.front();
      listOfCards.erase(listOfCards.begin());
      std::cout << "In the pack"<<std::endl;
      return topCard;
    }

    else {
      //CardClass* topCard = new CardClass(NONE,WHITE);
      std::vector<CardClass*>  temp = listOfCards;
      listOfCards = discardPile;
      discardPile = temp;
      std::cout << "ReFill"<<std::endl;
      shuffle();
      CardClass* topCard = listOfCards.front();
      listOfCards.erase(listOfCards.begin());
      return topCard;
    }
  }

  void PackOfCards::discard (CardClass* card) {
    discardPile.push_back(card);
  }

};