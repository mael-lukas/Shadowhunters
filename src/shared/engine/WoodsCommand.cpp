#include "WoodsCommand.h"
#include "Engine.h"

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
}