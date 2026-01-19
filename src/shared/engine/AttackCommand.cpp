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
            {   std::lock_guard<std::mutex> lock(engine.turnPhaseMutex);
                engine.currentTurnPhase = MOVE_PHASE;
            }
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.waitingCommand = nullptr;
                engine.isWaitingForAttackPrompt = false;
            }
            engine.goToNextPlayer();
            isDone = true;
            return;
        }
        if (isWaitingForTarget) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForAttackPrompt = true;
                engine.waitingCommand = this;
            }
            return;
        }
        else {
            {   std::lock_guard<std::mutex> lock(engine.turnPhaseMutex);
                engine.currentTurnPhase = MOVE_PHASE;
            }
            attacker->attackOther(*attacked);
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForAttackPrompt = false;
            }
            engine.goToNextPlayer();
            isDone = true;
        }
        
    }

    void AttackCommand::receivePromptAnswer(void* answer)
    {
        int targetID = *static_cast<int*>(answer);
        if (targetID == -1) {
            isAttackCancelled = true;
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.waitingCommand = nullptr;
                engine.isWaitingForAttackPrompt = false;
            }
        }
        else {
            attacker = &engine.getCurrentPlayer();
            attacked = engine.board->playerList[targetID].get();
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.waitingCommand = nullptr;
                engine.isWaitingForAttackPrompt = false;
            }
            isWaitingForTarget = false;
        }
    }
}
