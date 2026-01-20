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
            std::cout << "Attack Cancelled" << std::endl;
            std::cout << "=============END Of The Turn=============== "<< std::endl;
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
        std::cout << "Attack executed." << std::endl;
        std::cout << "=============END Of The Turn=============== "<< std::endl;
    }

    void AttackCommand::executeAI()
    {
        //std::cout << "[AI] Attacker ID: " << attacker->id << "[AI] target ID: " << attacked->id << std::endl;
        if(attacked == nullptr || attacked == attacker) {
            std::cout << "[AI] " << attacker->id << " chooses not to attack." << std::endl;
            engine.goToNextPlayer();
            engine.currentTurnPhase = MOVE_PHASE;
            isDone = true;
            std::cout << "Attack Cancelled" << std::endl;
            std::cout << "=============END Of The Turn=============== "<< std::endl;
            return;
        }
        std::cout << "[AI] " << attacker->id << " attacks " << attacked->id << std::endl;
        attacker->attackOther(*attacked);
        attacked = nullptr;
        engine.goToNextPlayer();
        engine.currentTurnPhase = MOVE_PHASE;
        isDone = true;
        std::cout << "Attack executed." << std::endl;
        std::cout << "=============END Of The Turn=============== "<< std::endl;
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

    bool AttackCommand::needTarget() {
        return true;
    }

    void AttackCommand::receiveAnswer(void* answer) {
        std::cout << "Receive Answer called in AttackCommand" << std::endl;
        attacked = static_cast<state::Player*>(answer);
        if(attacked == nullptr) {
            std::cout << "Attack cancelled via receiveAnswer" << std::endl;
        }
        else {
            std::cout << "Target selected via receiveAnswer: Player " << attacked->id << std::endl;
        }
    }
}
