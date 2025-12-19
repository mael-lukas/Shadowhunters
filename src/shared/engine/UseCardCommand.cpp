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
        // Handle effects before discarding
        if (card.name == DARK1) {
            if (target == nullptr) {
                isWaitingForPrompt = true;
                engine.isWaitingForCardTargetPrompt = true;
                engine.waitingCommand = this;
                return;
            } else {
                // Apply effect: deal 2 damage to target, heal self by 1
                target->receiveDamage(2);
                currentPlayer.receiveDamage(-1);
            }
        }
        if (card.name == DARK2) {
            if (target == nullptr) {
                std::cout<<"Waiting for target prompt..."<<std::endl;
                isWaitingForPrompt = true;
                engine.isWaitingForCardTargetPrompt = true;
                engine.waitingCommand = this;
                return;
            } else {
                // Apply effect: deal 2 damage to target, heal self by 1
                target->receiveDamage(2);
                currentPlayer.receiveDamage(-1);
            }
        }
        currentPlayer.equipCards.erase(std::find(currentPlayer.equipCards.begin(),currentPlayer.equipCards.end(),&card));
        engine.board->discardCard(&card);
    } else if (card.effectTimer == ONELOOP){
        currentPlayer.equipCards.push_back(&card);
    } else {
        currentPlayer.equipCards.push_back(&card);
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