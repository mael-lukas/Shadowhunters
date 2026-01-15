#include "MoveCommand.h"
#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include "state/CellClass.h"
#include "state/RollRule.h"
#include <iostream>

namespace engine {

    MoveCommand::MoveCommand(Engine& engine)
        : engine(engine)
    {}

    void MoveCommand::execute()
    {
        if (isWaitingForCell) {
            engine.isWaitingForCellPrompt = true;
            engine.waitingCommand = this;
            return;
        }

        if (cellAnswerReceived) {
            engine.currentTurnPhase = CELL_EFFECT_PHASE;
            engine.board->movePlayerTo(&engine.getCurrentPlayer(), promptCell);
            engine.isWaitingForCellPrompt = false;
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

    void MoveCommand::executeAI()
    {
        std::cout << "Move AI"<< std::endl;
        state::Player& player = engine.getCurrentPlayer();
        state::CellClass *oldPos = player.position;
        state::CellClass *newPos = oldPos;

        /*while (newPos == oldPos) {
            int die;
            do {
                die = engine.board->rollDice(state::RollRule::SUM);
            } while (die == 7);
            newPos = engine.board->dieToCell(die);
        }*/
        engine.currentTurnPhase = BATTLE_PHASE;
        std::cout << "Choice Position : " << choice_Position << std::endl;
        engine.board->movePlayerTo(&player, choice_Position);
        isDone = true;
        
    }

    void MoveCommand::receivePromptAnswer(void* answer)
    {
        promptCell = static_cast<state::CellClass*>(answer);
        engine.waitingCommand = nullptr;
        engine.isWaitingForCellPrompt = false;
        isWaitingForCell = false;
        cellAnswerReceived = true;
    }
}
