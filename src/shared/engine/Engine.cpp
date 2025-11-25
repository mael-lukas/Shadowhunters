#include "Engine.h"
#include "state/Board.h"
#include "state/Player.h"
#include <random>
#include <algorithm>


namespace engine {

    // ==========================
    //  Constructeur
    // ==========================
    Engine::Engine(state::Board* board)
        : board(board)
        , commands()
        , cardEffects()
        , currentPlayerIndex(0)
    {
        // Tu pourras appeler initGame(numPlayers) ailleurs si besoin
    }

    // ==========================
    //  Initialisation de partie
    // ==========================
    void Engine::initGame(int numPlayers)
    {
        currentPlayerIndex = 0;
        commands.clear();
        (void)numPlayers; // pour l’instant, on ne s’en sert pas
    }

    // ==========================
    //  Boucle de jeu (squelette)
    // ==========================
    void Engine::runGameLoop()
    {
        while (!isGameOver()) {
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

    void Engine::processPlayerAction(Command& cmd)
    {
        // Exécute la commande sur le moteur
        cmd.execute(*this);

        // Si tu veux garder un historique :
        // commands.push_back(&cmd); // attention à la durée de vie
    }

    void Engine::endTurn()
    {
        if (!board->playerList.empty()) {
            currentPlayerIndex = (currentPlayerIndex + 1) % board->playerList.size();
        }
    }

    bool Engine::isGameOver()
    {
        // TODO : condition de fin réelle
        return false;
    }

    void Engine::checkVictory()
    {
        // TODO : calculer les vainqueurs en fonction des rôles / HP restants
        
    }

    // ==========================
    //  Accesseurs
    // ==========================
    state::Player& Engine::getCurrentPlayer()
    {
        // ⚠️ Précondition : players ne doit pas être vide
        return players[currentPlayerIndex];
    }

    state::Board& Engine::getBoard()
    {
        // ⚠️ Précondition : board != nullptr
        return *board;
    }

}
