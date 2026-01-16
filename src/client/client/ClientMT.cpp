#include "ClientMT.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include "../../shared/engine/AttackCommand.h"
#include "../../shared/engine/RevealCommand.h"
#include "../../shared/engine/FranklinCapacityCommand.h"
#include "../../shared/engine/GeorgesCapacityCommand.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>


namespace client {
    ClientMT::ClientMT(render::RenderManager* renderMan, engine::Engine* engineGame, int playerID) : 
    renderMan(renderMan), engineGame(engineGame), playerID(playerID) {}

    void ClientMT::run() {
        renderMan->init();
        renderMan->clientID = playerID;
        while (renderMan->window.isOpen()) {
            if (engineGame->currentGameState != engine::ONGOING) {
                sf::Event event;
                while (renderMan->window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        renderMan->window.close();
                    }
                }
                renderMan->drawGameOverScreen(engineGame->currentGameState);
                std::this_thread::sleep_for(std::chrono::milliseconds(33));
                continue;
            }
            renderMan->renderingTurnPlayer = (engineGame->currentPlayerIndex == playerID);
            sf::Event event;
            while (renderMan->window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    renderMan->window.close();
                }
                renderMan->handleEvent(event, this);
            }
            {   std::lock_guard<std::mutex> lock(engineGame->turnPhaseMutex);
                renderMan->ui_render.setTurnPhase(engineGame->currentTurnPhase);    
            }
            renderMan->draw();
            {   std::lock_guard<std::mutex> lock(engineGame->promptMutex);
                lookForPrompts();
            }

            if (engineGame->currentGameState != engine::ONGOING) {
                std::cout << "Game Over!" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
        }
    }

    void ClientMT::lookForPrompts() {
        if (playerID != engineGame->currentPlayerIndex) {
            return;
        }
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
        if (engineGame->isWaitingForGatePrompt){
            renderMan->openGatePrompt();
        }
        if (engineGame->isWaitingForCardStealPrompt){
            std::vector<state::CardClass*> potentialCards;
            for (auto& player : engineGame->board->playerList) {
                if (player.get() != &engineGame->getCurrentPlayer() && player->isAlive) {
                    potentialCards.insert(potentialCards.end(), player->equipCards.begin(), player->equipCards.end());
                }
            }
            renderMan->openStealEquipPrompt(potentialCards);
        }
        if (engineGame->isWaitingForCardEffectTargetPrompt){
            renderMan->openCardEffectTargetPrompt();
        }
    }

    void ClientMT::moveClicked() {
        if (!engineGame->isBusy)
        {
            cmd = new engine::MoveCommand(*engineGame);
            engineGame->commands.push_back(cmd);
        }
    }

    void ClientMT::cellEffectClicked() {
        if (!engineGame->isBusy)
        {
            cmd = engineGame->cellEffectsFactory[engineGame->getCurrentPlayer().position->cell](*engineGame);
            engineGame->commands.push_back(cmd);
        }
    }

    void ClientMT::damageClicked() {
        if (engineGame->isBusy == false) {
            cmd = new engine::AttackCommand(*engineGame, &engineGame->getCurrentPlayer());
            engineGame -> commands.push_back(cmd);
        }
    }
    
    void ClientMT::YesNoAnswer(bool answer){
        std::cout << "[client] YES NO answer chosen : " << answer << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&answer);
        }
    }
    void ClientMT::chosenAttackTarget(int targetID) {
        std::cout << "[CLIENT] Chosen target ID: " << targetID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&targetID);
        }
    }

    void ClientMT::cellChosen(int cellID){
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

    void ClientMT::cardTypeChosen(int type){
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        std::cout << "Client cardtype chosen: " << type << std::endl;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&type);
        }
    }

    void ClientMT::woodsAnswerClicked(int buttonID) {
        std::cout << "[CLIENT] Woods button ID: " << buttonID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&buttonID);
        }
    }

    void ClientMT::stealEquipAnswer(state::CardClass* chosenCard){
        std::cout << "[CLIENT] Stolen card chosen:" << chosenCard << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(chosenCard);
        }
    }

    void ClientMT::chosenCardEffectTarget(int targetID){
        std::cout << "[CLIENT] Chosen card effect target ID: " << targetID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&targetID);
        }
    }

    void ClientMT::revealedClicked(){
        if (!engineGame->isBusy){
            cmd = new engine::RevealCommand(*engineGame, playerID);
            engineGame->commands.push_back(cmd);
        }
    }

    void ClientMT::capacityClicked(){
        if (!engineGame->isBusy){
            auto& currentPlayer = engineGame->getCurrentPlayer();
            if (currentPlayer.name == state::FRANKLIN) {
                cmd = new engine::FranklinCapacityCommand(*engineGame, &currentPlayer);
                engineGame->commands.push_back(cmd);
            }
            else if (currentPlayer.name == state::GEORGES) {
                cmd = new engine::GeorgesCapacityCommand(*engineGame, &currentPlayer);
                engineGame->commands.push_back(cmd);
            }
        }
    }
}