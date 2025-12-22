#include "StealEquipCommand.h"
#include "Engine.h"

namespace engine {

    StealEquipCommand::StealEquipCommand(Engine& engine, state::Player* thief)
        : engine(engine), thief(thief)
    {}

    void StealEquipCommand::execute() {
        
    }

    void StealEquipCommand::receivePromptAnswer(void* answer) {
        if (isPromptCancelled) {
        }
        if (isWaitingForVictim) {
        }
        if (isWaitingForCard) {
        }
    }
}