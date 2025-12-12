#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include <random>
#include <algorithm>
#include <iostream>

namespace engine
{
    Engine::Engine(state::Board *board): board(board), isBusy(false), currentPlayerIndex(0) {
        currentTurnPhase = MOVE_PHASE;
    }

    state::Player& Engine::getCurrentPlayer() {
        return *board->playerList[currentPlayerIndex];
    }

    void Engine::goToNextPlayer() {
        if (!board->playerList.empty()) {
            currentPlayerIndex = (currentPlayerIndex + 1) % board->playerList.size();
        }
    }

    void Engine::processOneCommand() {
        if (commands.empty()) {
            return;
        }
        isBusy = true;
        Command* cmd = commands.front();
        cmd->execute();
        if (cmd->isDone) {
            commands.erase(commands.begin());
            isBusy = false;
        }
    }

    void Engine::startTurn() {
        commands.clear();
    }


    void Engine::endTurn()
    {
        if (!board->playerList.empty())
        {
            currentPlayerIndex = (currentPlayerIndex + 1) % board->playerList.size();
        }
    }

    bool Engine::isGameOver()
    {
        bool shadowAlive = false;
        bool hunterAlive = false;

        for (auto &p_uptr : board->playerList)
        {
            state::Player *p = p_uptr.get();
            if (!p->isAlive)
                continue;

            switch (p->getRole())
            {
            case state::Role::SHADOW:
                shadowAlive = true;
                break;

            case state::Role::HUNTER:
                hunterAlive = true;
                break;

            default:
                // NEUTRAL ignoré
                break;
            }
        }

        return (!shadowAlive || !hunterAlive);
    }

    void Engine::checkVictory()
    {
        bool shadowAlive = false;
        bool hunterAlive = false;

        std::vector<state::Player *> shadows;
        std::vector<state::Player *> hunters;
        std::vector<state::Player *> neutrals;

        for (auto &p_uptr : board->playerList)
        {
            state::Player *p = p_uptr.get();
            if (!p->isAlive)
                continue;

            switch (p->getRole())
            {
            case state::Role::SHADOW:
                shadowAlive = true;
                shadows.push_back(p);
                break;

            case state::Role::HUNTER:
                hunterAlive = true;
                hunters.push_back(p);
                break;

            default:
                // Neutres ou autres rôles
                neutrals.push_back(p);
                break;
            }
        }

        if (!shadowAlive && hunterAlive)
        {
            std::cout << "[VICTOIRE] Les Hunters gagnent la partie.\n";
        }
        else if (!hunterAlive && shadowAlive)
        {
            std::cout << "[VICTOIRE] Les Shadows gagnent la partie.\n";
        }
        else if (!hunterAlive && !shadowAlive)
        {
            std::cout << "[VICTOIRE] Aucun Hunter ni Shadow vivant. "
                         "Les Neutres gagnent (si tu veux cette règle).\n";
        }
        else
        {
            // Sécurité
            std::cout << "[INFO] checkVictory() appelé sans vainqueur clair.\n";
        }
    }


    state::Board &Engine::getBoard()
    {
        return *board;
    }

}
