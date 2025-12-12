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
        attacker->attackOther(*attacked);
        engine.goToNextPlayer();
        isDone = true;
    }
}
