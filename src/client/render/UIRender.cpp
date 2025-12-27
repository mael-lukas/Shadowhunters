#include "UIRender.h"
#include <iostream>
#include "client/Client.h"

namespace render {

    UIRender::UIRender (sf::RenderWindow* win) : window(win) {
        currentTurnPhase = engine::TurnPhase::MOVE_PHASE;
    }

    void UIRender::init () {
        std::string path = std::string(ASSET_PATH);
        if (!font.loadFromFile(path + "/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }

        move_button.setSize(sf::Vector2f(200.f,140.f));
        move_button.setFillColor(sf::Color::White);
        move_button.setPosition(26.f,395.f);
        move_button_text.setFont(font);
        move_button_text.setCharacterSize(26);
        move_button_text.setFillColor(sf::Color::Black);
        move_button_text.setString("Roll dice");
        sf::FloatRect buttonRect = move_button_text.getLocalBounds();
        move_button_text.setOrigin(buttonRect.left + buttonRect.width / 2.0f,
                                   buttonRect.top + buttonRect.height / 2.0f);
        move_button_text.setPosition(move_button.getPosition().x + move_button.getSize().x / 2.0f,
                                     move_button.getPosition().y + move_button.getSize().y / 2.0f);


        cell_effect_button.setSize(sf::Vector2f(200.f,140.f));
        cell_effect_button.setFillColor(sf::Color::Blue);
        cell_effect_button.setPosition(26.f,570.f);
        cell_effect_button_text.setFont(font);
        cell_effect_button_text.setCharacterSize(25);
        cell_effect_button_text.setFillColor(sf::Color::White);
        cell_effect_button_text.setString("Use Cell effect");
        sf::FloatRect buttonRect3 = cell_effect_button_text.getLocalBounds();
        cell_effect_button_text.setOrigin(buttonRect3.left + buttonRect3.width / 2.0f,
                                   buttonRect3.top + buttonRect3.height / 2.0f);
        cell_effect_button_text.setPosition(cell_effect_button.getPosition().x + cell_effect_button.getSize().x / 2.0f,
                                     cell_effect_button.getPosition().y + cell_effect_button.getSize().y / 2.0f);

        attack_button.setSize(sf::Vector2f(200.f,140.f));
        attack_button.setFillColor(sf::Color::Red);
        attack_button.setPosition(26.f,745.f);
        attack_button_text.setFont(font);
        attack_button_text.setCharacterSize(25);
        attack_button_text.setFillColor(sf::Color::White);
        attack_button_text.setString("Conduct attack");
        sf::FloatRect buttonRect2 = attack_button_text.getLocalBounds();
        attack_button_text.setOrigin(buttonRect2.left + buttonRect2.width / 2.0f,
                                   buttonRect2.top + buttonRect2.height / 2.0f);
        attack_button_text.setPosition(attack_button.getPosition().x + attack_button.getSize().x / 2.0f,
                                     attack_button.getPosition().y + attack_button.getSize().y / 2.0f);
    }

    void UIRender::handleEvent (const sf::Event& event, client::Client* client) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
            std::cout << "click (pixels): x=" << clickPos.x << " y=" << clickPos.y << std::endl;

            if (currentTurnPhase == engine::TurnPhase::MOVE_PHASE && move_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Move button clicked" << std::endl;
                client->moveClicked(); 
            }
            if (currentTurnPhase == engine::TurnPhase::CELL_EFFECT_PHASE && cell_effect_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Cell effect button clicked" << std::endl;
                client->cellEffectClicked();
            }
            if (currentTurnPhase == engine::TurnPhase::BATTLE_PHASE && attack_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Attack button clicked" << std::endl;
                client->damageClicked();
            }
        }
    }

    void UIRender::draw () {
        if (currentTurnPhase == engine::TurnPhase::MOVE_PHASE) {
            window->draw(move_button);
            window->draw(move_button_text);
        }
        if (currentTurnPhase == engine::TurnPhase::CELL_EFFECT_PHASE) {
            window->draw(cell_effect_button);
            window->draw(cell_effect_button_text);
        }
        if (currentTurnPhase == engine::TurnPhase::BATTLE_PHASE) {
            window->draw(attack_button);
            window->draw(attack_button_text);
        }
    }

    void UIRender::setTurnPhase (engine::TurnPhase newTurnPhase) {
        currentTurnPhase = newTurnPhase;
    }

};