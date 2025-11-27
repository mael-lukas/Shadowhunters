#include "PlayerRender.h"
#include <iostream>
#include "state/Board.h"
#include "client/Client.h"

namespace render {
    PlayerRender::PlayerRender(state::Board* board, sf::RenderWindow* win) :
    board(board),
    window(win) {}

    void PlayerRender::init() {
        // TODO: load textures from player cards, pawns etc...

        ///// text based test /////
        std::string path = std::string(ASSET_PATH);
        if (!test_font.loadFromFile(path + "/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        test_text.setFont(test_font);
        test_text.setCharacterSize(20);
        test_text.setFillColor(sf::Color::Red);
        test_text.setPosition(400.f,20.f);

        test_button.setSize(sf::Vector2f(250.f,140.f));
        test_button.setFillColor(sf::Color::Red);
        test_button.setPosition(750.f,300.f);

        test_button_text.setFont(test_font);
        test_button_text.setCharacterSize(20);
        test_button_text.setFillColor(sf::Color::White);
        test_button_text.setString("Damage test button");

        sf::FloatRect buttonRect = test_button_text.getLocalBounds();
        test_button_text.setOrigin(buttonRect.left + buttonRect.width / 2.0f,
                                   buttonRect.top + buttonRect.height / 2.0f);
        test_button_text.setPosition(test_button.getPosition().x + test_button.getSize().x / 2.0f,
                                     test_button.getPosition().y + test_button.getSize().y / 2.0f);

        for (std::unique_ptr<state::Player>& player : board->playerList) {
            //Pawn* pawn = new Pawn(PawnType::POSITION, player.get());
            //listOfPawns.push_back(pawn);
        }
    }

    void PlayerRender::handleEvent(const sf::Event& event, client::Client* client) {
            if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
            if (test_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Damage test button clicked, simulating state change." << std::endl;
                client->damageClicked();
            }
        }
    }

    void PlayerRender::draw() {
        ///// text based test /////
        std::string playerInfo = "Player render info \n";
        for (auto& pair : board->playerPos) {
            for (state::Player* player : pair.second) {
                playerInfo += "There is a player in Cell " + std::to_string(player->position) + ". He has " + std::to_string(player->wounds) + " wounds"
                + ", " + std::to_string(player->equipCards.size()) + " equipped cards and is " + (player->isAlive ? "alive" : "dead") + "\n";
            }
        }
        test_text.setString(playerInfo);
        window->draw(test_text);
        window->draw(test_button);
        window->draw(test_button_text);
    }
}