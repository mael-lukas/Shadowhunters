#include "RenderManager.h"
#include <iostream>

namespace render {
    RenderManager::RenderManager(state::Board* board) : 
    board(board),
    board_render(board,&window),
    player_render(board,&window),
    card_render(board,&window),
    prompt_render(board,&window),
    ui_render(board,&window),
    needsRedraw(true) {}

    void RenderManager::init() {
        board->addObserver(this);
        for (auto& player : board->playerList) {
            player->addObserver(this);
        }
        window.create(sf::VideoMode::getDesktopMode(),"Shadowhunters",sf::Style::Default);
        board_render.init();
        player_render.init();
        card_render.init();
        prompt_render.init();
        ui_render.init();
    }

    void RenderManager::handleEvent(const sf::Event& event, client::Client* client) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
            needsRedraw = true;
        }
        board_render.handleEvent(event, client);
        player_render.handleEvent(event, client);
        card_render.handleEvent(event, client);
        prompt_render.handleEvent(event, client);
        ui_render.handleEvent(event, client);
    }

    void RenderManager::draw() {
        if (needsRedraw) {
            window.clear();
            board_render.draw();
            player_render.draw();
            card_render.draw();
            prompt_render.draw();
            ui_render.draw();
            window.display();
            needsRedraw = false;
        }
    }

    void RenderManager::drawGameOverScreen(engine::GameState gameState) {
        sf::Text text;
        text.setFont(ui_render.font);
        text.setCharacterSize(72);
        text.setFillColor(sf::Color::White);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f,
                       bounds.top  + bounds.height / 2.f);
        window.clear();
        if (gameState == engine::DRAW) {
            text.setPosition(550.f, 350.f);
            text.setString("Both parties are dead. \n\t\t It's a draw");
        }
        else {
            text.setPosition(360.f, 350.f);
            std::string winners = (gameState == engine::HUNTERS_WIN) ? "Hunters" : "Shadows";
            std::string losers = (gameState == engine::HUNTERS_WIN) ? "Shadows" : "Hunters";
            text.setString("All " + losers + " have been eliminated. \n\t\t\tThe " + winners + " win !\n");
        }
        window.draw(text);
        window.display();
    }

    void RenderManager::getNotified(state::StateEventID e) {
        needsRedraw = true;
    }

    void RenderManager::openAttackPrompt(std::vector<state::Player*> targets) {
        prompt_render.activePromptType = ATTACK_TARGET;
        prompt_render.targetPlayers = targets;
        needsRedraw = true;
    }

    void RenderManager::openYesNoPrompt(){
        prompt_render.activePromptType = YES_NO;
        needsRedraw = true;
    }

    void RenderManager::openWoodsPrompt() {
        prompt_render.activePromptType = WOODS_PROMPT;
        needsRedraw = true;
    }   
    void RenderManager::openGatePrompt(){
        prompt_render.activePromptType = GATE_PROMPT;
        needsRedraw = true;
    }
    void RenderManager::openCellPrompt() {
        prompt_render.activePromptType = ROLL_7;
        needsRedraw = true;
    }
    void RenderManager::openStealEquipPrompt(std::vector<state::CardClass*> potentialCards){
        prompt_render.activePromptType = STEAL_EQUIP;
        prompt_render.potentialCards = potentialCards;
        needsRedraw = true;
    }
    void RenderManager::openCardEffectTargetPrompt(){
        prompt_render.activePromptType = CARD_EFFECT_TARGET;
        needsRedraw = true;
    }
}