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
            renderMan->ui_render.setTurnPhase(engineGame->currentTurnPhase);
            renderMan->draw();
            if (engineGame->isWaitingForTargetPrompt) {
                renderMan->openTargetPrompt(engineGame->board->getNeighbours(&engineGame->getCurrentPlayer()));
            }
            else if (engineGame->isWaitingForYesNoPrompt){
                renderMan->openYesNoPrompt();
            }
            if (engineGame->isWaitingForWoodsPrompt) {
                renderMan->openWoodsPrompt();
            }
            if (engineGame->isWaitingForCellPrompt) {
                renderMan->openCellPrompt();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    void Client::moveClicked() {
        if (!engineGame->isBusy)
        {
            cmd = new engine::MoveCommand(*engineGame);
            engineGame->commands.push_back(cmd);
        }

    }

    void Client::damageClicked() {
        if (engineGame->isBusy == false) {
            cmd = new engine::AttackCommand(*engineGame, &engineGame->getCurrentPlayer());
            engineGame -> commands.push_back(cmd);
        }
    }

    void Client::drawClicked(state::CardType cardDraw) {
        if (engineGame->isBusy == false) {
            cmd = new engine::DrawCardCommand(*engineGame, cardDraw);
            engineGame -> commands.push_back(cmd);
        }

    }
    void Client::YesNoAnswer(bool answer){
        std::cout << "[client] YES NO answer chosen : " << answer << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&answer);
        }
    }
    void Client::chosenAttackTarget(int targetID) {
        std::cout << "[CLIENT] Chosen target ID: " << targetID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&targetID);
        }
    }

    void Client::cellChosen(int cellID){
        std::cout << "[CLIENT] Chosen cell ID: " << cellID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            state::CellClass* chosenCell = nullptr;
            for (state::CellClass* cc : engineGame->board->cellList) {
                if (cc->cell == static_cast<state::Cell>(cellID)) {
                    chosenCell = cc;
                    break;
                }
            } 
            engineGame->waitingCommand->receivePromptAnswer(chosenCell);
        }
    }

    void Client::revealedClicked(){

    }
}