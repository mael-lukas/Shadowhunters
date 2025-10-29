#include "Client.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace client {
    Client::Client(state::Board* board, render::RenderManager* renderMan) : 
    board(board), 
    renderMan(renderMan) {}

    void Client::run() {
        renderMan->init();
        while (renderMan->window.isOpen()) {
            sf::Event event;
            while (renderMan->window.pollEvent(event)) {
                renderMan->handleEvent(event, this);
            }

            renderMan->draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    void Client::moveTestClicked() {
        // Test for state update : player moved
        std::cout << "Client received move test click callback." << std::endl;
        board->movePlayerTo(*(board->playerPos[state::OUTSIDE][0]), state::GRAVEYARD);
    }

    void Client::damageTestClicked() {
        // Test for state update : first player in GRAVEYARD takes 3 damage
        std::cout << "Client received damage test click callback." << std::endl;
        if (board->playerPos[state::GRAVEYARD].size() > 0) {
            board->playerPos[state::GRAVEYARD][0]->receiveDamage(3);
        }
    }

    void Client::drawTestClicked() {
        // Test for state update : draw card & equip it to first player in GRAVEYARD if any
        std::cout << "Client received draw test click callback." << std::endl;
        state::Card drawnCard = board->drawDark();
        if (drawnCard != state::Card::NONE && board->playerPos[state::GRAVEYARD].size() > 0) {
            board->equipCard(*(board->playerPos[state::GRAVEYARD][0]), drawnCard);
        }
    }
}