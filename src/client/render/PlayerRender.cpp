#include "PlayerRender.h"
#include <iostream>
#include "state/Board.h"

namespace render {
    PlayerRender::PlayerRender(state::Board* board, sf::RenderWindow* win) :
    board(board),
    window(win) {}

    void PlayerRender::init() {
        // TODO: load textures from player cards, pawns etc...

        ///// text based test /////
        if (!test_font.loadFromFile("../../../res/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        test_text.setFont(test_font);
        test_text.setCharacterSize(20);
        test_text.setFillColor(sf::Color::Red);
        test_text.setPosition(400.f,20.f);
    }

    void PlayerRender::handleEvent(const sf::Event& event) {
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
    }
}