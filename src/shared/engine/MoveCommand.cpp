#include "MoveCommand.h"
#include "Engine.h"
#include "state/Player.h"

namespace engine {

MoveCommand::MoveCommand(state::Cell position, state::Player& player)
    : position(position)
    , player(player)
{
}

void MoveCommand::execute(Engine& engine)
{
    // Pour l’instant, on laisse le moteur gérer entièrement
    // la logique de déplacement (lancer de dés, etc.).
    //
    // 'position' est stocké dans la commande et pourra servir plus tard.
    // Pour éviter un warning "unused", on fait :
    (void)position;

    engine.playerMove(player);
}

}
