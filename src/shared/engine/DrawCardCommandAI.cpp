#include "DrawCardCommandAI.h"
#include "state/CardClass.h"
#include <iostream>
#include "UseCardCommand.h"

namespace engine
{

    DrawCardCommandAI::DrawCardCommandAI(Engine& engine, state::CardType type,bool draw) : engine(engine),cardType(type),draw(draw)
    {}

    void DrawCardCommandAI::execute()
    {
        if(draw){
            drawCard();
        }
        engine.currentTurnPhase = BATTLE_PHASE;
        engine.isWaitingForYesNoPrompt = false;
        isDone = true;
    }

    void DrawCardCommandAI::drawCard(){
        state::Player& currentPlayer = engine.getCurrentPlayer();
        state::CardClass* card;
        if (cardType == state::WHITE){
            card = engine.board->drawWhite();
        }
        if (cardType == state::DARK){
            card = engine.board->drawDark();
        }
        if (cardType == state::HERMIT){
            card = engine.board->drawHermit();
        }
        currentPlayer.equipCards.push_back(card);
        engine.commands.emplace_back(new UseCardCommand(engine,*card));
    }

}