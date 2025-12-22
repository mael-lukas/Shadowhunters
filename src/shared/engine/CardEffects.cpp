#include "Engine.h"
#include "DarkBatCommand.h"
#include "DarkDollCommand.h"
#include "DarkSpiderCommand.h"
#include <iostream>
#include <algorithm>

namespace engine {
    DarkSpiderCommand::DarkSpiderCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    DarkDollCommand::DarkDollCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    DarkBatCommand::DarkBatCommand(Engine& engine, state::Player* user): engine(engine), user(user)
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

    void DarkBatCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardEffectTargetPrompt = false;
        isWaitingForTarget = false;
    }

    
}