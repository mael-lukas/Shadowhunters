#include "AttackCommand.h"
#include "state/Player.h" 
#include <iostream>

namespace engine {

    AttackCommand::AttackCommand(Engine& engine, state::Player* attacker)
        : engine(engine), attacker(attacker)
    {
    }

    void AttackCommand::execute()
    {
        if (isAttackCancelled) {
            engine.isWaitingForTargetPrompt = false;
            engine.goToNextPlayer();
            isDone = true;
            return;
        }
        if (isWaitingForTarget) {
            engine.isWaitingForTargetPrompt = true;
            engine.waitingCommand = this;
            return;
        }
        else {
            attacker->attackOther(*attacked);
            engine.isWaitingForTargetPrompt = false;
            engine.goToNextPlayer();
            isDone = true;
        }
        
    }

    void AttackCommand::receivePromptAnswer(void* answer)
    {
        int targetID = *static_cast<int*>(answer);
        if (targetID == -1) {
            isAttackCancelled = true;
            engine.waitingCommand = nullptr;
            engine.isWaitingForTargetPrompt = false;
        }
        else {
            attacker = &engine.getCurrentPlayer();
            attacked = engine.board->playerList[targetID].get();
            engine.waitingCommand = nullptr;
            engine.isWaitingForTargetPrompt = false;
            isWaitingForTarget = false;
        }


    }
}
