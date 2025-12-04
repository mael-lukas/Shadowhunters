#include "AttackCommand.h"
#include "state/Player.h" 
#include <iostream>

namespace engine {

    AttackCommand::AttackCommand(state::Player& attacker, state::Player& attacked)
        : attacker(attacker), attacked(attacked)
    {
    }

    void AttackCommand::execute(Engine& engine)
    {
        if (isWaitingForTarget) {
            engine.isWaitingForTargetPrompt = true;
            return;
        }
        else {
            attacker.attackOther(attacked);
            engine.isWaitingForTargetPrompt = false;
            isDone = true;
        }
        
    }
}
