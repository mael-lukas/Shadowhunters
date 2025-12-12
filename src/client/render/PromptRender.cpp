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
            target_players_buttons[i].setPosition(sf::Vector2f(600.f,250.f + i*120.f));
            target_players_buttons[i].setFillColor(sf::Color(150,150,150,255));
        }

        buttonColors = {sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Red};

        yes_button.setPosition(sf::Vector2f(600.f,370.f));
        no_button.setPosition(sf::Vector2f(900.f,370.f));
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
        else if (activePromptType == YES_NO){
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
    }

    void PromptRender::draw() {
        if (activePromptType == NONE) {
            return;
        }
        if (activePromptType == ATTACK_TARGET) {
            window->draw(overlay);
            for (int i = 0; i < targetPlayers.size(); i++) {
                target_players_buttons[i].setFillColor(buttonColors[targetPlayers[i]->id]);
                window->draw(target_players_buttons[i]);
            }
            window->draw(target_players_buttons.back());
        }
        if (activePromptType == WOODS_PROMPT) {
            window->draw(overlay);
        }
        else if(activePromptType == YES_NO){
            window->draw(overlay);
            window->draw(yes_button);
            window->draw(no_button);
        }
    }
}