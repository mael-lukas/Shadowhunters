#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"

namespace engine {

void Engine::playerMove(state::Player& player)
{
    // 1) Lancer les dés (somme)
    int roll = board.rollDice(state::SUM);

    // 2) Trouver la case associée
    state::Cell destination = board.dieToCell[roll];

    // 3) Déplacer dans Board
    board.movePlayerToCell(player, destination);

    // 4) Mettre à jour le Player
    player.position = destination;

}

}
