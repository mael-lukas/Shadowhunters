#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include <random>
#include <algorithm>
#include <iostream>

namespace engine
{
    Engine::Engine(state::Board *board): board(board), isBusy(false), currentPlayerIndex(0) {
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
        std::cout << "[ENGINE] Processing command...\n";
        cmd->execute(*this);
        if (cmd->isDone) {
            delete cmd;
            commands.erase(commands.begin());
            isBusy = false;
        }

    }


    void Engine::startTurn() {
        commands.clear();
        // Tu peux aussi notifier le client que c’est le tour de getCurrentPlayer()
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

        // Fin si un des deux camps n’a plus de survivants
        return (!shadowAlive || !hunterAlive);
    }

    void Engine::checkVictory()
    {
        bool shadowAlive = false;
        bool hunterAlive = false;

        // On prépare aussi des listes pour afficher qui a gagné
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

        // Détermination du camp gagnant
        if (!shadowAlive && hunterAlive)
        {
            std::cout << "[VICTOIRE] Les Hunters gagnent la partie.\n";
            // Ici tu peux plus tard notifier le client / UI
        }
        else if (!hunterAlive && shadowAlive)
        {
            std::cout << "[VICTOIRE] Les Shadows gagnent la partie.\n";
        }
        else if (!hunterAlive && !shadowAlive)
        {
            // Cas bizarre : plus de Hunters ni de Shadows vivants
            std::cout << "[VICTOIRE] Aucun Hunter ni Shadow vivant. "
                         "Les Neutres gagnent (si tu veux cette règle).\n";
        }
        else
        {
            // Normalement, checkVictory ne devrait être appelé
            // que quand isGameOver() a renvoyé true, donc ce cas
            // ne devrait pas arriver, mais on le garde par sécurité.
            std::cout << "[INFO] checkVictory() appelé sans vainqueur clair.\n";
        }
    }


    state::Board &Engine::getBoard()
    {
        // Précondition : board != nullptr
        return *board;
    }

}
