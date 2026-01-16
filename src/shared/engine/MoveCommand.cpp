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
        std::cout << "Move Phase Human of the id: " << engine.getCurrentPlayer().id <<" and this is a : "<< engine.getCurrentPlayer().type << std::endl;
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
        std::cout << "Move Phase AI of the id: " << engine.getCurrentPlayer().id <<" and this is a : "<< engine.getCurrentPlayer().type << std::endl;
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
        engine.currentTurnPhase = CELL_EFFECT_PHASE;
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

    bool MoveCommand::needTarget() {
        return false;
    }

    void MoveCommand::receiveAnswer(void* answer) {
    }
}
