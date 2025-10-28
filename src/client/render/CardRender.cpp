#include "CardRender.h"
#include <iostream>
#include "state/Board.h"

namespace render {
    CardRender::CardRender(state::Board* board, sf::RenderWindow* win) :
    board(board),
    window(win) {}

    void CardRender::init() {
        // TODO: load textures from all cards

        ///// text based test /////
        if (!test_font.loadFromFile("../../../res/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        test_text.setFont(test_font);
        test_text.setCharacterSize(20);
        test_text.setFillColor(sf::Color::Blue);
        test_text.setPosition(400.f,100.f);
    }

    void CardRender::handleEvent(const sf::Event& event) {
    }

    void CardRender::draw() {
        ///// text based test /////
        std::string cardInfo = "Card render info \n";
        cardInfo += "The black deck contains " + std::to_string(board->darkPack.listOfCards.size()) + " cards.\n";
        cardInfo += "The white deck contains " + std::to_string(board->whitePack.listOfCards.size()) + " cards.\n";
        cardInfo += "The hermit deck contains " + std::to_string(board->hermitPack.listOfCards.size()) + " cards.\n";
        for (auto& pair : board->playerPos) {
            for (state::Player* player : pair.second) {
                if (player->equipCards.size() > 0) {
                    cardInfo += "Player in Cell " + std::to_string(player->position) + " has equipped cards: ";
                    for (state::Card card : player->equipCards) {
                        cardInfo += std::to_string(card) + " ";
                    }
                    cardInfo += "\n";
                }
            }
        }
        test_text.setString(cardInfo);
        window->draw(test_text);
    }
}