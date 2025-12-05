#include "RenderManager.h"
#include <iostream>

namespace render {
    RenderManager::RenderManager(state::Board* board) : 
    board(board),
    board_render(board,&window),
    player_render(board,&window),
    card_render(board,&window),
    prompt_render(board,&window),
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
    }

    void RenderManager::draw() {
        if (needsRedraw) {
            window.clear();
            board_render.draw();
            player_render.draw();
            card_render.draw();
            prompt_render.draw();
            window.display();
            needsRedraw = false;
        }
    }

    void RenderManager::getNotified(state::StateEventID e) {
        needsRedraw = true;
    }

    void RenderManager::openTargetPrompt(std::vector<state::Player*> targets) {
        prompt_render.activePromptType = ATTACK_TARGET;
        prompt_render.targetPlayers = targets;
        needsRedraw = true;
    }
}