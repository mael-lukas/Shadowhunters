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
        , players()
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

    void Engine::distributeRoles()
    {
        int n = static_cast<int>(players.size());
        if (n < 4)
        {
            // Pas assez de joueurs pour une partie standard Shadow Hunters
            return;
        }

        // 1) Calcul du nombre de Hunters / Shadows / Neutres
        int numHunters = 1;
        int numShadows = 1;
        int numNeutrals = n - 2;

        while (numNeutrals >= 2)
        {
            numNeutrals -= 2;
            numHunters += 1;
            numShadows += 1;
        }

        // 2) Préparer un tableau de rôles
        std::vector<state::Role> roles;
        roles.reserve(n);

        for (int i = 0; i < numHunters; ++i)
            roles.push_back(state::Role::HUNTER);

        for (int i = 0; i < numShadows; ++i)
            roles.push_back(state::Role::SHADOW);

        for (int i = 0; i < numNeutrals; ++i)
            roles.push_back(state::Role::NEUTRAL);

        // 3) Mélanger les rôles pour que la répartition soit aléatoire
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(roles.begin(), roles.end(), g);

        // 4) Assigner les rôles aux joueurs
        for (int i = 0; i < n; ++i)
        {
            //players[i].setRole(roles[i]);
        }
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
        if (!players.empty()) {
            currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
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

    // ==========================
    //  Mouvement (utilisé par MoveCommand)
    // ==========================
    void Engine::playerMove(state::Player& player)
    {
        if (!board) return; // sécurité basique

        // 1) Lancer les dés (somme 1d4 + 1d6)
        int roll = board->rollDice(state::SUM);

        // 2) Trouver la case associée
        state::Cell destination = board->dieToCell[roll];

        // 3) Déplacer sur le Board
        board->movePlayerToCell(player, destination);

        // 4) Mettre à jour la position du Player
        player.position = destination;

        // (optionnel) notifier ici les observateurs si tu veux
        // player.notifyObservers(state::StateEventID::PLAYER_CHANGED);
        // board->notifyObservers(state::StateEventID::BOARD_CHANGED);
    }

    // ==========================
    //  Effets de cartes (pour UsecardCommand plus tard)
    // ==========================
    void Engine::applyCardEffect(state::Card card, state::Player& target)
    {
        // Plus tard, vous pourrez utiliser cardEffects :
        //
        // auto it = cardEffects.find(card);
        // if (it != cardEffects.end() && it->second) {
        //     it->second->execute(*this);
        // }
        //
        (void)card;
        (void)target;
    }

}
