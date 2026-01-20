#include "HeuristicAI.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include "../../shared/engine/AttackCommand.h"
#include "../../shared/engine/GateCommand.h"
#include "../../shared/engine/WoodsCommand.h"
#include "../../shared/engine/StealEquipCommand.h"
#include "../../shared/engine/DarkSpiderCommand.h"
#include "../../shared/engine/DarkDollCommand.h"
#include "../../shared/engine/DarkBatCommand.h"
#include "../../shared/engine/WhiteAidCommand.h"
#include "../../shared/engine/RevealCommand.h"

#include <random>
#include <algorithm>
#include <iostream>

namespace ai
{
    HeuristicAI::HeuristicAI(state::Board* board, engine::Engine* engine)
    : AI::AI(board,engine) {}

    HeuristicAI::~HeuristicAI() {}

    void HeuristicAI::drawCard() {
        if (engine->isBusy == false) {

            /*state::CardType cardType = state::CardType(randomChoice(3));
            auto* cmd = new engine::DrawCardCommand(*engine,cardType);
            targetID = randomChoice((engine->board->playerList.size()+1));
            state::Player* attacked = engine->board->playerList[targetID].get();
            std::cout << "[AI] Drawing card of type: " << static_cast<int>(cardType) << " Target Player ID: " << attacked->id << std::endl;
            cmd -> receiveAnswer(attacked);
            cmd -> draw = randomChoice(2);
            engine -> commands.push_back(cmd);*/
        }
    }

    void HeuristicAI::move() {
        if (!engine->isBusy)
        {
            auto* cmd = new engine::MoveCommand(*engine);
            int die;
            currentPos = engine->getCurrentPlayer().position;
            
            do {
                
                do {
                    die = engine->board->rollDice(state::RollRule::SUM);
                    std::cout << "Die rolled: " << die << std::endl;
                } while (die == 7);
                newPos = engine->board->dieToCell(die);
                std::cout << "Die rolled 1: " << die << " => Cell: " << newPos->cell << std::endl;
            }while (newPos == currentPos);
            cmd -> choice_Position = newPos;
            engine->commands.push_back(cmd);
        }
    }

    int HeuristicAI::scoreMove(state::CellClass* cell) {
        switch(cell -> cell) {
            case state::CHURCH:   return 0;
            case state::HERMITZONE: return 20;
            case state::WOODS:    return 15;
            case state::GRAVEYARD:return -5;
            case state::ALTAR:    return 5;
            case state::GATE:     return 10;
            default: return 0;
        }
    }

    void HeuristicAI::attackTarget() {
        if (engine->isBusy) return;

        auto neighbours = engine->board->getNeighbours(&engine->getCurrentPlayer());
        if (neighbours.empty()) return;

        state::Player* bestTarget = nullptr;
        int bestScore = -1000;

        for (auto* p : neighbours) {
            int s = scoreAttack(p);
            if (s > bestScore) {
                bestScore = s;
                bestTarget = p;
            }
        }

        auto* cmd = new engine::AttackCommand(*engine, &engine->getCurrentPlayer());
        cmd->receiveAnswer(bestTarget);
        engine->commands.push_back(cmd);
    }

    int HeuristicAI::scoreAttack(state::Player* target) {
        int score = 0;

        if (!target->isAlive) return -1000;

        score += 10;

        score += target->wounds * 5;

        if(target->wounds <= 12) {
            score += 5;
        }
        if(target->wounds >= 12 && target->wounds < 14) {
            score += 10;
        }

        if(target->revealed){
            if (target->getRole() != engine->getCurrentPlayer().getRole()) score += 15;
            else score -= 15;
        }
        

        score += target->equipCards.size() * 3;

        return score;
    }

    void HeuristicAI::cellEffect() {
        

    }

    void HeuristicAI::cardTarget(engine::Command* cmd) {
        

    }

    void HeuristicAI::revealed() {
        if (engine->isBusy == false) {
            
            
        }
    }

};
