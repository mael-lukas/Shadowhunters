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
        std::cout << "Executing AttackCommand" << std::endl;
        if (isAttackCancelled) {
            engine.currentTurnPhase = MOVE_PHASE;
            std::cout << "Attack cancelled." << std::endl;
            engine.isWaitingForTargetPrompt = false;
            std::cout << "Player actuelle: " << engine.getCurrentPlayer().id << std::endl;
            engine.goToNextPlayer();
            std::cout << "Player future: " << engine.getCurrentPlayer().id << std::endl;
            isDone = true;
            return;
        }
        if (isWaitingForTarget) {
            engine.isWaitingForTargetPrompt = true;
            engine.waitingCommand = this;
            return;
        }
        else {
            engine.currentTurnPhase = MOVE_PHASE;
            attacker->attackOther(*attacked);
            std::cout << "Target attacked." << std::endl;
            engine.isWaitingForTargetPrompt = false;
            std::cout << "Player actuelle: " << engine.getCurrentPlayer().id << std::endl;
            engine.goToNextPlayer();
            std::cout << "Player future: " << engine.getCurrentPlayer().id << std::endl;
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
