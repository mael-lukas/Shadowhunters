#include "MoveCommand.h"

namespace engine {

MoveCommand::MoveCommand(state::Player& player)
    : player(player)
{}

void MoveCommand::execute(Engine& engine)
{
    engine.playerMove(player);
}

}
