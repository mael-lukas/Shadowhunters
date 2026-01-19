#include "MoveCommand.h"
#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include "state/CellClass.h"
#include "state/RollRule.h"

namespace engine {

    MoveCommand::MoveCommand(Engine& engine)
        : engine(engine)
    {}

    void MoveCommand::execute()
    {
        if (isWaitingForCell) {
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCellPrompt = true;
                engine.waitingCommand = this;
            }
            return;
        }

        if (cellAnswerReceived) {
            engine.currentTurnPhase = CELL_EFFECT_PHASE;
            engine.board->movePlayerTo(&engine.getCurrentPlayer(), promptCell);
            {   std::lock_guard<std::mutex> lock(engine.promptMutex);
                engine.isWaitingForCellPrompt = false;
            }
            isDone = true;
            return;
        }

        state::Player& player = engine.getCurrentPlayer();
        state::CellClass *oldPos = player.position;
        state::CellClass *newPos = oldPos;

        while (newPos == oldPos) {
            int die = engine.board->rollDice(state::RollRule::SUM);
            if (die == 7) {
                isWaitingForCell = true;
                return;
            }
            newPos = engine.board->dieToCell(die);
        }
        engine.currentTurnPhase = CELL_EFFECT_PHASE;
        engine.board->movePlayerTo(&player, newPos);
        isDone = true;
    }

    void MoveCommand::receivePromptAnswer(void* answer)
    {
        promptCell = static_cast<state::CellClass*>(answer);
        {   std::lock_guard<std::mutex> lock(engine.promptMutex);
            engine.waitingCommand = nullptr;
            engine.isWaitingForCellPrompt = false;
        }
        isWaitingForCell = false;
        cellAnswerReceived = true;
    }
}
