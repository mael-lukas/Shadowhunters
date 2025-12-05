#include "DrawCardCommand.h"
#include "state/CardClass.h"
#include "UseCardCommand.h"
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
            currentPlayer.equipCards.push_back(engine.board->drawDark());
        if (cardType == state::DARK)
            currentPlayer.equipCards.push_back(engine.board->drawDark());
        if (cardType == state::HERMIT){
            currentPlayer.equipCards.push_back(engine.board->drawHermit());
        }
        engine.commands.push_back(new UseCardCommand(engine,*currentPlayer.equipCards.back()));
        isDone = true;
    }
}