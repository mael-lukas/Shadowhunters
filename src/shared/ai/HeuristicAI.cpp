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
            state::CardType cardType = state::CardType(randomChoice(3));
            auto* cmd = new engine::DrawCardCommand(*engine,cardType);
            targetID = randomChoice((engine->board->playerList.size()+1));
            state::Player* attacked = engine->board->playerList[targetID].get();
            std::cout << "[AI] Drawing card of type: " << static_cast<int>(cardType) << " Target Player ID: " << attacked->id << std::endl;
            cmd -> receiveAnswer(attacked);
            cmd -> draw = randomChoice(2);
            engine -> commands.push_back(cmd);
        }
    }

    void HeuristicAI::move() {
        
    }

    void HeuristicAI::attackTarget() {
        
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
