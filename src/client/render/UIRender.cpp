#include "UIRender.h"
#include <iostream>
#include "client/Client.h"
#include "../shared/state/CharacterName.h"
#include "../shared/state/Subject.h"
#include "../shared/state/CardType.h"
#include "../shared/state/Card.h"

namespace render {

    UIRender::UIRender (state::Board* board, sf::RenderWindow* win) : board(board), window(win) {
        currentTurnPhase = engine::TurnPhase::MOVE_PHASE;
    }

    void UIRender::init () {
        std::string path = std::string(ASSET_PATH);
        if (!font.loadFromFile(path + "/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }

        for (int i = 0; i < 4; i++) {
            sf::Texture texture;
            if (!texture.loadFromFile(path + "/sh_card_textures/sh_character_bubbles/char" + std::to_string(i) + ".png")) {
                std::cerr << "Error loading character texture " << i << std::endl;
            }
            characterTextures.push_back(texture);
            sf::Texture texture2;
            if (!texture2.loadFromFile(path + "/sh_card_textures/sh_characters/" + std::to_string(i) + ".jpg")) {
                std::cerr << "Error loading character texture " << i << std::endl;
            }
            characterFullCard.push_back(texture2);
        }

        for (int i = 0; i < static_cast<int>(state::Card::NONE); i++) {
            sf::Texture texture;
            if (!texture.loadFromFile(path + "/sh_card_textures/sh_cards/card" + std::to_string(i) + ".jpg")) {
                std::cerr << "Error loading card texture " << i << std::endl;
            }
            cardTextures.push_back(texture);
        }

        for (int i = 0; i < characterTextures.size(); i++) {
            sf::Sprite sprite;
            sprite.setTexture(characterTextures[i]);
            sprite.setScale(0.3f, 0.3f);
            characterSprites.push_back(sprite);
            sf::Sprite sprite2;
            sprite2.setTexture(characterFullCard[i]);
            sprite2.setScale(0.3f, 0.3f);
            characterCardSprite.push_back(sprite2);
        }

        characterBubblesPos = {sf::Vector2f(500.f, 860.f), sf::Vector2f(440.f, 230.f), sf::Vector2f(1320.f, 170.f), sf::Vector2f(1410.f, 800.f)};
        playerColors = {sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Red};
        // create move button
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


        // create reveal button
        reveal_button.setSize(sf::Vector2f(200.f,140.f));
        reveal_button.setFillColor(sf::Color::Blue);
        reveal_button.setPosition(26.f,195.f);
        reveal_button_text.setFont(font);
        reveal_button_text.setCharacterSize(26);
        reveal_button_text.setFillColor(sf::Color::White);
        reveal_button_text.setString("Reveal yourself");
        sf::FloatRect buttonRect4 = reveal_button_text.getLocalBounds();
        reveal_button_text.setOrigin(buttonRect4.left + buttonRect4.width / 2.0f,
                                   buttonRect4.top + buttonRect4.height / 2.0f);
        reveal_button_text.setPosition(reveal_button.getPosition().x + reveal_button.getSize().x / 2.0f,
                                     reveal_button.getPosition().y + reveal_button.getSize().y / 2.0f);

        // create capacity button
        capacity_button.setSize(sf::Vector2f(200.f,140.f));
        capacity_button.setFillColor(sf::Color::Green);
        capacity_button.setPosition(26.f,195.f);
        capacity_button_text.setFont(font);
        capacity_button_text.setCharacterSize(26);
        capacity_button_text.setFillColor(sf::Color::White);
        capacity_button_text.setString("Capacity\nOne time use");
        sf::FloatRect buttonRect5 = capacity_button_text.getLocalBounds();
        capacity_button_text.setOrigin(buttonRect5.left + buttonRect5.width / 2.0f,
                                   buttonRect5.top + buttonRect5.height / 2.0f);
        capacity_button_text.setPosition(capacity_button.getPosition().x + capacity_button.getSize().x / 2.0f,
                                     capacity_button.getPosition().y + capacity_button.getSize().y / 2.0f);
    }

    void UIRender::handleEvent (const sf::Event& event, client::Client* client) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
            std::cout << "click (pixels): x=" << clickPos.x << " y=" << clickPos.y << std::endl;
            
            // Find the current player
            state::Player* currentPlayer = nullptr;
            for (auto& player : board->playerList) {
                if (player->isTurnPlayer) {
                    currentPlayer = player.get();
                    break;
                }
            }
            
            // Check reveal button first
            if (currentPlayer && !currentPlayer->revealed && 
                reveal_button.getGlobalBounds().contains(clickPos)){
                std::cout << "Reveal button clicked" << std::endl;
                client->revealedClicked(); 
                return;
            }
            
            // Check capacity button
            if (currentPlayer && currentTurnPhase == engine::TurnPhase::MOVE_PHASE && 
                currentPlayer->revealed &&
                (currentPlayer->name == state::CharacterName::FRANKLIN || 
                 currentPlayer->name == state::CharacterName::GEORGES) &&
                !currentPlayer->capacityUsed &&
                capacity_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Capacity button clicked" << std::endl;
                client->capacityClicked();
                return;
            }
            
            if (currentTurnPhase == engine::TurnPhase::MOVE_PHASE && move_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Move button clicked" << std::endl;
                client->moveClicked(); 
                return;
            }
            if (currentTurnPhase == engine::TurnPhase::CELL_EFFECT_PHASE && cell_effect_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Cell effect button clicked" << std::endl;
                client->cellEffectClicked();
                return;
            }
            if (currentTurnPhase == engine::TurnPhase::BATTLE_PHASE && attack_button.getGlobalBounds().contains(clickPos)) {
                std::cout << "Attack button clicked" << std::endl;
                client->damageClicked();
                return;
            }
            for(int i = 0; i < board->playerList.size(); i++){
                state::Player* player = board->playerList[i].get();
                sf::Sprite sprite = characterSprites[static_cast<int>(player->name)];
                sprite.setRotation(i * 90.f);
                sprite.setPosition(characterBubblesPos[i]);
                std::cout<< "is each player tested, displaying info"<<player->name<< std::endl;
                if(sprite.getGlobalBounds().contains(clickPos)){
                    std::cout<< "clicked on player, displaying info"<<player->name<< std::endl;
                    selectedPlayer = player;
                    draw();
                    window->display();
                return;
                }
            }
        }
    }

