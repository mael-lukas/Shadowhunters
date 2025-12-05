#include "DrawCardCommand.h"
#include "state/CardClass.h"
#include <iostream>

namespace engine
{

    DrawCardCommand::DrawCardCommand(state::CardType type):cardType(type)
    {}
    void DrawCardCommand::execute(Engine &engine)
    {
        state::Player& currentPlayer = engine.getCurrentPlayer();

        if (cardType == state::WHITE)
            currentPlayer.equipCards.push_back(engine.board->drawDark());
        if (cardType == state::DARK)
            currentPlayer.equipCards.push_back(engine.board->drawDark());
        if (cardType == state::HERMIT){
            engine.board->hermitPack.discard(engine.board->drawHermit());
        }
        isDone = true;
    }
}