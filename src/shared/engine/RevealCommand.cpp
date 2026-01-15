#include "RevealCommand.h"
#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include "state/CellClass.h"
#include "state/RollRule.h"

namespace engine {

    RevealCommand::RevealCommand(Engine& engine,int playerId)
        : engine(engine), playerId(playerId)
    {}

    void RevealCommand::execute()
    {
        engine.board->playerList[playerId]->revealed=true;
        isDone = true;
    }

    void RevealCommand::executeAI()
    {
        engine.board->playerList[playerId]->revealed=true;
        isDone = true;
    }

}