    void UIRender::draw () {
        for (int i = 0; i < board->playerList.size(); i++) {
            state::Player* player = board->playerList[i].get();
            sf::Sprite sprite = characterSprites[static_cast<int>(player->name)];
            sprite.setRotation(i * 90.f);
            sprite.setPosition(characterBubblesPos[i]);
            sf::FloatRect bounds = sprite.getGlobalBounds();
            sf::Vector2f sprite_center = sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            float outline = 10.f;
            sf::CircleShape circle;
            circle.setRadius(bounds.width / 2.f + outline);
            circle.setOrigin(circle.getRadius(), circle.getRadius());
            circle.setPosition(sprite_center);
            circle.setFillColor(playerColors[i]);
            if (player->isTurnPlayer) {
                circle.setOutlineThickness(5.f);
                circle.setOutlineColor(sf::Color::White);
                if(player->revealed==false){
                    window->draw(reveal_button);
                    window->draw(reveal_button_text);
                }

                else if (currentTurnPhase == engine::TurnPhase::MOVE_PHASE && 
                         !player->capacityUsed &&
                         (player->name == state::CharacterName::FRANKLIN || 
                          player->name == state::CharacterName::GEORGES)) {
                    window->draw(capacity_button);
                    window->draw(capacity_button_text);
                }
            } else {
                circle.setOutlineThickness(0.f);
            }
            window->draw(circle);
            if (player->revealed==true){
            window->draw(sprite);}

        }
        if(selectedPlayer){
            sf::Sprite sprite = characterCardSprite[static_cast<int>(selectedPlayer->name)];
            sprite.setPosition(1500.f,200.f);
            window->draw(sprite);
            selectedPlayer = nullptr;
        }
        if (effectCard != nullptr) {
            sf::Sprite sprite(cardTextures[static_cast<int>(effectCard->name)]);
            sprite.setScale(0.3f, 0.3f);
            sprite.setPosition(1500.f, 200.f);
            window->draw(sprite);
        }
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

    void UIRender::setTurnPhase(engine::TurnPhase newTurnPhase) {
        currentTurnPhase = newTurnPhase;
    }

    void UIRender::setEffectCard(state::CardClass* card) {
        effectCard = card;
    }

};