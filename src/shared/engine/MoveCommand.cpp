#include "MoveCommand.h"
#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include "state/CellClass.h"
#include "state/RollRule.h"

namespace engine {

MoveCommand::MoveCommand(Engine& engine)
    : engine(engine)
{}

void MoveCommand::execute()
{
    state::Player& player = engine.getCurrentPlayer();
    state::Board& board = engine.getBoard();

    state::CellClass *oldPos = player.position;
    state::CellClass *newPos = oldPos;


    while (newPos == oldPos) {
        int die = board.rollDice(state::RollRule::SUM);
        newPos = board.dieToCell(die);
    }

    engine.currentTurnPhase = BATTLE_PHASE;
    board.movePlayerTo(&player, newPos);
    isDone = true;
}

}
