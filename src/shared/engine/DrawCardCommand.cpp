#include "DrawCardCommand.h"
#include "state/CardClass.h"
#include <iostream>
#include "UseCardCommand.h"

namespace engine
{

    DrawCardCommand::DrawCardCommand(Engine& engine, state::CardType type) : engine(engine)
    {
        cardType = type;
    }

    void DrawCardCommand::execute()
    {
        if (isWaitingForValidation) {
            engine.isWaitingForYesNoPrompt = true;
            engine.waitingCommand = this;
            return;
        }
        else {
            if(draw){
                drawCard();
            }
            engine.currentTurnPhase = BATTLE_PHASE;
            engine.isWaitingForYesNoPrompt = false;
            isDone = true;
        }
    }

    void DrawCardCommand::drawCard(){
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
    
    void DrawCardCommand::receivePromptAnswer(void* answer){
        draw = *static_cast<bool*>(answer);
        std::cout << "answer is : " << static_cast<int>(draw) << std::endl;
        engine.waitingCommand = nullptr;
        isWaitingForValidation = false;
    }

}