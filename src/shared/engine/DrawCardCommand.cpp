#include "DrawCardCommand.h"

namespace engine
{

    DrawCardCommand::DrawCardCommand(state::CardType type)
    {
        cardType = type;
    }
    void DrawCardCommand::execute(Engine &engine)
    {
        if (cardType == state::WHITE)
            engine.getCurrentPlayer().equipCards.push_back(engine.board -> drawWhite());
        if (cardType == state::DARK)
            engine.getCurrentPlayer().equipCards.push_back(engine.board -> drawDark());
        if (cardType == state::HERMIT)
            engine.getCurrentPlayer().equipCards.push_back(engine.board -> drawHermit());
    }
}