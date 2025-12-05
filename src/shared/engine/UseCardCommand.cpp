#include "UseCardCommand.h"
#include "Engine.h"

using namespace engine;
using namespace state;

UseCardCommand::UseCardCommand(Engine& engine, state::Card card)
    : engine(engine), card(card)
{
}

void UseCardCommand::execute()
{
    // On récupère le joueur actif (à adapter si ton Engine fonctionne différemment)
    state::Player& currentPlayer = engine.getCurrentPlayer();
    isDone = true;
}
