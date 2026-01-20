#include "AI.h"
#include "../../shared/engine/Engine.h"
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
                move();
                std::cout << "Crash Ater Move" << std::endl;
                break;
            case engine::BATTLE_PHASE:
                std::cout << "BATTLE_PHASE" << std::endl;
                revealed();
                attackTarget();
                break;
            case engine::CELL_EFFECT_PHASE:
                std::cout << "CELL_EFFECT_PHASE" << std::endl;
                revealed();
                std::cout << "SI pas de Crash Ater Revealed message visible" << std::endl;
                cellEffect();
                std::cout << "SI pas de Crash Ater cellEffect message visible" << std::endl;
                if(!engine->commands.empty() && engine->commands.back()->needTarget()){
                    cardTarget(engine->commands.back());
                }
                break;
            case engine::CARD_EFFECT_PHASE:
                std::cout << "CARD_EFFECT_PHASE" << std::endl;
                revealed();
                drawCard();
                break;
            default:
            std::cerr << "Unknown turn phase!" << std::endl;
                break;
        }
        std::cout << "SI pas de Crash Ater switch message visible" << std::endl;
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

    void AI::cellEffect() {
        // Implementation of AI using cell effects
    }

    void AI::cardTarget(engine::Command* cmd) {
        // Implementation of AI choosing a card target
    }

    void AI::revealed() {
        // Implementation of AI revealing itself
    }
};
