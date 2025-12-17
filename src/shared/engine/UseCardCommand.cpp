#include "UseCardCommand.h"
#include "Engine.h"
#include <iostream>
#include <algorithm>
using namespace engine;
using namespace state;

UseCardCommand::UseCardCommand(Engine& engine, state::CardClass card)
    : engine(engine), card(card)
{
}

void UseCardCommand::execute()
{
    state::Player& currentPlayer = engine.getCurrentPlayer();
    if (card.effectTimer == INSTANT){
        currentPlayer.equipCards.erase(std::find(currentPlayer.equipCards.begin(),currentPlayer.equipCards.end(),&card));
        engine.board->discardCard(&card);
    }
    isDone = true;
}
