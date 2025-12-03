#include "MoveCommand.h"
#include "Engine.h"
#include "state/Player.h"

namespace engine {

MoveCommand::MoveCommand(state::CellClass* position) : position(position)
{}

    void MoveCommand::execute(Engine& engine)
    {
        isDone = true;
    }

}
