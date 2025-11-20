#include "AttackCommand.h"
#include "state/Player.h" 

namespace engine {

    AttackCommand::AttackCommand(state::Player& attacker, state::Player& attacked)
        : attacker(attacker), attacked(attacked)
    {
    }

    void AttackCommand::execute(Engine& engine)
    {
        // Pour l’instant, on n’utilise pas directement l’engine
        (void)engine; // évite un warning “unused parameter”

        // Toute la logique d’attaque est dans Player::attackOther
        attacker.attackOther(attacked);
    }

}
