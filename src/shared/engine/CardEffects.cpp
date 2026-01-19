#include "Engine.h"
#include "DarkBatCommand.h"
#include "DarkDollCommand.h"
#include "DarkSpiderCommand.h"
#include "BlackRitualCommand.h"
#include "BlackDynamiteCommand.h"
#include "WhiteFlareCommand.h"
#include "WhiteWaterCommand.h"
#include "WhiteAidCommand.h"
#include "WhiteBlessingCommand.h"
#include "WhiteAdventCommand.h"
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
    WhiteBlessingCommand::WhiteBlessingCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    WhiteAdventCommand::WhiteAdventCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    BlackRitualCommand::BlackRitualCommand(Engine& engine, state::Player* user): engine(engine), isWaitingForChoice(true), user(user)
    {}
    BlackDynamiteCommand::BlackDynamiteCommand(Engine& engine, state::Player* user): engine(engine), user(user)
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

    void WhiteBlessingCommand::execute() {
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
                int healAmount = engine.board->rollDice(state::ONLYD6);
                target->receiveDamage(-healAmount);
                std::cout << "[BLESSING] Healed " << target->name << " for " << healAmount << " HP" << std::endl;
            }
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardEffectTargetPrompt = false;
            }
            isDone = true;
        }
    }

    void WhiteBlessingCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        {   std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForCardEffectTargetPrompt = false;
        }
        isWaitingForTarget = false;
    }

    void WhiteAdventCommand::execute() {
        sf::Clock timer;
        if (user->getRole() != state::HUNTER) {
            engine.currentTurnPhase = BATTLE_PHASE;
            timer.restart();
            while (timer.getElapsedTime().asSeconds() < 1.5f) {}
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            isDone = true;
            return;
        }

        if (user->revealed) {
            engine.currentTurnPhase = BATTLE_PHASE;
            user->wounds = 0;
            user->receiveDamage(0);
            std::cout << "[ADVENT] " << user->name << " was fully healed!" << std::endl;
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            isDone = true;
            return;
        }
        
        if (isWaitingForChoice) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForYesNoPrompt = true;
                engine.waitingCommand = this;
                engine.customPromptText = "Do you want to reveal yourself?";
            }
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if (user->revealed) {
                user->wounds = 0;
                user->receiveDamage(0);
                std::cout << "[ADVENT] " << user->name << " revealed and was fully healed!" << std::endl;
            }
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForYesNoPrompt = false;
            }
            isDone = true;
        }
    }

    void WhiteAdventCommand::receivePromptAnswer(void* answer) {
        bool choice = *static_cast<bool*>(answer);
        if (choice) {
            user->revealed = true;
        }
        {   std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForYesNoPrompt = false;
        }
        isWaitingForChoice = false;
    }

    void BlackRitualCommand::execute() {
        sf::Clock timer;
        if (user->getRole() != state::SHADOW) {
            timer.restart();
            while (timer.getElapsedTime().asSeconds() < 1.5f) {}
            engine.currentTurnPhase = BATTLE_PHASE;
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            isDone = true;
            return;
        }

        if (user->revealed) {
            engine.currentTurnPhase = BATTLE_PHASE;
            user->wounds = 0;
            user->receiveDamage(0);
            std::cout << "[DIABOLIC RITUAL] " << user->name << " was fully healed!" << std::endl;
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            isDone = true;
            return;
        }
        
        if (isWaitingForChoice) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForYesNoPrompt = true;
                engine.waitingCommand = this;
                engine.customPromptText = "Do you want to reveal yourself?";
            }
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if (user->revealed) {
                user->wounds = 0;
                user->receiveDamage(0);
                std::cout << "[DIABOLIC RITUAL] " << user->name << " revealed and was fully healed!" << std::endl;
            }
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForYesNoPrompt = false;
            }
            isDone = true;
        }
    }

    void BlackRitualCommand::receivePromptAnswer(void* answer) {
        bool choice = *static_cast<bool*>(answer);
        if (choice) {
            user->revealed = true;
        }
        {   std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForYesNoPrompt = false;
        }
        isWaitingForChoice = false;
    }

    void BlackDynamiteCommand::execute() {
        sf::Clock timer;
        engine.currentTurnPhase = BATTLE_PHASE;
        timer.restart();
        while (timer.getElapsedTime().asSeconds() < 1.5f) {}
        
        int result = engine.board->rollDice(state::SUM);
        std::cout << "[DYNAMITE] Rolled " << result << std::endl;
        
        if (result == 7) {
            std::cout << "[DYNAMITE] Nothing happens (rolled 7)" << std::endl;
        } else {
            for (auto& player : engine.board->playerList) {
                if (player->isAlive && player->position->zone == result) {
                    player->receiveDamage(3);
                    std::cout << "[DYNAMITE] " << player->name << " takes 3 damage!" << std::endl;
                }
            }
        }
        
        engine.board->discardCard(user->equipCards.back());
        user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        isDone = true;
    }
}