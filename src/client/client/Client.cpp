#include "Client.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace client {
    Client::Client(state::Board* board, render::RenderManager* renderMan, engine::Engine* engineGame) : 
    board(board), 
    renderMan(renderMan), engineGame(engineGame) {}

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

    void Client::moveClicked(state::Cell newLocation) {
        cmd = new engine::MoveCommand(newLocation);
        engineGame -> commands.push_back(cmd);
    }

    void Client::damageClicked() {
        // Test for state update : first player in GRAVEYARD takes 3 damage
        std::cout << "Client received damage test click callback." << std::endl;

        if (board->playerPos[state::GRAVEYARD].size() > 0) {
            board->playerPos[state::GRAVEYARD][0]->receiveDamage(3);
        }

        /*
        cmd = new engine::AttackCommand(int id_AttackedPlayer);
        engineGame -> commands.push_back(cmd);
        */
    }

    void Client::drawClicked(state::CardType cardDraw) {
        cmd = new engine::DrawCardCommand(cardDraw);
        engineGame -> commands.push_back(cmd);
    }

    void Client::revealedClicked(){

    }
}