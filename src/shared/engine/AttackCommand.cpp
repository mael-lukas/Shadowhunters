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
        // if (isWaitingForTarget) {
        //     engine.isWaitingForPrompt = true;
        //     return;
        // }
        // else {
        //     attacker.attackOther(attacked);
        //     engine.isWaitingForPrompt = false;
        //     isDone = true;
        // }
        int pl = rand() % 4;
        engine.board->playerList[pl]->receiveDamage(1);
        isDone = true;
    }
}
