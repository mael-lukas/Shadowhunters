#include "RandomAI.h"
#include "../../shared/engine/DrawCardCommandAI.h"
#include "../../shared/engine/MoveCommandAI.h"
#include "../../shared/engine/AttackCommandAI.h"

#include <random>
#include <algorithm>
#include <iostream>

namespace ai
{
    RandomAI::RandomAI(state::Board* board, engine::Engine* engine)
    : AI::AI(board,engine) {}

    RandomAI::~RandomAI() {}

    void RandomAI::drawCard() {
        state::CardType cardType = state::CardType(randomChoice(3));
        bool draw = randomChoice(2);
        if (engine->isBusy == false) {
            cmd = new engine::DrawCardCommandAI(*engine,cardType,draw);
            engine -> commands.push_back(cmd);
        }
    }

    void RandomAI::move() {
        if (!engine->isBusy)
        {
            cmd = new engine::MoveCommandAI(*engine);
            engine->commands.push_back(cmd);
        }
    }

    void RandomAI::attackTarget() {
        if (engine->isBusy == false) {
            state::Player* attacker = &engine->getCurrentPlayer();
            neighboursPlayers=engine->board->getNeighbours(&engine->getCurrentPlayer());
            if(neighboursPlayers.size() == 0) {
                cmd = new engine::AttackCommandAI(*engine, attacker, nullptr);
                engine -> commands.push_back(cmd);
                return;
            }
            targetID = randomChoice((neighboursPlayers.size()+1));
            state::Player* attacked = neighboursPlayers[targetID];
            if(targetID == neighboursPlayers.size()) {
                attacked = nullptr; // choose no target
            }
            cmd = new engine::AttackCommandAI(*engine, attacker, attacked);
            engine -> commands.push_back(cmd);
            neighboursPlayers.clear();
        }
    }

    int RandomAI::randomChoice (int numberOfChoices) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> dist(0, numberOfChoices-1);
        return dist(g);
    }

};
