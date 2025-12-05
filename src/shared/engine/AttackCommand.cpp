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
        if (isWaitingForTarget) {
            engine.isWaitingForTargetPrompt = true;
            engine.waitingCommand = this;
            return;
        }
        else {
            std::cout << "[ENGINE] Executing AttackCommand: Player " << attacker->id << " attacks Player " << attacked->id << std::endl;
            attacker->attackOther(*attacked);
            engine.isWaitingForTargetPrompt = false;
            engine.goToNextPlayer();
            isDone = true;
        }
        
    }

    void AttackCommand::receivePromptAnswer(void* answer)
    {
        int targetID = *static_cast<int*>(answer);
        attacker = &engine.getCurrentPlayer();
        std::cout << "current player ID: " << engine.getCurrentPlayer().id << std::endl;
        std::cout << "attacker player ID: " << attacker->id << std::endl;
        attacked = engine.board->playerList[targetID].get();
        std::cout << "[ENGINE] AttackCommand received target ID: " << attacked->id << std::endl;
        engine.waitingCommand = nullptr;
        engine.isWaitingForTargetPrompt = false;
        isWaitingForTarget = false;

    }
}
