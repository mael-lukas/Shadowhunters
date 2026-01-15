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
            engine.currentTurnPhase = MOVE_PHASE;
            engine.isWaitingForAttackPrompt = false;
            engine.goToNextPlayer();
            isDone = true;
            return;
        }
        if (isWaitingForTarget) {
            engine.isWaitingForAttackPrompt = true;
            engine.waitingCommand = this;
            return;
        }
        else {
            engine.currentTurnPhase = MOVE_PHASE;
            attacker->attackOther(*attacked);
            engine.isWaitingForAttackPrompt = false;
            engine.goToNextPlayer();
            isDone = true;
        }
    }

    void AttackCommand::executeAI()
    {
        if(attacked == nullptr) {
            std::cout << "[AI] " << attacker->id << " chooses not to attack." << std::endl;
            engine.goToNextPlayer();
            engine.currentTurnPhase = MOVE_PHASE;
            isDone = true;
            return;
        }
        std::cout << "[AI] " << attacker->id << " attacks " << attacked->id << std::endl;
        attacker->attackOther(*attacked);
        attacked = nullptr;
        engine.goToNextPlayer();
        engine.currentTurnPhase = MOVE_PHASE;
        isDone = true;
    }

    void AttackCommand::receivePromptAnswer(void* answer)
    {
        int targetID = *static_cast<int*>(answer);
        if (targetID == -1) {
            isAttackCancelled = true;
            engine.waitingCommand = nullptr;
            engine.isWaitingForAttackPrompt = false;
        }
        else {
            attacker = &engine.getCurrentPlayer();
            attacked = engine.board->playerList[targetID].get();
            engine.waitingCommand = nullptr;
            engine.isWaitingForAttackPrompt = false;
            isWaitingForTarget = false;
        }
    }
}
