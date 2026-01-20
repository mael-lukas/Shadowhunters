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
            engine.isWaitingForCardEffectTargetPrompt = true;
            engine.waitingCommand = this;
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
            engine.isWaitingForCardEffectTargetPrompt = false;
            isDone = true;
        }
    }

    void DarkSpiderCommand::executeAI() {
        std::cout << "Dark Spider AI executed" << std::endl;
        engine.currentTurnPhase = BATTLE_PHASE;
        if (target != nullptr) {
            std::cout << "Dark Spider AI: Target:" << target->id << " received 2 damage. His wounds: " << target->wounds << std::endl;
            std::cout << "Dark Spider AI: User:" << user->id << " received 2 damage. His wounds: " << user->wounds << std::endl;
            target->receiveDamage(2);
            user->receiveDamage(2);
            std::cout << "Dark Spider AI: Target:" << target->id << " received 2 damage. His wounds: " << target->wounds << std::endl;
            std::cout << "Dark Spider AI: User:" << user->id << " received 2 damage. His wounds: " << user->wounds << std::endl;
        }
        if (!user->equipCards.empty()) {
            std::cout << "Dark Spider AI: Discarding Dark Spider card." << std::endl;
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        }
        engine.isWaitingForCardEffectTargetPrompt = false;
        isDone = true;
        
    }

    void DarkSpiderCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardEffectTargetPrompt = false;
        isWaitingForTarget = false;
    }

    void DarkDollCommand::execute() {
        if (isWaitingForTarget) {
            engine.isWaitingForCardEffectTargetPrompt = true;
            engine.waitingCommand = this;
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
            engine.isWaitingForCardEffectTargetPrompt = false;
            isDone = true;
        }
    }

    void DarkDollCommand::executeAI() {
        std::cout << "Dark Doll AI executed" << std::endl;
        engine.currentTurnPhase = BATTLE_PHASE;
        if (target != nullptr) {
            int result = engine.board->rollDice(state::ONLYD6);
            if (result <= 4) {
                std::cout << "Dark Doll AI: Player:" << target->id << " HP: " << target->wounds << std::endl;
                target->receiveDamage(3);
                std::cout << "Dark Doll AI: Target:" << target->id << " received 3 damage." << std::endl;
            }
            else {
                std::cout << "Dark Doll AI: User:" << user->id << " HP: " << user->wounds << std::endl;
                user->receiveDamage(3);
                std::cout << "Dark Doll AI: User:" << user->id << " received 3 damage." << std::endl;
            }
        }
        else{
            std::cout << "Dark Doll AI: Discarding Dark Doll card." << std::endl;
        }
        if (!user->equipCards.empty()) {
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        }
        engine.isWaitingForCardEffectTargetPrompt = false;
        isDone = true;
        
    }

    void DarkDollCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardEffectTargetPrompt = false;
        isWaitingForTarget = false;
    }

    void DarkBatCommand::execute() {
        if (isWaitingForTarget) {
            engine.isWaitingForCardEffectTargetPrompt = true;
            engine.waitingCommand = this;
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
            engine.isWaitingForCardEffectTargetPrompt = false;
            isDone = true;
        }
    }

    //A modifier pour que la target soit choisie par l'IA
    void DarkBatCommand::executeAI() {
        std::cout << "Dark Bat AI executed" << std::endl;
        engine.currentTurnPhase = BATTLE_PHASE;
        if (target != nullptr) {
            std::cout << "Dark Bat AI: Player:" << target->id << " HP: " << target->wounds << std::endl;
            std::cout << "Dark Bat AI: User:" << user->id << " HP: " << user->wounds << std::endl;
            target->receiveDamage(2);
            user->receiveDamage(-1);
            std::cout << "Dark Bat AI: Target:" << target->id << " received 2 damage.His wounds: " << target->wounds << std::endl;
            std::cout << "Dark Bat AI: User:" << user->id << " healed 1 damage. His wounds: " << user->wounds << std::endl;
        }
        if (!user->equipCards.empty()) {
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        }
        engine.isWaitingForCardEffectTargetPrompt = false;
        isDone = true;
    }

    void DarkBatCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardEffectTargetPrompt = false;
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

    void WhiteFlareCommand::executeAI() {
        sf::Clock timer;
        engine.currentTurnPhase = BATTLE_PHASE;
        timer.restart();
        while (timer.getElapsedTime().asSeconds() < 1.5f) {}  
        for (auto& player : engine.board->playerList) {
            if (player.get() != &engine.getCurrentPlayer() && player->isAlive) {
                std::cout << "White Flare AI: Player:" << player->id << " Wounds: " << player->wounds << std::endl;
                player->receiveDamage(2);
                std::cout << "White Flare AI: Player:" << player->id << " received 2 damage.His wounds: " << player->wounds << std::endl; 
            }
        }
        if (!user->equipCards.empty()) {
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        }
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

    void WhiteWaterCommand::executeAI() {
        std::cout << "White Water AI executed" << std::endl;
        sf::Clock timer;
        engine.currentTurnPhase = BATTLE_PHASE;
        timer.restart();
        while (timer.getElapsedTime().asSeconds() < 1.5f) {}
        user->receiveDamage(-2);
        if (!user->equipCards.empty()) {
            engine.board->discardCard(user->equipCards.back());
            user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        }
        isDone = true;
    }

    void WhiteAidCommand::execute() {
        if (isWaitingForTarget) {
            engine.isWaitingForCardEffectTargetPrompt = true;
            engine.waitingCommand = this;
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
            engine.isWaitingForCardEffectTargetPrompt = false;
            isDone = true;
        }
    }

    void WhiteAidCommand::executeAI() {
        engine.currentTurnPhase = BATTLE_PHASE;
        if (target != nullptr) {
            std::cout << "White Aid AI: Player:" << target->id << " Wounds: " << target->wounds << std::endl;
            target->wounds = 0;
            target->receiveDamage(7);
            std::cout << "White Aid AI: Player:" << target->id << " Wounds: " << target->wounds << std::endl;
        }
        engine.board->discardCard(user->equipCards.back());
        user->equipCards.erase(std::find(user->equipCards.begin(),user->equipCards.end(),user->equipCards.back()));
        engine.isWaitingForCardEffectTargetPrompt = false;
        isDone = true;
        
    }

    void WhiteAidCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardEffectTargetPrompt = false;
        isWaitingForTarget = false;
    }

