#include "PlayerRender.h"
#include <iostream>
#include "state/Board.h"
#include "client/Client.h"
#include "render/Pawn.h"

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


        std::vector<sf::Color> pawnColors = {
            sf::Color::Blue,
            sf::Color::Green,
            sf::Color::Yellow,
            sf::Color::Red
        };

        for (std::unique_ptr<state::Player>& player : board->playerList) {
            sf::CircleShape* shape_wounds = new sf::CircleShape(15.f);
            sf::CircleShape* shape_position = new sf::CircleShape(15.f);
            shape_wounds->setFillColor(pawnColors[player->id]);
            shape_position->setFillColor(pawnColors[player->id]);
            // shape_wounds->setOutlineColor(sf::Color::Black);
            // shape_position->setOutlineColor(sf::Color::Black);
            // shape_wounds->setOutlineThickness(3.f);
            // shape_position->setOutlineThickness(3.f);
            shape_wounds->setOrigin(15.f,15.f);
            shape_position->setOrigin(15.f,15.f);
            Pawn* pawn_wounds = new Pawn(PawnType::WOUNDS, player.get(), shape_wounds, player->wounds);
            Pawn* pawn_position = new Pawn(PawnType::POSITION, player.get(), shape_position, player->position.cell);
            listOfPawns.push_back(pawn_wounds);
            listOfPawns.push_back(pawn_position);
        }

        wounds_coordinates = {{sf::Vector2f(569.f,770.f), sf::Vector2f(618.f,794.f), sf::Vector2f(629.f,744.f), sf::Vector2f(678.f,762.f)},
        {sf::Vector2f(542.f,689.f), sf::Vector2f(584.f,672.f), sf::Vector2f(628.f,631.f), sf::Vector2f(642.f,665.f)},
        };

        cell_coordinates[state::OUTSIDE] = {sf::Vector2f(959.f,548.f), sf::Vector2f(1019.f,548.f), sf::Vector2f(1039.f,493.f), sf::Vector2f(979.f,493.f)};

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
        for (auto& player : board->playerList) {
            playerInfo += "There is a player in Cell " + std::to_string(player->position.cell) + ". He has " + std::to_string(player->wounds) + " wounds"
            + ", " + std::to_string(player->equipCards.size()) + " equipped cards and is " + (player->isAlive ? "alive" : "dead") + "\n";
        }
        test_text.setString(playerInfo);
        window->draw(test_text);
        window->draw(test_button);
        window->draw(test_button_text);

        std::cout << "Test buttons rendered." << std::endl;

        for (Pawn* pawn : listOfPawns) {
            if (pawn->type == PawnType::POSITION) {
                state::Cell cell = pawn->owner->position.cell;
                pawn->shape->setPosition(cell_coordinates[cell][pawn->owner->id]);
            } else if (pawn->type == PawnType::WOUNDS) {
                int wounds = pawn->owner->wounds;
                pawn->shape->setPosition(wounds_coordinates[wounds][pawn->owner->id]);
            }
            window->draw(*pawn->shape);
        }
    }
}