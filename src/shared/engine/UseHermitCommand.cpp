#include "UseHermitCommand.h"
#include "Engine.h"
#include <algorithm>
using namespace engine;
using namespace state;

UseHermitCommand::UseHermitCommand(Engine& engine, state::CardClass card)
    : engine(engine), card(card)
{
}

void UseHermitCommand::execute()
{
    state::Player& currentPlayer = engine.getCurrentPlayer();
    if (card.effectTimer == INSTANT){
        engine.board->discardCard(&card);
        currentPlayer.equipCards.erase(std::find(currentPlayer.equipCards.begin(),currentPlayer.equipCards.end(),&card));
    }
    engine.currentTurnPhase = BATTLE_PHASE;
    isDone = true;
}