//============================================================================
// NeedTarget
    bool DarkSpiderCommand::needTarget() {
        return true;
    }
    bool DarkDollCommand::needTarget() {
        return true;
    }
    bool DarkBatCommand::needTarget() {
        return true;
    }
    bool WhiteFlareCommand::needTarget() {
        return false;
    }
    bool WhiteWaterCommand::needTarget() {
        return false;
    }
    bool WhiteAidCommand::needTarget() {
        return true;
    }

//============================================================================
// ReceiveAnswer
    void DarkSpiderCommand::receiveAnswer(void* answer) {
        target = static_cast<state::Player*>(answer);
        if(target != nullptr){
            std::cout << "Dark Spider target received: Player ID " << target->id << std::endl;
        }
        else {
            std::cout << "Dark Spider target received: nullptr" << std::endl;
        }
    }
    void DarkDollCommand::receiveAnswer(void* answer) {
        target = static_cast<state::Player*>(answer);
        if(target != nullptr){
            std::cout << "Dark Doll target received: Player ID " << target->id << std::endl;
        }
        else {
            std::cout << "Dark Doll target received: nullptr" << std::endl;
        }
    }
    void DarkBatCommand::receiveAnswer(void* answer) {
        target = static_cast<state::Player*>(answer);
        if(target != nullptr){
            std::cout << "Dark Bat target received: Player ID " << target->id << std::endl;
        }
        else {
            std::cout << "Dark Bat target received: nullptr" << std::endl;
        }
    }
    void WhiteFlareCommand::receiveAnswer(void* answer) {
    }
    void WhiteWaterCommand::receiveAnswer(void* answer) {
    }
    void WhiteAidCommand::receiveAnswer(void* answer) {
        target = static_cast<state::Player*>(answer);   
    }
    
}