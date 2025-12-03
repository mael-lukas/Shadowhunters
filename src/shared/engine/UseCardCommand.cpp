#include "UseCardCommand.h"
#include "Engine.h"

using namespace engine;
using namespace state;

UseCardCommand::UseCardCommand(state::Card card)
    : card(card)
{
}

void UseCardCommand::execute(Engine& engine)
{
    // On récupère le joueur actif (à adapter si ton Engine fonctionne différemment)
    state::Player& currentPlayer = engine.getCurrentPlayer();
    isDone = true;
}
