#include "DrawCardCommand.h"

namespace engine
{

    DrawCardCommand::DrawCardCommand(Engine& engine, state::CardType type) : engine(engine)
    {
        cardType = type;
    }
    void DrawCardCommand::execute()
    {
        // if (cardType == state::WHITE)
        //     engine.getCurrentPlayer().equipCards.push_back(engine.board->drawWhite());
        // if (cardType == state::DARK)
        //     engine.getCurrentPlayer().equipCards.push_back(engine.board->drawDark());
        // if (cardType == state::HERMIT)
        //     engine.getCurrentPlayer().equipCards.push_back(engine.board->drawHermit());
        isDone = true;
    }
}