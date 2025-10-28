#include "BoardRender.h"
#include <iostream>
#include "state/Board.h"

namespace render {
    BoardRender::BoardRender(state::Board* board, sf::RenderWindow* win) :
    board(board),
    window(win) {}

    void BoardRender::init() {
        // TODO: load textures from board, load textures from cells in map
        
        ///// text based test /////
        if (!test_font.loadFromFile("../../../res/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        test_text.setFont(test_font);
        test_text.setCharacterSize(20);
        test_text.setFillColor(sf::Color::White);
        test_text.setPosition(20.f,20.f);
    }

    void BoardRender::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            std::cout << "OMG a click !" << std::endl;
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
        window->draw(test_text);
    }
}