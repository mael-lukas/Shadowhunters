#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include <random>
#include <algorithm>
#include "GateCommand.h"
#include <iostream>
#include "DrawCardCommand.h"
#include "WoodsCommand.h"
#include "StealEquipCommand.h"
#include "DarkSpiderCommand.h"
#include "DarkDollCommand.h"
#include "DarkBatCommand.h"
#include "WhiteFlareCommand.h"
#include "WhiteWaterCommand.h"
#include "WhiteAidCommand.h"

namespace engine
{
    Engine::Engine(state::Board *board): board(board), isBusy(false), currentPlayerIndex(0) {
        currentTurnPhase = MOVE_PHASE;
        cellEffectsFactory[state::GRAVEYARD] = [](Engine& engine) { return new DrawCardCommand(engine, state::DARK); };
        cellEffectsFactory[state::CHURCH] = [](Engine& engine) { return new DrawCardCommand(engine, state::WHITE); };
        cellEffectsFactory[state::HERMITZONE] = [](Engine& engine) { return new DrawCardCommand(engine, state::HERMIT); };
        cellEffectsFactory[state::ALTAR] = [](Engine& engine) { return new StealEquipCommand(engine, &engine.getCurrentPlayer()); };
        cellEffectsFactory[state::GATE] = [](Engine& engine) { return new GateCommand(engine); };
        cellEffectsFactory[state::WOODS] = [](Engine& engine) { return new WoodsCommand(engine); };

        cardEffectsFactory[state::SPIDER1] = [](Engine& engine) { return new DarkSpiderCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::SPIDER2] = [](Engine& engine) { return new DarkSpiderCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::DOLL1] = [](Engine& engine) { return new DarkDollCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::DOLL2] = [](Engine& engine) { return new DarkDollCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::BAT1] = [](Engine& engine) { return new DarkBatCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::BAT2] = [](Engine& engine) { return new DarkBatCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::BAT3] = [](Engine& engine) { return new DarkBatCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::FLARE1] = [](Engine& engine) { return new WhiteFlareCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::FLARE2] = [](Engine& engine) { return new WhiteFlareCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::WATER1] = [](Engine& engine) { return new WhiteWaterCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::WATER2] = [](Engine& engine) { return new WhiteWaterCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::WATER3] = [](Engine& engine) { return new WhiteWaterCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::WATER4] = [](Engine& engine) { return new WhiteWaterCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::AID1] = [](Engine& engine) { return new WhiteAidCommand(engine, &engine.getCurrentPlayer()); };
        cardEffectsFactory[state::AID2] = [](Engine& engine) { return new WhiteAidCommand(engine, &engine.getCurrentPlayer()); };

        board->playerList[0]->isTurnPlayer = true;
    }

    state::Player& Engine::getCurrentPlayer() {
        return *board->playerList[currentPlayerIndex];
    }

    void Engine::goToNextPlayer() {
        if (!board->playerList.empty()) {
            state::Player& currentPlayer = getCurrentPlayer();
            currentPlayer.isTurnPlayer = false;
            currentPlayerIndex = (currentPlayerIndex + 1) % board->playerList.size();
            while (!getCurrentPlayer().isAlive) {
                currentPlayerIndex = (currentPlayerIndex + 1) % board->playerList.size();
            }
        }
        state::Player& currentPlayer = getCurrentPlayer();
        currentPlayer.isTurnPlayer = true;
        auto& equipCards = currentPlayer.equipCards;
        for (auto it = equipCards.begin(); it != equipCards.end(); ) {
            if ((*it)->effectTimer == state::ONELOOP) {
                board->discardCard(*it);
                it = equipCards.erase(it);
            } else {
                ++it;
            }
        }
    }

    void Engine::processOneCommand() {
        //std::cout << "[ENGINE] Processing one command. isBusy = " << (bool)isBusy << std::endl;
        if (commands.empty()) {
            return;
        }
        isBusy = true;
        state::Player& currentPlayer = getCurrentPlayer();
        Command* cmd = commands.front();
        if( currentPlayer.type != state::HUMAN) {
            cmd->executeAI();
        }
        else{
            //std::cout << "Execute Human command" << std::endl;
            cmd->execute();
        }

        
        if (cmd->isDone) {
            commands.erase(commands.begin());
            isBusy = false;
        }
    }

    void Engine::checkForVictory() {
        bool huntersAlive = false;
        bool shadowsAlive = false;
        for (auto& player : board->playerList) {
            if (player->isAlive) {
                if (player->getRole() == state::HUNTER) {
                    huntersAlive = true;
                } else {
                    shadowsAlive = true;
                }
            }
        }
        if (!shadowsAlive && huntersAlive) {
            currentGameState = HUNTERS_WIN;
        }
        else if (!huntersAlive && shadowsAlive) {
            currentGameState = SHADOWS_WIN;
        }
        else if (!huntersAlive && !shadowsAlive) {
            currentGameState = DRAW;
        }
        else {
            currentGameState = ONGOING;
        }
    }

    // void Engine::startTurn() {
    //     commands.clear();
    // }


    // void Engine::endTurn()
    // {
    //     if (!board->playerList.empty())
    //     {
    //         currentPlayerIndex = (currentPlayerIndex + 1) % board->playerList.size();
    //     }
    // }

}
