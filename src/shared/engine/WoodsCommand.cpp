#include "WoodsCommand.h"
#include "Engine.h"

#include <iostream>

namespace engine {
    WoodsCommand::WoodsCommand(Engine& engine)
        : engine(engine)
    {}

    void WoodsCommand::execute()
    {
        if (isPromptCancelled) {
            engine.currentTurnPhase = BATTLE_PHASE;
            engine.isWaitingForWoodsPrompt = false;
            isDone = true;
            return;
        }
        if (isWaitingForTarget) {
            engine.isWaitingForWoodsPrompt = true;
            engine.waitingCommand = this;
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if (target != nullptr) {
                target->receiveDamage(damage);
            }
            engine.isWaitingForWoodsPrompt = false;
            isDone = true;
        }   
    }

     void WoodsCommand::executeAI()
    {
        switch(woods_Choice) {
            case ATTACK:
                std::cout<< "--------WoodsCommandAI: ATTACK"  << std::endl;
                damage = 2;
                target->receiveDamage(damage);
                isDone = true;
                break;
            case HEAL:
                std::cout<< "--------WoodsCommandAI: HEAL"  << std::endl;
                damage = -1;
                break;
            default:
                std::cout<< "--------WoodsCommandAI: CANCEL"  << std::endl;
                engine.currentTurnPhase = BATTLE_PHASE;
                isDone = true;
                return;
        }
        engine.currentTurnPhase = BATTLE_PHASE;
        if (target != nullptr) {
            target->receiveDamage(damage);
        }
        isDone = true;
    }

    void WoodsCommand::receivePromptAnswer(void* answer)
    {
        int targetID = *static_cast<int*>(answer);
        if (targetID == -1) {
            isPromptCancelled = true;
            engine.waitingCommand = nullptr;
            engine.isWaitingForWoodsPrompt = false;
        }
        else {
            if (targetID%2 == 0) {
                damage = 2;
            }
            else {
                damage = -1;
            }
            target = engine.board->playerList[targetID/2].get();
            engine.waitingCommand = nullptr;
            engine.isWaitingForWoodsPrompt = false;
            isWaitingForTarget = false;
        }
    }

    bool WoodsCommand::needTarget() {
        return true;
    }

    void WoodsCommand::receiveAnswer(void* answer) {
    }
}