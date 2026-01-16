#include "Client.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include "../../shared/engine/AttackCommand.h"
#include "../../shared/engine/RevealCommand.h"
#include "../../shared/engine/FranklinCapacityCommand.h"
#include "../../shared/engine/GeorgesCapacityCommand.h"
#include <iostream>
#include <thread>
#include <chrono>


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
        if (engineGame->isWaitingForYesNoPrompt) {
            state::Player* currentPlayer = &engineGame->getCurrentPlayer();
            bool isAdvent = false;
            if (!currentPlayer->equipCards.empty()) {
                state::CardClass* card = currentPlayer->equipCards.back();
                if (card->name == state::Card::ADVENT1 || card->name == state::Card::ADVENT2) {
                    isAdvent = true;
                }
            }
            if (isAdvent) {
                int playerIndex = -1;
                for (int i = 0; i < engineGame->board->playerList.size(); ++i) {
                    if (engineGame->board->playerList[i].get() == currentPlayer) {
                        playerIndex = i;
                        break;
                    }
                }
                if (playerIndex != -1) {
                    sf::Vector2f bubblePos = renderMan->ui_render.characterBubblesPos[playerIndex];
                    sf::Vector2f promptPos = bubblePos + sf::Vector2f(100.f, 0.f);
                    renderMan->openYesNoPrompt("Voulez-vous révéler votre identité ?", promptPos);
                } else {
                    renderMan->openYesNoPrompt();
                }
            } else {
                renderMan->openYesNoPrompt();
            }
            if (!currentPlayer->equipCards.empty()) {
                renderMan->showEffectCard(currentPlayer->equipCards.back());
            }
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
            // Show the effect card
            state::Player* currentPlayer = &engineGame->getCurrentPlayer();
            if (!currentPlayer->equipCards.empty()) {
                renderMan->showEffectCard(currentPlayer->equipCards.back());
            }
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

    void Client::capacityClicked(){
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
        renderMan->clearEffectCard();
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
        renderMan->clearEffectCard();
        if (engineGame->waitingCommand != nullptr) {
            engineGame->waitingCommand->receivePromptAnswer(&targetID);
        }
    }


}