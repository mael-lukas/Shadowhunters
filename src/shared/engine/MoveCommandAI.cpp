#include "MoveCommandAI.h"
#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include "state/CellClass.h"
#include "state/RollRule.h"

namespace engine {

    MoveCommandAI::MoveCommandAI(Engine& engine)
        : engine(engine)
    {}

    void MoveCommandAI::execute()
    {

        state::Player& player = engine.getCurrentPlayer();
        state::CellClass *oldPos = player.position;
        state::CellClass *newPos = oldPos;

        while (newPos == oldPos) {
            int die;
            do {
                die = engine.board->rollDice(state::RollRule::SUM);
            } while (die == 7);
            newPos = engine.board->dieToCell(die);
        }
        engine.currentTurnPhase = BATTLE_PHASE;
        engine.board->movePlayerTo(&player, newPos);
        isDone = true;
        
    }
}