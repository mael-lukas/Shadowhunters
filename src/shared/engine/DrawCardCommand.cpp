#include "DrawCardCommand.h"
#include "state/CardClass.h"
#include <iostream>

namespace engine
{

    DrawCardCommand::DrawCardCommand(state::CardType type)
    {
        cardType = type;
    }
    void DrawCardCommand::execute(Engine &engine)
    {
        if (cardType == state::WHITE)
            engine.getCurrentPlayer().equipCards.push_back(engine.board->drawDark());
        if (cardType == state::DARK)
            engine.getCurrentPlayer().equipCards.push_back(engine.board->drawDark());
        if (cardType == state::HERMIT){
            engine.board->hermitPack.discard(engine.board->drawHermit());
        }
        isDone = true;
    }
        
}