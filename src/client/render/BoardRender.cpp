#include "BoardRender.h"
#include <iostream>
#include "state/Board.h"
#include "client/Client.h"

namespace render {
    BoardRender::BoardRender(state::Board* board, sf::RenderWindow* win) :
    board(board),
    window(win) {}

    void BoardRender::init() {
        // TODO: load textures from board, load textures from cells in map
        
        ///// text based test /////
        std::string path = std::string(ASSET_PATH);
        if (!test_font.loadFromFile(path + "/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        if (!boardTexture.loadFromFile(path + "/Board.jpg")) {
            std::cerr << "Error loading board texture" << std::endl;
        }

        boardSprite.setTexture(boardTexture);
        boardSprite.setScale(0.6f,0.6f);
        //center image on screen
        sf::FloatRect spriteRect = boardSprite.getLocalBounds();
        boardSprite.setOrigin(spriteRect.left + spriteRect.width / 2.0f,
                              spriteRect.top + spriteRect.height / 2.0f);
        boardSprite.setPosition(920.f,520.f); //half of 1920x1080

        test_text.setFont(test_font);
        test_text.setCharacterSize(20);
        test_text.setFillColor(sf::Color::White);
        test_text.setPosition(20.f,20.f);

        test_button.setSize(sf::Vector2f(250.f,140.f));
        test_button.setFillColor(sf::Color::White);
        test_button.setPosition(450.f,300.f);

        test_button_text.setFont(test_font);
        test_button_text.setCharacterSize(20);
        test_button_text.setFillColor(sf::Color::Black);
        test_button_text.setString("Move test button");

        sf::FloatRect buttonRect = test_button_text.getLocalBounds();
        test_button_text.setOrigin(buttonRect.left + buttonRect.width / 2.0f,
                                   buttonRect.top + buttonRect.height / 2.0f);
        test_button_text.setPosition(test_button.getPosition().x + test_button.getSize().x / 2.0f,
                                     test_button.getPosition().y + test_button.getSize().y / 2.0f);

    }

    void BoardRender::handleEvent(const sf::Event& event, client::Client* client) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
            if (test_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Move test button clicked, simulating state change." << std::endl;
                client->moveTestClicked();
            }
        }
    }

    void BoardRender::draw() {
        ///// text based test /////
        std::string boardInfo = "State render info \n";
        boardInfo += "Player count outside the board: " + std::to_string(board->playerPos[state::OUTSIDE].size()) + "\n";
        boardInfo += "Player count in the Graveyard: " + std::to_string(board->playerPos[state::GRAVEYARD].size()) + "\n";
        boardInfo += "Player count in the Altar: " + std::to_string(board->playerPos[state::ALTAR].size()) + "\n";
        boardInfo += "Player count in the Hermit zone: " + std::to_string(board->playerPos[state::HERMITZONE].size()) + "\n";
        boardInfo += "Player count in the Woods: " + std::to_string(board->playerPos[state::WOODS].size()) + "\n";
        boardInfo += "Player count in the Gate: " + std::to_string(board->playerPos[state::GATE].size()) + "\n";
        boardInfo += "Player count in the Church: " + std::to_string(board->playerPos[state::CHURCH].size()) + "\n";
        for (const auto& pair : board->cellToZone) {
            boardInfo += "Cell " + std::to_string(pair.first) + " is in zone " + std::to_string(pair.second) + "\n";
        }
        test_text.setString(boardInfo);
        window->draw(boardSprite);
        window->draw(test_text);
        window->draw(test_button);
        window->draw(test_button_text);
    }
}