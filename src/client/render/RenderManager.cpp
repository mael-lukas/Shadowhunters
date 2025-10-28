#include "RenderManager.h"

namespace render {
    RenderManager::RenderManager(state::Board* board) : 
    board(board),
    board_render(board,&window),
    player_render(board,&window),
    card_render(board,&window),
    needsRedraw(true) {}

    void RenderManager::init() {
        board->addObserver(this);
        for (state::Player* player : board->playerPos[state::OUTSIDE]) {
            player->addObserver(this);
        }
        window.create(sf::VideoMode(1200,600),"Shadowhunters");
        board_render.init();
        player_render.init();
        card_render.init();
    }

    void RenderManager::handleEvent(const sf::Event& event) {
        board_render.handleEvent(event);
        player_render.handleEvent(event);
        card_render.handleEvent(event);
    }

    void RenderManager::run() {
        init();
        while(window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                handleEvent(event);
            }
            if (needsRedraw) {
                window.clear();
                board_render.draw();
                player_render.draw();
                card_render.draw();
                window.display();
                needsRedraw = false;
            }
            sf::sleep(sf::milliseconds(15));
        }
    }

    void RenderManager::getNotified(state::StateEventID e) {
        needsRedraw = true;
    }
}