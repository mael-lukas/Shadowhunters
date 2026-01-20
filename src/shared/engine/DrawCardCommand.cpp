#include "DrawCardCommand.h"
#include "state/CardClass.h"
#include "UseHermitCommand.h"
#include <iostream>

namespace engine
{

    DrawCardCommand::DrawCardCommand(Engine& engine, state::CardType type) : engine(engine)
    {
        cardType = type;
    }
    DrawCardCommand::DrawCardCommand(Engine& engine,state::CardType type,bool draw): engine(engine),isWaitingForValidation(false),draw(draw)
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
            engine.currentTurnPhase = BATTLE_PHASE;
            if(draw){
                engine.currentTurnPhase = CARD_EFFECT_PHASE;
                drawCard();
            }
            engine.isWaitingForYesNoPrompt = false;
            isDone = true;
        }
    }

    void DrawCardCommand::executeAI(){
        engine.currentTurnPhase = BATTLE_PHASE;
        if(draw){
            engine.currentTurnPhase = CARD_EFFECT_PHASE;
            drawCard();
        }
        draw = false;
        engine.isWaitingForYesNoPrompt = false;
        isDone = true;
    }
    void DrawCardCommand::drawCard(){
        state::Player& currentPlayer = engine.getCurrentPlayer();
        state::CardClass* card;
        if (cardType == state::WHITE){
            card = engine.board->drawWhite();
        }
        if (cardType == state::DARK){
            std::cout << "Drawing DARK card" << std::endl;
            card = engine.board->drawDark();
        }
        if (cardType == state::HERMIT){
            card = engine.board->drawHermit();
        }
        currentPlayer.equipCards.push_back(card);
        if (cardType == state::HERMIT) {
            engine.commands.emplace_back(new UseHermitCommand(engine, *card));
            //engine.commands.insert(engine.commands.begin(), new UseHermitCommand(engine, *card));
        }
        else {
            engine.commands.emplace_back(engine.cardEffectsFactory[card->name](engine));
            //engine.commands.insert(engine.commands.begin(), engine.cardEffectsFactory[card->name](engine));
        }
    }
    
    void DrawCardCommand::receivePromptAnswer(void* answer){
        draw = *static_cast<bool*>(answer);
        //std::cout << "answer is : " << static_cast<int>(draw) << std::endl;
        engine.waitingCommand = nullptr;
        isWaitingForValidation = false;
    }

    bool DrawCardCommand::needTarget() {
        return false;
    }

    void DrawCardCommand::receiveAnswer(void* answer) {
        draw = *static_cast<bool*>(answer);
        std::cout << "receive answer called in DrawCardCommand, draw = " << draw << std::endl;
        engine.waitingCommand = nullptr;
        isWaitingForValidation = false;
    }
}