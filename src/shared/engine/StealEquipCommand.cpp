#include "StealEquipCommand.h"
#include "Engine.h"
#include <algorithm>

namespace engine {

    StealEquipCommand::StealEquipCommand(Engine& engine, state::Player* thief)
        : engine(engine), thief(thief)
    {}

    void StealEquipCommand::execute() {
        if (isPromptCancelled) {
            engine.currentTurnPhase = BATTLE_PHASE;
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardStealPrompt = false;
            }
            isDone = true;
            return;
        }
        if (isWaitingForCard) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardStealPrompt = true;
                engine.waitingCommand = this;
            }
            return;
        }
        else {
            engine.currentTurnPhase = BATTLE_PHASE;
            if (equipCard != nullptr) {
                for (auto& player : engine.board->playerList) {
                    if (player.get() != thief) {
                        auto& equipCards = player->equipCards;
                        auto it = std::find(equipCards.begin(), equipCards.end(), equipCard);
                        if (it != equipCards.end()) {
                            equipCards.erase(it);
                            thief->equipACard(equipCard);
                            break;
                        }
                    }
                }
            }
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCardStealPrompt = false;
            }
            isDone = true;
        }
    }

    void StealEquipCommand::receivePromptAnswer(void* answer) {
        state::CardClass* chosenCard = static_cast<state::CardClass*>(answer);
        if (chosenCard == nullptr) {
            isPromptCancelled = true;
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.waitingCommand = nullptr;
                engine.isWaitingForCardStealPrompt = false;
            }
        }
        else {
            equipCard = chosenCard;
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.waitingCommand = nullptr;
                engine.isWaitingForCardStealPrompt = false;
            }
            isWaitingForCard = false;
        }
    }
}