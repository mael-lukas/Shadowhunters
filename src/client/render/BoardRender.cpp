#include "BoardRender.h"
#include <iostream>
#include "state/Board.h"
#include "client/Client.h"
#include <cmath>


namespace render {
    BoardRender::BoardRender(state::Board* board, sf::RenderWindow* win, RenderManager& renderMan) :
    board(board),
    window(win),
    renderMan(renderMan)
    {}

    void BoardRender::init() {

        ///// text based test /////
        std::string path = std::string(ASSET_PATH);
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
        
    }

    void BoardRender::handleEvent(const sf::Event& event, client::Client* client) {
    }

    void BoardRender::handleEvent(const sf::Event& event, client::ClientMT* client) {
    }


    void BoardRender::draw() {
        window->draw(boardSprite);
        for (int i = 0; i < state::OUTSIDE; i++) {
            state::Cell cell = static_cast<state::Cell>(i);
            window->draw(cellSprites[cell]);
        }
    }
}