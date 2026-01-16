#include "Engine.h"
#include "DarkBatCommand.h"
#include "DarkDollCommand.h"
#include "DarkSpiderCommand.h"
#include "WhiteFlareCommand.h"
#include "WhiteWaterCommand.h"
#include "WhiteAidCommand.h"
#include <iostream>
#include <algorithm>
#include <SFML/System/Clock.hpp>

namespace engine {
    DarkSpiderCommand::DarkSpiderCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    DarkDollCommand::DarkDollCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    DarkBatCommand::DarkBatCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    WhiteFlareCommand::WhiteFlareCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    WhiteWaterCommand::WhiteWaterCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    WhiteAidCommand::WhiteAidCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}

    void DarkSpiderCommand::execute() {
        if (isWaitingForTarget) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = true;
                engine.waitingCommand = this;
            }
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if (target != nullptr) {
                target->receiveDamage(2);
                user->receiveDamage(2);
            }
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = false;
            }
            isDone = true;
        }
    }

    void DarkSpiderCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        {   std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForCardEffectTargetPrompt = false;
        }
        isWaitingForTarget = false;
    }

    void DarkDollCommand::execute() {
        if (isWaitingForTarget) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = true;
                engine.waitingCommand = this;
            }
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if (target != nullptr) {
                int result = engine.board->rollDice(state::ONLYD6);
                if (result <= 4) {
                    target->receiveDamage(3);
                }
                else {
                    user->receiveDamage(3);
                }
            }
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = false;
            }
            isDone = true;
        }
    }

    void DarkDollCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        {   std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForCardEffectTargetPrompt = false;
        }
        isWaitingForTarget = false;
    }

    void DarkBatCommand::execute() {
        if (isWaitingForTarget) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = true;
                engine.waitingCommand = this;
            }
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if (target != nullptr) {
                target->receiveDamage(2);
                user->receiveDamage(-1);
            }
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = false;
            }
            isDone = true;
        }
    }

    void DarkBatCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        {   std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForCardEffectTargetPrompt = false;
        }
        isWaitingForTarget = false;
    }

    void WhiteFlareCommand::execute() {
        sf::Clock timer;
        engine.currentTurnPhase = BATTLE_PHASE;
        timer.restart();
        while (timer.getElapsedTime().asSeconds() < 1.5f) {}  
        for (auto& player : engine.board->playerList) {
            if (player.get() != &engine.getCurrentPlayer() && player->isAlive) {
                player->receiveDamage(2);
            }
        }
        engine.board->discardCard(user->equipCards.back());
        user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        isDone = true;
    }

    void WhiteWaterCommand::execute() {
        sf::Clock timer;
        engine.currentTurnPhase = BATTLE_PHASE;
        timer.restart();
        while (timer.getElapsedTime().asSeconds() < 1.5f) {}
        user->receiveDamage(-2);
        engine.board->discardCard(user->equipCards.back());
        user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        isDone = true;
    }

    void WhiteAidCommand::execute() {
        if (isWaitingForTarget) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = true;
                engine.waitingCommand = this;
            }
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if (target != nullptr) {
                target->wounds = 0;
                target->receiveDamage(7);
            }
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = false;
            }
            isDone = true;
        }
    }

    void WhiteAidCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        {   std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForCardEffectTargetPrompt = false;
        }
        isWaitingForTarget = false;
    }
}