#include "AI.h"
namespace ai
{
    AI::AI(state::Board* board, engine::Engine* engine)
    : board(board), engine(engine) {}

    AI::~AI() {}

    void AI::playTurn() {
        // Implementation of AI turn logic
    }

    void AI::drawCard() {
        // Implementation of AI drawing a card
    }

    void AI::move() {
        // Implementation of AI movement logic
    }

    void AI::attackTarget(int targetID) {
        // Implementation of AI attacking a target
    }

};
