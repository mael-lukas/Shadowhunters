#include "AI.h"
#include <iostream>

namespace ai
{
    AI::AI(state::Board* board, engine::Engine* engine)
    : board(board), engine(engine) {}

    AI::~AI() {}

    void AI::setTurnPhase (engine::TurnPhase newTurnPhase) {
        currentTurnPhase = newTurnPhase;
    }

    void AI::playPhase() {
        switch(currentTurnPhase) {
            case engine::MOVE_PHASE:
                std::cout << "Move Phase" << std::endl;
                move();
                break;
            case engine::BATTLE_PHASE:
                std::cout << "BATTLE_PHASE" << std::endl;
                attackTarget();
                break;
            case engine::CELL_EFFECT_PHASE:
                std::cout << "CELL_EFFECT_PHASE" << std::endl;
                drawCard();
                break;
            default:
            std::cerr << "Unknown turn phase!" << std::endl;
                break;
        }
    }

    void AI::drawCard() {
        // Implementation of AI drawing a card
    }

    void AI::move() {
        // Implementation of AI movement logic
        std::cout << "Move Function Called from the AI" << std::endl;
    }

    void AI::attackTarget() {
        // Implementation of AI attacking a target
    }

};
