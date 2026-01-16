#include "Client.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include "../../shared/engine/AttackCommand.h"
#include "../../shared/engine/RevealCommand.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "HermitGiveReceive.h"

namespace client {
    Client::Client(render::RenderManager* renderMan, engine::Engine* engineGame) : 
    renderMan(renderMan), engineGame(engineGame) {}

    void Client::run() {
        renderMan->init();
        while (renderMan->window.isOpen()) {
            if (engineGame->currentGameState != engine::ONGOING) {
                sf::Event event;
                while (renderMan->window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        renderMan->window.close();
                    }
                }
                renderMan->drawGameOverScreen(engineGame->currentGameState);
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
                continue;
            }
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
            lookForPrompts();
            engineGame->checkForVictory();
            if (engineGame->currentGameState != engine::ONGOING) {
                std::cout << "Game Over!" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    void Client::lookForPrompts() {
        if (engineGame->isWaitingForAttackPrompt) {
            renderMan->openAttackPrompt(engineGame->board->getNeighbours(&engineGame->getCurrentPlayer()));
        }

        if(engineGame->isWaitingForHermitTargetPrompt){

            //is waiting for effet
            return renderMan->openHermitGivePrompt(engineGame->getCurrentPlayer().id);
            
        }
        if(engineGame->isWaitingForHermitInfoPrompt){
            for(auto card : engineGame->getCurrentPlayer().equipCards){
                if(card->type == state::HERMIT){
                    //TODO Give
                    return renderMan->openHermitReceivePrompt(card);
                }
            }
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

    void Client::moveClicked() {
        if (!engineGame->isBusy)
        {   
            
            cmd = new engine::MoveCommand(*engineGame);
            engineGame->commands.push_back(cmd);
        }
    }

    void Client::revealedClicked(){
        if (!engineGame->isBusy){
            cmd = new engine::RevealCommand(*engineGame,engineGame->getCurrentPlayer().id);
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

    void Client::cardTypeChosen(int type){
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        std::cout << "Client cardtype chosen: " << type << std::endl;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&type);
        }
    }

    void Client::woodsAnswerClicked(int buttonID) {
        std::cout << "[CLIENT] Woods button ID: " << buttonID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&buttonID);
        }
    }

    void Client::stealEquipAnswer(state::CardClass* chosenCard){
        std::cout << "[CLIENT] Stolen card chosen:" << chosenCard << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(chosenCard);
        }
    }

    void Client::chosenCardEffectTarget(int targetID){
        std::cout << "[CLIENT] Chosen card effect target ID: " << targetID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&targetID);
        }
    }

    void Client::chosenHermitTarget(int targetID){
        std::cout << "[CLIENT] Chosen hermit effect target ID: " << targetID << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&targetID);
        }
    }
    void Client::hermitEffect(client::HermitGiveReceive answer){
        std::cout << "[CLIENT] Chosen hermit effect " << std::endl;
        std::cout << "answer choice" << answer.choice << "\n answer damage" << answer.receive << std::endl;
        renderMan->prompt_render.activePromptType = render::PromptType::NONE;
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&answer);
        }
    }

}