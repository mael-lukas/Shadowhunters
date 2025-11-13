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
        test_text.setPosition(400.f,100.f);

        test_button.setSize(sf::Vector2f(250.f,140.f));
        test_button.setFillColor(sf::Color::Blue);
        test_button.setPosition(550.f,500.f);

        test_button_text.setFont(test_font);
        test_button_text.setCharacterSize(20);
        test_button_text.setFillColor(sf::Color::White);
        test_button_text.setString("Draw test button");

        sf::FloatRect buttonRect = test_button_text.getLocalBounds();
        test_button_text.setOrigin(buttonRect.left + buttonRect.width / 2.0f,
                                   buttonRect.top + buttonRect.height / 2.0f);
        test_button_text.setPosition(test_button.getPosition().x + test_button.getSize().x / 2.0f,
                                     test_button.getPosition().y + test_button.getSize().y / 2.0f);
    }

    void CardRender::handleEvent(const sf::Event& event, client::Client* client) {
            if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
            if (test_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Draw test button clicked, simulating state change." << std::endl;
                client->drawTestClicked();
            }
        }
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
        window->draw(test_button);
        window->draw(test_button_text);
    }
}