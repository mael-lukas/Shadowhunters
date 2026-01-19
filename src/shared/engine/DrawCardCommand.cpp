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
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForYesNoPrompt = true;
                engine.waitingCommand = this;
            }
            engine.customPromptText = "Do you want to draw a card?";
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if(draw){
                engine.currentTurnPhase = CARD_EFFECT_PHASE;
                drawCard();
            }
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForYesNoPrompt = false;
            }
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
        card = new state::CardClass(state::RITUAL1, state::DARK, state::INSTANT);
        currentPlayer.equipCards.push_back(card);
        if (cardType == state::HERMIT) {
            engine.commands.emplace_back(new UseHermitCommand(engine, *card));
        }
        else {
            engine.commands.emplace_back(engine.cardEffectsFactory[card->name](engine));
        }
    }
    
    void DrawCardCommand::receivePromptAnswer(void* answer){
        draw = *static_cast<bool*>(answer);
        std::cout << "answer is : " << static_cast<int>(draw) << std::endl;
        {  std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForYesNoPrompt = false;
        }
        isWaitingForValidation = false;
    }
}