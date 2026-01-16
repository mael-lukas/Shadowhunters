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
        overlay.setSize(sf::Vector2f(800.f,750.f));
        overlay.setFillColor(sf::Color(0,0,0,200));
        overlay.setPosition(500.f,200.f);

/////////////// Initialize ATTACK TARGET buttons ////////////////
        for (int i = 0; i < board->playerList.size(); i++) {
            target_players_buttons.push_back(sf::RectangleShape(sf::Vector2f(350.f,100.f)));
            target_players_buttons[i].setPosition(sf::Vector2f(700.f,250.f + i*120.f));
            target_players_buttons[i].setFillColor(sf::Color(150,150,150,255));
        }
        buttonColors = {sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Red};

        yes_button.setPosition(sf::Vector2f(600.f,370.f));
        yes_button.setFillColor(sf::Color(7,196,0));
        no_button.setPosition(sf::Vector2f(900.f,370.f));
        no_button.setFillColor(sf::Color(224,2,2));

/////////////// Initialize CELL CHOICE (ROLL 7) buttons ////////////////
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

/////////////// Initialize UNDERWORLD GATE PROMPT buttons ////////////////
        cardTypeColors = {sf::Color::White, sf::Color::Black, sf::Color::Green, sf::Color(200,200,200)};
        for (int i=0;i<4;i++){
            gateButtons.push_back(sf::RectangleShape(sf::Vector2f(350.f,100.f)));
            gateButtons[i].setPosition(sf::Vector2f(730.f,280.f + i*120.f));
            gateButtons[i].setFillColor(cardTypeColors[i]);
            gateButtons[i].setOutlineColor(sf::Color(150,150,150));
            gateButtons[i].setOutlineThickness(3.f);
        }

/////////////// Initialize WEIRD WOODS PROMPT buttons ////////////////
        for (int i = 0; i < 8; i++) {
            woodsButtons.push_back(sf::RectangleShape(sf::Vector2f(300.f,80.f)));
            woodsButtons[i].setPosition(sf::Vector2f(530.f + (i%2)*430.f, 280.f + (i/2)*110.f));
            woodsButtons[i].setFillColor(buttonColors[i/2]);
        }
        woodsButtons.push_back(sf::RectangleShape(sf::Vector2f(300.f,80.f)));
        woodsButtons[8].setPosition(sf::Vector2f(740.f, 730.f));
        woodsButtons[8].setFillColor(sf::Color(150,150,150));
        
/////////////// Initialize STEAL EQUIP CARD PROMPT buttons ////////////////
        for (int i = 0; i < 4; i++) {
            stealEquipButtons.push_back(sf::RectangleShape(sf::Vector2f(350.f,100.f)));
            stealEquipButtons[i].setPosition(sf::Vector2f(700.f,250.f + i*120.f));
            stealEquipButtons[i].setFillColor(sf::Color::Black);
            stealEquipButtons[i].setOutlineColor(sf::Color(200,200,200));
            stealEquipButtons[i].setOutlineThickness(3.f);
        }
        stealEquipButtons.push_back(sf::RectangleShape(sf::Vector2f(350.f,100.f)));
        stealEquipButtons[4].setPosition(sf::Vector2f(700.f,250.f + 4*120.f));
        stealEquipButtons[4].setFillColor(sf::Color(150,150,150,255));

