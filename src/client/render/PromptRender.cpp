#include "PromptRender.h"
#include <iostream>
#include "state/Board.h"
#include "client/Client.h"

namespace render {
    PromptRender::PromptRender(state::Board* board, sf::RenderWindow* win) :
    board(board),
    window(win) {}

    void PromptRender::init() {
        std::string path = std::string(ASSET_PATH);
        if (!font.loadFromFile(path + "/arial.ttf")) {
            std::cerr << "Error loading font" << std::endl;
        }
        overlay.setSize(sf::Vector2f(800.f,600.f));
        overlay.setFillColor(sf::Color(0,0,0,200));
        overlay.setPosition(500.f,200.f);

        for (int i = 0; i < board->playerList.size(); i++) {
            target_players_buttons.push_back(sf::RectangleShape(sf::Vector2f(350.f,100.f)));
            target_players_buttons[i].setPosition(sf::Vector2f(700.f,250.f + i*120.f));
            target_players_buttons[i].setFillColor(sf::Color(150,150,150,255));
        }

        buttonColors = {sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Red};

        yes_button.setPosition(sf::Vector2f(600.f,370.f));
        no_button.setPosition(sf::Vector2f(900.f,370.f));

         for (int i = 0; i < state::OUTSIDE; i++) {
            sf::Texture texture;
            std::string cellPath = path + "/sh_card_textures/sh_area/area0" + std::to_string(i) + ".jpg";
            if (!texture.loadFromFile(cellPath)) {
                std::cerr << "Error loading texture for cell " << i << std::endl;
            }
            cellTextures.push_back(texture);
        }
        for (int i = 0; i < state::OUTSIDE; i++) {
            sf::Sprite sprite;
            sprite.setTexture(cellTextures[i]);
            sprite.setScale(0.13f,0.13f);
            sprite.setPosition(600.f + 200.f*(i/2), 300.f + 250.f*(i%2));
            cellSprites.push_back(sprite);
        }
    }

    void PromptRender::handleEvent(const sf::Event& event, client::Client* client) {
        if (activePromptType == NONE) {
            return;
        }
        if (activePromptType == ATTACK_TARGET) {
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < targetPlayers.size(); i++) {
                    if (target_players_buttons[i].getGlobalBounds().contains(clickPos)) {
                        client->chosenAttackTarget(targetPlayers[i]->id);
                    }
                }
                if (target_players_buttons.back().getGlobalBounds().contains(clickPos)) {
                    client->chosenAttackTarget(-1);
                }
            }
        }
        if (activePromptType == WOODS_PROMPT) {
            
        }
        if (activePromptType == YES_NO){
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                if(yes_button.getGlobalBounds().contains(clickPos)){
                    client->YesNoAnswer(true);
                }
                else if (no_button.getGlobalBounds().contains(clickPos)){
                    client->YesNoAnswer(false);
                }
            }
        }
        if (activePromptType == ROLL_7) {
            sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
            for (int i = 0; i < cellSprites.size(); i++) {
                if (cellSprites[i].getGlobalBounds().contains(clickPos)) {
                    client->cellChosen(i);
                }
            }
        }
    }

    void PromptRender::draw() {
        if (activePromptType == NONE) {
            return;
        }
        if (activePromptType == ATTACK_TARGET) {
            sf::Text promptText;
            promptText.setFont(font);
            promptText.setString("Choose your attack target:");
            promptText.setCharacterSize(34);
            promptText.setFillColor(sf::Color::White);
            promptText.setPosition(700.f,190.f);
            window->draw(overlay);
            window->draw(promptText);
            for (int i = 0; i < targetPlayers.size(); i++) {
                target_players_buttons[i].setFillColor(buttonColors[targetPlayers[i]->id]);
                window->draw(target_players_buttons[i]);
            }
            window->draw(target_players_buttons.back());
        }
        if (activePromptType == WOODS_PROMPT) {
            window->draw(overlay);
        }
        if(activePromptType == YES_NO){
            window->draw(overlay);
            window->draw(yes_button);
            window->draw(no_button);
        }
        if (activePromptType == ROLL_7) {
            sf::Text promptText;
            promptText.setFont(font);
            promptText.setString("Choose a cell to move to:");
            promptText.setCharacterSize(34);
            promptText.setFillColor(sf::Color::White);
            promptText.setPosition(700.f,220.f);
            window->draw(overlay);
            window->draw(promptText);
            for (const sf::Sprite& sprite : cellSprites) {
                window->draw(sprite);
            }
        }
    }
}