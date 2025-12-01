#include "MoveCommand.h"
#include "Engine.h"
#include "state/Player.h"

namespace engine {

MoveCommand::MoveCommand(state::CellClass* position) : position(position)
{}

    void MoveCommand::execute(Engine& engine)
    {
        // Pour l’instant, on laisse le moteur gérer entièrement
        // la logique de déplacement (lancer de dés, etc.).
        //
        // 'position' est stocké dans la commande et pourra servir plus tard.
        // Pour éviter un warning "unused", on fait :
        (void)position;

        // L'objectif sera de modifier le joueur courant pour lui
        // assigner une nouvelle position.
        //engine.playerMove(player);
    }

}
