#include "BoardRender.h"
#include <iostream>
#include "state/Board.h"
#include "client/Client.h"
#include <cmath>


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
        for (int i = 0; i < state::OUTSIDE; i++) {
            sf::Texture texture;
            std::string cellPath = path + "/sh_card_textures/sh_area/area0" + std::to_string(i) + ".jpg";
            if (!texture.loadFromFile(cellPath)) {
                std::cerr << "Error loading texture for cell " << i << std::endl;
            }
            cellTextures[static_cast<state::Cell>(i)] = texture;
        }

        boardSprite.setTexture(boardTexture);
        boardSprite.setScale(0.6f,0.6f);
        //center image on screen
        sf::FloatRect spriteRect = boardSprite.getLocalBounds();
        boardSprite.setOrigin(spriteRect.left + spriteRect.width / 2.0f,
                              spriteRect.top + spriteRect.height / 2.0f);
        boardSprite.setPosition(920.f,520.f); //half of 1920x1080

        sf::Vector2f center(1001,524);
        float radius = 109.f;
        float angleOffsetDeg = 30.f;

        for (int zone = 0; zone < 3; zone++) {
            float angleDeg = zone * 120.f + angleOffsetDeg;
            float angleRad = angleDeg * 3.14159265f / 180.f;

            sf::Vector2f positionOfZone(
                center.x + radius * std::cos(angleRad),
                center.y + radius * std::sin(angleRad)
            );

            sf::Vector2f tangentDirection(
                -std::sin(angleRad),
                std::cos(angleRad)
            );

            sf::Vector2f radialDirection(
                std::cos(angleRad),
                std::sin(angleRad)
            );

            for (int i = 0; i < 2; i++) {
                state::CellClass *cellClass = board->cellList[2 * zone + i];
                state::Cell cell = cellClass->cell;
                sf::Sprite cellSprite;
                cellSprite.setTexture(cellTextures[cell]);
                cellSprite.setScale(0.13f,0.13f);
                cellSprite.setOrigin(cellSprite.getLocalBounds().width / 2.0f,
                                     cellSprite.getLocalBounds().height / 2.0f);
                float tangentOffset = (i == 0) ? -74.f : 74.f;
                float radialOffset = cellSprite.getGlobalBounds().height / 2.0f;

                sf::Vector2f finalOffset = tangentDirection * tangentOffset + radialDirection * radialOffset;
                cellSprite.setPosition(positionOfZone + finalOffset);
                cellSprite.setRotation(angleDeg + 90);
                cellSprites[cell] = cellSprite;
            }
        }

        test_text.setFont(test_font);
        test_text.setCharacterSize(20);
        test_text.setFillColor(sf::Color::White);
        test_text.setPosition(20.f,20.f);

        test_button.setSize(sf::Vector2f(250.f,140.f));
        test_button.setFillColor(sf::Color::White);
        test_button.setPosition(26.f,395.f);

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
            std::cout << "click (pixels): x=" << clickPos.x << " y=" << clickPos.y << std::endl;

            handleClickOnCell(event, client);

            if (test_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Move test button clicked, simulating state change." << std::endl;
                client->moveClicked(board->cellList[0]);
            }
        }
    }

    void BoardRender::handleClickOnCell(const sf::Event& event, client::Client* client) {
        sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
        for (state::CellClass* cc : board->cellList) {
            state::Cell cell = cc->cell;
            sf::FloatRect cellBounds = cellSprites[cell].getGlobalBounds();
            if (cellBounds.contains(clickPos)) {
                std::cout << "Cell " << cell << " clicked." << std::endl;
                client->moveClicked(cc);
                return;
            }
        }
    }

    void BoardRender::draw() {
        ///// text based test /////
        std::string boardInfo = "State render info \n";
        for (state::CellClass* cc : board->cellList) {
            boardInfo += "Cell " + std::to_string(cc->cell) + " in zone " + std::to_string(cc->zone) + " containes " + std::to_string(cc->playersInCell.size()) + " players.\n";
            boardInfo += "\n";
        }
        test_text.setString(boardInfo);
        window->draw(boardSprite);
        window->draw(test_text);
        window->draw(test_button);
        window->draw(test_button_text);
        for (int i = 0; i < state::OUTSIDE; i++) {
            state::Cell cell = static_cast<state::Cell>(i);
            window->draw(cellSprites[cell]);
        }
    }
}