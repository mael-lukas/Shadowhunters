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
        test_button.setPosition(26.f,570.f);

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
            Pawn* pawn_position = new Pawn(PawnType::POSITION, player.get(), shape_position, player->position->cell);
            listOfPawns.push_back(pawn_wounds);
            listOfPawns.push_back(pawn_position);
        }

        wounds_coordinates = {{sf::Vector2f(569.f,770.f), sf::Vector2f(618.f,794.f), sf::Vector2f(629.f,744.f), sf::Vector2f(678.f,762.f)},
        {sf::Vector2f(542.f,689.f), sf::Vector2f(584.f,672.f), sf::Vector2f(618.f,635.f), sf::Vector2f(654.f,653.f)},
        {sf::Vector2f(530.f,595.f),sf::Vector2f(559.f,564.f),sf::Vector2f(593.f,588.f),sf::Vector2f(618.f,555.f)},
        {sf::Vector2f(531.f,470.f),sf::Vector2f(543.f,505.f),sf::Vector2f(568.f,476.f),sf::Vector2f(588.f,509.f)},
        {sf::Vector2f(525.f,394.f),sf::Vector2f(540.f,428.f),sf::Vector2f(569.f,396.f),sf::Vector2f(584.f,430.f)},
        {sf::Vector2f(542.f,298.f),sf::Vector2f(523.f,341.f),sf::Vector2f(555.f,328.f),sf::Vector2f(589.f,343.f)},
        {sf::Vector2f(613.f,236.f),sf::Vector2f(566.f,256.f),sf::Vector2f(599.f,266.f),sf::Vector2f(617.f,296.f)},
        {sf::Vector2f(655.f,227.f),sf::Vector2f(700.f,229.f),sf::Vector2f(676.f,253.f),sf::Vector2f(668.f,286.f)},
        {sf::Vector2f(749.f,230.f),sf::Vector2f(782.f,243.f),sf::Vector2f(765.f,276.f),sf::Vector2f(731.f,283.f)},
        {sf::Vector2f(817.f,290.f),sf::Vector2f(825.f,330.f),sf::Vector2f(791.f,319.f),sf::Vector2f(757.f,334.f)},
        {sf::Vector2f(825.f,373.f),sf::Vector2f(813.f,405.f),sf::Vector2f(790.f,380.f),sf::Vector2f(758.f,375.f)},
        {sf::Vector2f(745.f,473.f),sf::Vector2f(778.f,444.f),sf::Vector2f(746.f,440.f),sf::Vector2f(739.f,409.f)},
        {sf::Vector2f(660.f,475.f),sf::Vector2f(707.f,496.f),sf::Vector2f(694.f,467.f),sf::Vector2f(706.f,437.f)},
        {sf::Vector2f(635.f,408.f),sf::Vector2f(644.f,437.f),sf::Vector2f(664.f,399.f),sf::Vector2f(675.f,428.f)},
        {sf::Vector2f(691.f,375.f),sf::Vector2f(707.f,345.f),sf::Vector2f(672.f,332.f),sf::Vector2f(650.f,355.f)}
        };

        cell_coordinates[state::OUTSIDE] = {sf::Vector2f(959.f,548.f), sf::Vector2f(1019.f,548.f), sf::Vector2f(1039.f,493.f), sf::Vector2f(979.f,493.f)};
        cell_coordinates[board->cellList[0]->cell] = {sf::Vector2f(1253.f,621.f), sf::Vector2f(1217.f,598.f), sf::Vector2f(1248.f,542.f), sf::Vector2f(1284.f,562.f)};
        cell_coordinates[board->cellList[1]->cell] = {sf::Vector2f(1176.f,669.f), sf::Vector2f(1213.f,691.f), sf::Vector2f(1180.f,748.f), sf::Vector2f(1143.f,730.f)};
        cell_coordinates[board->cellList[2]->cell] = {sf::Vector2f(796.f,687.f), sf::Vector2f(825.f,745.f), sf::Vector2f(867.f,726.f), sf::Vector2f(834.f,668.f)};
        cell_coordinates[board->cellList[3]->cell] = {sf::Vector2f(727.f,559.f), sf::Vector2f(756.f,617.f), sf::Vector2f(767.f,538.f), sf::Vector2f(796.f,598.f)};
        cell_coordinates[board->cellList[4]->cell] = {sf::Vector2f(897.f,257.f), sf::Vector2f(897.f,303.f), sf::Vector2f(963.f,257.f), sf::Vector2f(963.f,303.f)};
        cell_coordinates[board->cellList[5]->cell] = {sf::Vector2f(1045.f,257.f), sf::Vector2f(1045.f,307.f), sf::Vector2f(1109.f,257.f), sf::Vector2f(1109.f,307.f)};
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
            playerInfo += "There is a player in Cell " + std::to_string(player->position->cell) + ". He has " + std::to_string(player->wounds) + " wounds"
            + ", " + std::to_string(player->equipCards.size()) + " equipped cards and is " + (player->isAlive ? "alive" : "dead") + "\n";
        }
        test_text.setString(playerInfo);
        window->draw(test_text);
        window->draw(test_button);
        window->draw(test_button_text);

        for (Pawn* pawn : listOfPawns) {
            if (pawn->type == PawnType::POSITION) {
                state::Cell cell = pawn->owner->position->cell;
                pawn->shape->setPosition((cell_coordinates[cell])[pawn->owner->id]);
            } else if (pawn->type == PawnType::WOUNDS) {
                int wounds = pawn->owner->wounds;
                pawn->shape->setPosition(wounds_coordinates[wounds][pawn->owner->id]);
            }
            window->draw(*pawn->shape);
        }
    }
}