//////////////// Initialize CARD EFFECT TARGET PROMPT buttons ////////////////
        for (int i = 0; i < board->playerList.size(); i++) {
            cardEffectTargetButtons.push_back(sf::RectangleShape(sf::Vector2f(350.f,100.f)));
            cardEffectTargetButtons[i].setPosition(sf::Vector2f(700.f,250.f + i*120.f));
            cardEffectTargetButtons[i].setFillColor(buttonColors[i]);
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
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < 8; i++) {
                    if (woodsButtons[i].getGlobalBounds().contains(clickPos) && board->playerList[i/2]->isAlive) {
                        client->woodsAnswerClicked(i);
                    }
                }
                if (woodsButtons.back().getGlobalBounds().contains(clickPos)) {
                    client->woodsAnswerClicked(-1);
                }
            }
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
        if (activePromptType == GATE_PROMPT){
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < gateButtons.size(); i++) {
                    if (gateButtons[i].getGlobalBounds().contains(clickPos)) {
                        std::cout << "[PromptRender] Gate button " << i << " clicked." << std::endl;
                        client->cardTypeChosen(i);
                    }
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
        if (activePromptType == STEAL_EQUIP){
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < potentialCards.size(); i++) {
                    if (stealEquipButtons[i].getGlobalBounds().contains(clickPos)) {
                        client->stealEquipAnswer(potentialCards[i]);
                    }
                }
                if (stealEquipButtons.back().getGlobalBounds().contains(clickPos)) {
                    client->stealEquipAnswer(nullptr);
                }
            }
        }
        if (activePromptType == CARD_EFFECT_TARGET){
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < cardEffectTargetButtons.size(); i++) {
                    if (cardEffectTargetButtons[i].getGlobalBounds().contains(clickPos) && board->playerList[i]->isAlive) {
                        client->chosenCardEffectTarget(board->playerList[i]->id);
                    }
                }
            }
        }
    }

    void PromptRender::handleEvent(const sf::Event& event, client::ClientMT* client) {
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
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < 8; i++) {
                    if (woodsButtons[i].getGlobalBounds().contains(clickPos) && board->playerList[i/2]->isAlive) {
                        client->woodsAnswerClicked(i);
                    }
                }
                if (woodsButtons.back().getGlobalBounds().contains(clickPos)) {
                    client->woodsAnswerClicked(-1);
                }
            }
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
        if (activePromptType == GATE_PROMPT){
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < gateButtons.size(); i++) {
                    if (gateButtons[i].getGlobalBounds().contains(clickPos)) {
                        std::cout << "[PromptRender] Gate button " << i << " clicked." << std::endl;
                        client->cardTypeChosen(i);
                    }
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
        if (activePromptType == STEAL_EQUIP){
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < potentialCards.size(); i++) {
                    if (stealEquipButtons[i].getGlobalBounds().contains(clickPos)) {
                        client->stealEquipAnswer(potentialCards[i]);
                    }
                }
                if (stealEquipButtons.back().getGlobalBounds().contains(clickPos)) {
                    client->stealEquipAnswer(nullptr);
                }
            }
        }
        if (activePromptType == CARD_EFFECT_TARGET){
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
                for (int i = 0; i < cardEffectTargetButtons.size(); i++) {
                    if (cardEffectTargetButtons[i].getGlobalBounds().contains(clickPos) && board->playerList[i]->isAlive) {
                        client->chosenCardEffectTarget(board->playerList[i]->id);
                    }
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

            sf::Text cancel_text; 
            cancel_text.setFont(font);
            cancel_text.setCharacterSize(35);
            cancel_text.setFillColor(sf::Color::Black);
            cancel_text.setString("Cancel");
            sf::FloatRect buttonRect = cancel_text.getLocalBounds();
            cancel_text.setOrigin(buttonRect.left + buttonRect.width / 2.0f,
                                    buttonRect.top + buttonRect.height / 2.0f);
            cancel_text.setPosition(target_players_buttons.back().getPosition().x + target_players_buttons.back().getSize().x / 2.0f,
                                        target_players_buttons.back().getPosition().y + target_players_buttons.back().getSize().y / 2.0f);

            window->draw(overlay);
            window->draw(promptText);
            for (int i = 0; i < targetPlayers.size(); i++) {
                target_players_buttons[i].setFillColor(buttonColors[targetPlayers[i]->id]);
                window->draw(target_players_buttons[i]);
            }
            window->draw(target_players_buttons.back());
            window->draw(cancel_text);
        }
        if (activePromptType == WOODS_PROMPT) {
            sf::Text promptText;
            promptText.setFont(font);
            promptText.setString("Deal 2 damage:\t\t\t\t\t Heal 1 wound:");
            promptText.setCharacterSize(34);
            promptText.setFillColor(sf::Color::White);
            promptText.setPosition(570.f,210.f);

            sf::Text cancel_text; 
            cancel_text.setFont(font);
            cancel_text.setCharacterSize(35);
            cancel_text.setFillColor(sf::Color::Black);
            cancel_text.setString("Cancel");
            sf::FloatRect buttonRect = cancel_text.getLocalBounds();
            cancel_text.setOrigin(buttonRect.left + buttonRect.width / 2.0f,
                                    buttonRect.top + buttonRect.height / 2.0f);
            cancel_text.setPosition(woodsButtons.back().getPosition().x + woodsButtons.back().getSize().x / 2.0f,
                                        woodsButtons.back().getPosition().y + woodsButtons.back().getSize().y / 2.0f);

            window->draw(overlay);
            window->draw(promptText);
            for (int i = 0; i < woodsButtons.size() - 1; i++) {
                if (board->playerList[i/2]->isAlive) {
                    window->draw(woodsButtons[i]);
                }
            }
            window->draw(woodsButtons.back());
            window->draw(cancel_text);
        }
        if (activePromptType == GATE_PROMPT){
            sf::Text promptText;
            promptText.setFont(font);
            promptText.setString("From which deck would you like to draw a card ?");
            promptText.setCharacterSize(34);
            promptText.setFillColor(sf::Color::White);
            promptText.setPosition(570.f,210.f);

            sf::Text cancel_text; 
            cancel_text.setFont(font);
            cancel_text.setCharacterSize(35);
            cancel_text.setFillColor(sf::Color::Black);
            cancel_text.setString("Cancel");
            sf::FloatRect buttonRect = cancel_text.getLocalBounds();
            cancel_text.setOrigin(buttonRect.left + buttonRect.width / 2.0f,
                                    buttonRect.top + buttonRect.height / 2.0f);
            cancel_text.setPosition(gateButtons.back().getPosition().x + gateButtons.back().getSize().x / 2.0f,
                                        gateButtons.back().getPosition().y + gateButtons.back().getSize().y / 2.0f);

            window->draw(overlay);
            window->draw(promptText);
            for (const sf::RectangleShape& button : gateButtons) {
                window->draw(button);
            }
            window->draw(cancel_text);
        }
        if(activePromptType == YES_NO){
            sf::Text promptText;
            promptText.setFont(font);
            promptText.setString("Would you like to draw a card:");
            promptText.setCharacterSize(34);
            promptText.setFillColor(sf::Color::White);
            promptText.setPosition(700.f,290.f);
            window->draw(overlay);
            window->draw(promptText);
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
        if (activePromptType == STEAL_EQUIP){
            sf::Text promptText;
            promptText.setFont(font);
            promptText.setString("Choose an equip card to steal:");
            promptText.setCharacterSize(34);
            promptText.setFillColor(sf::Color::White);
            promptText.setPosition(650.f,190.f);

            sf::Text cancel_text; 
            cancel_text.setFont(font);
            cancel_text.setCharacterSize(35);
            cancel_text.setFillColor(sf::Color::Black);
            cancel_text.setString("Cancel");
            sf::FloatRect buttonRect = cancel_text.getLocalBounds();
            cancel_text.setOrigin(buttonRect.left + buttonRect.width / 2.0f,
                                    buttonRect.top + buttonRect.height / 2.0f);
            cancel_text.setPosition(stealEquipButtons.back().getPosition().x + stealEquipButtons.back().getSize().x / 2.0f,
                                        stealEquipButtons.back().getPosition().y + stealEquipButtons.back().getSize().y / 2.0f);

            window->draw(overlay);
            window->draw(promptText);
            for (int i = 0; i < potentialCards.size(); i++) {
                window->draw(stealEquipButtons[i]);
            }
            window->draw(stealEquipButtons.back());
            window->draw(cancel_text);
        }
        if (activePromptType == CARD_EFFECT_TARGET){
            sf::Text promptText;
            promptText.setFont(font);
            promptText.setString("Choose a target for the card effect:");
            promptText.setCharacterSize(34);
            promptText.setFillColor(sf::Color::White);
            promptText.setPosition(650.f,190.f);

            window->draw(overlay);
            window->draw(promptText);
            for (int i = 0; i < cardEffectTargetButtons.size(); i++) {
                if (board->playerList[i]->isAlive) {
                    window->draw(cardEffectTargetButtons[i]);
                }
            }
        }
    }
}