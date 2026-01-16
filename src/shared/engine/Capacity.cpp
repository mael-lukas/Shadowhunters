#include "FranklinCapacityCommand.h"
#include "GeorgesCapacityCommand.h"

namespace engine {
  FranklinCapacityCommand::FranklinCapacityCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}
    GeorgesCapacityCommand::GeorgesCapacityCommand(Engine& engine, state::Player* user): engine(engine), user(user)
    {}

    void FranklinCapacityCommand::execute() {
        if (isWaitingForTarget) {
            engine.isWaitingForCardEffectTargetPrompt = true;
            engine.waitingCommand = this;
            return;
        }
        else {
            if (target != nullptr) {
                int dmg = engine.board->rollDice(state::ONLYD6);
                target->receiveDamage(dmg);
                user->capacityUsed = true;
            }
            engine.isWaitingForCardEffectTargetPrompt = false;
            isDone = true;
        }
    }

    void FranklinCapacityCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardEffectTargetPrompt = false;
        isWaitingForTarget = false;
    }

    void GeorgesCapacityCommand::execute() {
        if (isWaitingForTarget) {
            engine.isWaitingForCardEffectTargetPrompt = true;
            engine.waitingCommand = this;
            return;
        }
        else {
            if (target != nullptr) {
                int dmg = engine.board->rollDice(state::ONLYD4);
                target->receiveDamage(dmg);
                user->capacityUsed = true;
            }
            engine.isWaitingForCardEffectTargetPrompt = false;
            isDone = true;
        }
    }

    void GeorgesCapacityCommand::receivePromptAnswer(void* answer) {
        int targetID = *static_cast<int*>(answer);
        target = engine.board->playerList[targetID].get();
        engine.waitingCommand = nullptr;
        engine.isWaitingForCardEffectTargetPrompt = false;
        isWaitingForTarget = false;
    }
}