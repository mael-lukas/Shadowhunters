#include "Client.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include <iostream>
#include <thread>
#include <chrono>


namespace client {
    Client::Client(state::Board* board, render::RenderManager* renderMan, engine::Engine* engineGame) : 
    board(board), renderMan(renderMan), engineGame(engineGame) {}

    void Client::run() {
        renderMan->init();
        while (renderMan->window.isOpen()) {
            sf::Event event;
            while (renderMan->window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    renderMan->window.close();
                }
                if (!engineGame->isBusy) {
                    renderMan->handleEvent(event, this);
                }
            }
            if (!engineGame->isBusy) {
                engineGame->processOneCommand();
            }
            renderMan->draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    void Client::moveClicked(state::CellClass* newLocation) {
        cmd = new engine::MoveCommand(newLocation);
        engineGame -> commands.push_back(cmd);

        // test with test button and direct link to board (to be removed when engine is functional) //
        int pos = rand() % 6;
        int pl = rand() % 4;
        board->movePlayerTo(board->playerList[pl].get(),board->cellList[pos]);
    }

    void Client::damageClicked() {
        /*
        cmd = new engine::AttackCommand(int id_AttackedPlayer);
        engineGame -> commands.push_back(cmd);
        */

        // test with test button and direct link to board (to be removed when engine is functional) //
        int pl = rand() % 4;
        board->playerList[pl]->receiveDamage(1);
    }

    void Client::drawClicked(state::CardType cardDraw) {
        cmd = new engine::DrawCardCommand(cardDraw);
        engineGame -> commands.push_back(cmd);
    }

    void Client::revealedClicked(){

    }
}