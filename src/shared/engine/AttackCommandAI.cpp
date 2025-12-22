#include "AttackCommandAI.h"
#include "state/Player.h" 
#include <iostream>

namespace engine {

    AttackCommandAI::AttackCommandAI(Engine& engine, state::Player* attacker,state::Player* attacked)
        : engine(engine), attacker(attacker), attacked(attacked)
    {
    }

    void AttackCommandAI::execute()
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
        engine.goToNextPlayer();
        engine.currentTurnPhase = MOVE_PHASE;
        isDone = true;
    }
}
