#include "DrawCardCommand.h"
#include "state/CardClass.h"
#include <iostream>

namespace engine
{

    DrawCardCommand::DrawCardCommand(Engine& engine, state::CardType type) : engine(engine)
    {
        cardType = type;
    }
    void DrawCardCommand::execute()
    {
        state::Player& currentPlayer = engine.getCurrentPlayer();

        if (cardType == state::WHITE)
            currentPlayer.equipCards.push_back(engine.board->drawWhite());
        if (cardType == state::DARK)
            currentPlayer.equipCards.push_back(engine.board->drawDark());
        if (cardType == state::HERMIT){
            engine.board->hermitPack.discard(engine.board->drawHermit());
        }
        isDone = true;
    }

    void DrawCardCommand::receivePromptAnswer(void* answer)
    {

    }
}