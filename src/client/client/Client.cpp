#include "Client.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include "../../shared/engine/AttackCommand.h"
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
                renderMan->handleEvent(event, this);
            }
            engineGame->processOneCommand();
            renderMan->draw();
            if (engineGame->isWaitingForTargetPrompt) {
                renderMan->openTargetPrompt(engineGame->board->getNeighbours(&engineGame->getCurrentPlayer()));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    void Client::moveClicked(state::CellClass* newLocation) {
        if (!engineGame->isBusy)
        {
            cmd = new engine::MoveCommand(*engineGame, newLocation);
            engineGame->commands.push_back(cmd);
        }

        // test with test button and direct link to board (to be removed when engine is functional) //
        // int pos = rand() % 6;
        // int pl = rand() % 4;
        // board->movePlayerTo(board->playerList[pl].get(),board->cellList[pos]);
    }

    void Client::damageClicked() {
        if (engineGame->isBusy == false) {
            cmd = new engine::AttackCommand(*engineGame, &engineGame->getCurrentPlayer());
            engineGame -> commands.push_back(cmd);
        }
        // // test with test button and direct link to board (to be removed when engine is functional) //
        // int pl = rand() % 4;
        // board->playerList[pl]->receiveDamage(1);
    }

    void Client::drawClicked(state::CardType cardDraw) {
        if (engineGame->isBusy == false) {
            cmd = new engine::DrawCardCommand(*engineGame, cardDraw);
            engineGame -> commands.push_back(cmd);
        }

    }

    void Client::chosenAttackTarget(int targetID) {
        std::cout << "[CLIENT] Chosen target ID: " << targetID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&targetID);
        }
    }

    void Client::revealedClicked(){

    }
}