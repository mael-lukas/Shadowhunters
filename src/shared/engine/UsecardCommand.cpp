#include "UsecardCommand.h"
#include "Engine.h"

using namespace engine;
using namespace state;

UsecardCommand::UsecardCommand(state::Card card)
    : card(card)
{
}

void UsecardCommand::execute(Engine& engine)
{
    // On récupère le joueur actif (à adapter si ton Engine fonctionne différemment)
    state::Player& currentPlayer = engine.getCurrentPlayer();

    
}
