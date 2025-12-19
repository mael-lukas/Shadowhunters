#include "CardRender.h"
#include <iostream>
#include "state/Board.h"
#include "client/Client.h"

namespace render {
    CardRender::CardRender(state::Board* board, sf::RenderWindow* win) :
    board(board),
    window(win) {}

    void CardRender::init() {
        // TODO: load textures from all cards

        ///// text based test /////
        std::string path = std::string(ASSET_PATH);
        if (!test_font.loadFromFile(path + "/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        test_text.setFont(test_font);
        test_text.setCharacterSize(20);
        test_text.setFillColor(sf::Color::Blue);
        test_text.setPosition(20.f,20.f);
    }

    void CardRender::handleEvent(const sf::Event& event, client::Client* client) {
    }

    void CardRender::draw() {
        ///// text based test /////
        std::string cardInfo = "Card render info \n";
        cardInfo += "The black deck contains " + std::to_string(board->darkPack.listOfCards.size()) + " cards.\n";
        cardInfo += "The white deck contains " + std::to_string(board->whitePack.listOfCards.size()) + " cards.\n";
        cardInfo += "The hermit deck contains " + std::to_string(board->hermitPack.listOfCards.size()) + " cards.\n";
        for (auto& player : board->playerList) {
            if (player->equipCards.size() > 0) {
                cardInfo += "Player in Cell " + std::to_string(player->position->cell) + " has equipped cards: ";
                for (state::CardClass* card : player->equipCards) {
                    cardInfo += std::to_string(card->name) + " ";
                }
                cardInfo += "\n";
            }
        }
        test_text.setString(cardInfo);
        window->draw(test_text);
    }
}