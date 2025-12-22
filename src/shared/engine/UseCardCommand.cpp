#include "UseCardCommand.h"
#include "Engine.h"
#include <iostream>
#include <algorithm>
using namespace engine;
using namespace state;

UseCardCommand::UseCardCommand(Engine& engine, state::CardClass card)
    : engine(engine), card(card)
{
}

void UseCardCommand::execute()
{
    state::Player& currentPlayer = engine.getCurrentPlayer();
    if (card.effectTimer == INSTANT){
        
        engine.board->discardCard(&card);
        currentPlayer.equipCards.erase(std::find(currentPlayer.equipCards.begin(),currentPlayer.equipCards.end(),&card));
    }
    isDone = true;
}

void UseCardCommand::receivePromptAnswer(void* answer)
{
    int targetID = *static_cast<int*>(answer);
    if (targetID == -1) {
        // Cancel, perhaps do nothing or discard
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardTargetPrompt = false;
        isWaitingForPrompt = false;
        // Still discard the card
        state::Player& currentPlayer = engine.getCurrentPlayer();
        isDone = true;
    } else {
        target = engine.board->playerList[targetID].get();
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardTargetPrompt = false;
        isWaitingForPrompt = false;
        // Now execute the effect
        execute();
    }
}