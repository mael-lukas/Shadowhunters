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
            std::cout << "Player actuelle: " << engine.getCurrentPlayer().id << std::endl;
            engine.goToNextPlayer();
            std::cout << "Player future: " << engine.getCurrentPlayer().id << std::endl;
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
