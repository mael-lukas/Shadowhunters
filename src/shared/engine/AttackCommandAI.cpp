#include "AttackCommandAI.h"
#include "state/Player.h" 
#include <iostream>
#include <thread>
#include <chrono>

namespace engine {

    AttackCommandAI::AttackCommandAI(Engine& engine, state::Player* attacker,state::Player* attacked)
        : engine(engine), attacker(attacker), attacked(attacked)
    {
    }

    void AttackCommandAI::execute()
    {
        if(attacked == nullptr) {
            engine.goToNextPlayer();
            engine.currentTurnPhase = MOVE_PHASE;
            isDone = true;
            return;
        }
        
        attacker->attackOther(*attacked);
        engine.goToNextPlayer();
        engine.currentTurnPhase = MOVE_PHASE;
        isDone = true;
    }
}
