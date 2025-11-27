#include "MoveCommand.h"

namespace engine {

MoveCommand::MoveCommand(state::CellClass position) : position(position)
{}

void MoveCommand::execute(Engine& engine)
{
    //engine.playerMove(player);
}

}
