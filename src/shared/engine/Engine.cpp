#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include <random>
#include <algorithm>
#include <iostream>

namespace engine
{

    // ==========================
    //  Constructeur
    // ==========================
    Engine::Engine(state::Board *board)
        : board(board), commands(), cardEffects(), currentPlayerIndex(0)
    {
        // Tu pourras appeler initGame(numPlayers) ailleurs si besoin
        int numPlayers = 4; // Exemple : initialisation avec 4 joueurs
        initGame(numPlayers);
    }

    // ==========================
    //  Initialisation de partie
    // ==========================
    void Engine::initGame(int numPlayers)
    {
        currentPlayerIndex = 0;
        commands.clear();
        runGameLoop();
    }

    // ==========================
    //  Boucle de jeu (squelette)
    // ==========================
    void Engine::runGameLoop()
    {
        while (!isGameOver())
        {
            startTurn();

            // Ici, en pratique, le client créera des Command
            // (MoveCommand, AttackCommand, UsecardCommand, etc.)
            // et les passera à processPlayerAction(cmd).

            endTurn();
        }

        checkVictory();
    }

    void Engine::startTurn()
    {
        commands.clear();
        // Tu peux aussi notifier le client que c’est le tour de getCurrentPlayer()
    }

    void Engine::processPlayerAction(Command &cmd)
    {
        // Exécute la commande sur le moteur
        cmd.execute(*this);

        // Si tu veux garder un historique :
        // commands.push_back(&cmd); // attention à la durée de vie
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
            state::Player *p = p_uptr.get(); // 🔥 conversion unique_ptr → Player*
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

    // ==========================
    //  Getters
    // ==========================
    state::Player &Engine::getCurrentPlayer()
    {
        // Précondition : players ne doit pas être vide
        return *board->playerList[currentPlayerIndex].get();
    }

    state::Board &Engine::getBoard()
    {
        // Précondition : board != nullptr
        return *board;
    }

}
