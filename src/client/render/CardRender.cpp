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

        playerDefaultPos = {sf::Vector2f(750.f, 850.f), sf::Vector2f(450.f, 400.f), sf::Vector2f(1050.f, 180.f), sf::Vector2f(1400.f, 600.f)};
        playerRotations = {0.f, 90.f, 180.f, 270.f};
        playerOffsets = {sf::Vector2f(130.f, 0.f), sf::Vector2f(0.f, 130.f), sf::Vector2f(-130.f, 0.f), sf::Vector2f(0.f, -130.f)};
    }

    void CardRender::handleEvent(const sf::Event& event, client::Client* client) {
    }

    void CardRender::draw() {
        ///// text based test /////
        std::string cardInfo = "Card render info \n";
        cardInfo += "The black deck contains " + std::to_string(board->darkPack.listOfCards.size()) + " cards.\n";
        cardInfo += "The white deck contains " + std::to_string(board->whitePack.listOfCards.size()) + " cards.\n";
        cardInfo += "The hermit deck contains " + std::to_string(board->hermitPack.listOfCards.size()) + " cards.\n";
        // for (auto& player : board->playerList) {
        //     if (player->equipCards.size() > 0) {
        //         cardInfo += "Player in Cell " + std::to_string(player->position->cell) + " has equipped cards: ";
        //         for (state::CardClass* card : player->equipCards) {
        //             cardInfo += std::to_string(card->name) + " ";
        //         }
        //         cardInfo += "\n";
        //     }
        // }
        for (int i = 0; i < board->playerList.size(); i++) {
            state::Player* player = board->playerList[i].get();
            for (int j = 0; j < player->equipCards.size(); j++) {
                state::CardClass* card = player->equipCards[j];
                if(card->type != state::HERMIT){
                sf::Texture texture;
                sf::Sprite sprite;
                std::string path = std::string(ASSET_PATH);
                if (!texture.loadFromFile(path + "/sh_card_textures/sh_cards/card" + std::to_string(static_cast<int>(card->name)) + ".jpg")) {
                    std::cerr << "Error loading texture for card " << card->name << std::endl;
                }
                sprite.setTexture(texture);
                sprite.setPosition(playerDefaultPos[i] + playerOffsets[i] * static_cast<float>(j));
                sprite.setRotation(playerRotations[i]);
                sprite.setScale(0.1f, 0.1f);
                window->draw(sprite);
                }
                else {
                    sf::Texture texture;
                    sf::Sprite sprite;
                    std::string path = std::string(ASSET_PATH);
                    if (!texture.loadFromFile(path + "/sh_card_textures/sh_cards/card" + std::to_string(static_cast<int>(card->name)) + ".jpg")) {
                        std::cerr << "Error loading texture for card " << card->name << std::endl;
                    }
                sprite.setTexture(texture);
                sprite.setPosition(1500.f,200.f);
                sprite.setScale(0.3f, 0.3f);
                window->draw(sprite);
                }
            }
        }
        test_text.setString(cardInfo);
        window->draw(test_text);
    }
}