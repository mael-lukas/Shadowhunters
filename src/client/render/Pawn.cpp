#include "Pawn.h"
#include <SFML/Graphics.hpp>

namespace render {

    Pawn::Pawn(PawnType type, state::Player* owner, sf::CircleShape* shape, int cellId) :
    type(type),
    owner(owner),
    shape(shape),
    cellId(cellId) {}

}