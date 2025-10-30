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
        window.create(sf::VideoMode(1200,700),"Shadowhunters");
        board_render.init();
        player_render.init();
        card_render.init();
    }

    void RenderManager::handleEvent(const sf::Event& event, client::Client* client) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        board_render.handleEvent(event, client);
        player_render.handleEvent(event, client);
        card_render.handleEvent(event, client);
    }

    void RenderManager::draw() {
        if (needsRedraw) {
            window.clear();
            board_render.draw();
            player_render.draw();
            card_render.draw();
            window.display();
            needsRedraw = false;
        }
    }

    void RenderManager::getNotified(state::StateEventID e) {
        needsRedraw = true;
    }
}