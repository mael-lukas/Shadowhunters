#include "MoveCommand.h"
#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include "state/CellClass.h"
#include "state/RollRule.h"
#include <iostream>

namespace engine
{

    MoveCommand::MoveCommand(Engine &engine)
        : engine(engine)
    {
    }

    void MoveCommand::execute()
    {
        state::Player &player = engine.getCurrentPlayer();
        state::Board &board = engine.getBoard();

        if (die == -1)
        {
            die = board.rollDice(state::RollRule::SUM);
            std::cout << "[Dice] Résultat du dé : " << die << std::endl;
        }

        if (die != 7)
        {
            state::CellClass *newPos = board.dieToCell(die);

            if (newPos == player.position)
            {
                die = -1;
                return;
            }

            board.movePlayerTo(&player, newPos);
            engine.currentTurnPhase = BATTLE_PHASE;
            isDone = true;

            die = -1;
            isWaitingForTarget = true;
            return;
        }

        if (isWaitingForTarget)
        {
            engine.isWaitingForTargetPrompt = true;
            engine.waitingCommand = this;
            return;

            board.movePlayerTo(&player, ChosenCell);
            engine.currentTurnPhase = BATTLE_PHASE;
            isDone = true;

            die = -1;
            isWaitingForTarget = true;
        }
    }

    void MoveCommand::receivePromptAnswer(void *answer)
    {
        int cellID = *static_cast<int *>(answer);

        if (cellID == -1)
        {
            engine.waitingCommand = nullptr;
            engine.isWaitingForTargetPrompt = false;
            isDone = true;

            die = -1;
            isWaitingForTarget = true;
            return;
        }

        state::Cell target = static_cast<state::Cell>(cellID);
        state::Player &player = engine.getCurrentPlayer();

        for (auto &cellPtr : engine.getBoard().cellList)
        {
            if (cellPtr->cell == target)
            {
                if (cellPtr == player.position)
                {
                    return;
                }
                ChosenCell = cellPtr;
                break;
            }
        }

        isWaitingForTarget = false;
        engine.waitingCommand = nullptr;
        engine.isWaitingForTargetPrompt = false;
    }
}