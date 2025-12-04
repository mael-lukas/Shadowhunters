#include "MoveCommand.h"
#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include "state/CellClass.h"
#include "state/RollRule.h"

namespace engine {

MoveCommand::MoveCommand(state::CellClass* position)
    : position(position)
{}

void MoveCommand::execute(Engine& engine)
{
    state::Player& player = engine.getCurrentPlayer();
    state::Board& board = engine.getBoard();

    state::CellClass *oldPos = player.position;
    state::CellClass *newPos = oldPos;


    while (newPos == oldPos)
    {
        int die = board.rollDice(state::RollRule::SUM);
        newPos = board.dieToCell(die);

    }

    board.movePlayerTo(&player, newPos);

    isDone = true;
}

}
