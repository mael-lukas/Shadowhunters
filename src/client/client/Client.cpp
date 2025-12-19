#include "Client.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include "../../shared/engine/AttackCommand.h"
#include "../../shared/ai/RandomAI.h"
#include <iostream>
#include <thread>
#include <chrono>


namespace client {
    Client::Client(state::Board* board, render::RenderManager* renderMan, engine::Engine* engineGame,ai::RandomAI* randomAI) : 
    board(board), renderMan(renderMan), engineGame(engineGame),randomAI(randomAI){}

    void Client::run() {
        sf::Clock clock; // démarre le chrono
        sf::Time elapsed1;
        std::cout << elapsed1.asSeconds() << std::endl;
        renderMan->init();
        while (renderMan->window.isOpen()) {
            sf::Event event;
            if(engineGame->getCurrentPlayer().type != state::LevelAI::HUMAN){
                while (renderMan->window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        renderMan->window.close();
                    }
                    else {}
                }
                elapsed1 = clock.getElapsedTime();
                if(elapsed1.asSeconds() > 0.8f){
                    renderMan->draw();
                    randomAI->setTurnPhase(engineGame->currentTurnPhase);
                    randomAI->playPhase();
                    engineGame ->processOneCommand();
                    clock.restart();
                }
            }
            else{
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
            }
            engineGame->processOneCommand();
            renderMan->ui_render.setTurnPhase(engineGame->currentTurnPhase);
            renderMan->draw();
            lookForPrompts();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    void Client::lookForPrompts() {
        if (engineGame->isWaitingForAttackPrompt) {
            renderMan->openAttackPrompt(engineGame->board->getNeighbours(&engineGame->getCurrentPlayer()));
        }
        if (engineGame->isWaitingForYesNoPrompt) {
            renderMan->openYesNoPrompt();
        }
        if (engineGame->isWaitingForWoodsPrompt) {
            renderMan->openWoodsPrompt();
        }
        if (engineGame->isWaitingForCellPrompt) {
            renderMan->openCellPrompt();
        }
    }

    void Client::moveClicked() {
        if (!engineGame->isBusy)
        {
            cmd = new engine::MoveCommand(*engineGame);
            engineGame->commands.push_back(cmd);
        }
    }

    void Client::cellEffectClicked() {
        if (!engineGame->isBusy)
        {
            cmd = engineGame->cellEffectsFactory[engineGame->getCurrentPlayer().position->cell](*engineGame);
            engineGame->commands.push_back(cmd);
        }
    }

    void Client::damageClicked() {
        if (engineGame->isBusy == false) {
            cmd = new engine::AttackCommand(*engineGame, &engineGame->getCurrentPlayer());
            engineGame -> commands.push_back(cmd);
        }
    }
    
    void Client::YesNoAnswer(bool answer){
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&answer);
        }
    }
    void Client::chosenAttackTarget(int targetID) {
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&targetID);
        }
    }

    void Client::cellChosen(int cellID){
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

    void Client::woodsAnswerClicked(int buttonID) {
        std::cout << "[CLIENT] Woods button ID: " << buttonID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&buttonID);
        }
    }

    void Client::revealedClicked(){

    }
}