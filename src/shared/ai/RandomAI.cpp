#include "RandomAI.h"
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
    RandomAI::RandomAI(state::Board* board, engine::Engine* engine)
    : AI::AI(board,engine) {}

    RandomAI::~RandomAI() {}

    void RandomAI::drawCard() {
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

    void RandomAI::move() {
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
        else {
            std::cout << "Engine is busy the move is not run" << std::endl;
        }
    }

    void RandomAI::attackTarget() {
        if (engine->isBusy == false) {
            state::Player* attacker = &engine->getCurrentPlayer();
            auto* cmd = new engine::AttackCommand(*engine, attacker); 
            neighboursPlayers=engine->board->getNeighbours(&engine->getCurrentPlayer());
            
            if(neighboursPlayers.size() == 0) {
                std::cout << "[AI] No neighbours to attack. Choosing no target." << std::endl;
                cmd -> receiveAnswer(nullptr); // choose no target
                engine -> commands.push_back(cmd);
                return;
            }

            //std::cout << "Si cela va là alors qu'il n'y a pas de voisin c'est que tu as merdé" << std::endl;
            targetID = randomChoice((neighboursPlayers.size()));
            state::Player* attacked = neighboursPlayers[targetID];
            std::cout << "[AI] Attacker ID: " << attacker->id << " chooses to attack Target ID: " << attacked->id << std::endl;
            //cmd = new engine::AttackCommand(*engine, attacker);
            cmd -> receiveAnswer(attacked);
            engine -> commands.push_back(cmd);
            neighboursPlayers.clear();
        }
    }

    void RandomAI::cellEffect() {
        std::cout << "[AI] Executing cell effect for cell: " << std::endl;
        engine->isBusy = true; // empêche l’IA de faire autre chose pendant l'effet
        state::Cell* currentCell = &engine->getCurrentPlayer().position->cell;

        // Liste pour stocker toutes les commandes créées
        std::vector<engine::Command*> createdCommands;
        switch(newPos->cell) {
            case state::GATE:{
                auto* cmd = new engine::GateCommand(*engine);
                cmd->gate_Choice = (engine::CardChoice)randomChoice(4); 
                //cardTarget(state::CardType(gate->gate_Choice));
                engine->commands.push_back(cmd);
                break;
            }
            case state::WOODS:{
                auto* cmd = new engine::WoodsCommand(*engine);
                cmd->woods_Choice = (engine::Choice)randomChoice(3); 
                cmd->target = engine->board->playerList[randomChoice(engine->board->playerList.size())].get();
                engine->commands.push_back(cmd);
                break;
            }
            case state::ALTAR:{
                auto* cmd = new engine::StealEquipCommand(*engine, &engine->getCurrentPlayer());
                cmd->target = engine->board->playerList[randomChoice(engine->board->playerList.size())].get();
                engine->commands.push_back(cmd);
                break;
            }
            case state::HERMITZONE:{
                auto* cmd = new engine::DrawCardCommand(*engine, state::HERMIT);
                cmd->draw = true;
                engine->commands.push_back(cmd);
                break;
            }
            case state::CHURCH:{
                auto* cmd = new engine::DrawCardCommand(*engine, state::WHITE);
                cmd->draw = true;
                engine->commands.push_back(cmd);
                break;

            }
            case state::GRAVEYARD:{
                auto* cmd = new engine::DrawCardCommand(*engine, state::DARK);
                cmd->draw = true;
                engine->commands.push_back(cmd);
                break;
            }
            default:
                std::cerr << "Unknown cell effect!" << std::endl;
                break;
        }

    }

    void RandomAI::cardTarget(engine::Command* cmd) {
        if (!cmd) return;

        auto& players = engine->board->playerList;

        auto randomPlayer = [&]() -> state::Player* {
            return players[randomChoice(players.size())].get();
        };

        if (cmd->needTarget()) {
            state::Player* target = randomPlayer();
            cmd->receiveAnswer(target);
            std::cout << "[AI] Card target chosen: Player ID " << target->id << std::endl;
            return;
        }

    }

    void RandomAI::revealed() {
        if (engine->isBusy == false) {
            if(randomChoice(100) < 5){
                auto* cmd = new engine::RevealCommand(*engine, engine->getCurrentPlayer().id);
                std::cout << "[AI] Revealing self: Player ID " << engine->getCurrentPlayer().id << std::endl;
                engine -> commands.push_back(cmd);
            }
            
        }
    }

    int RandomAI::randomChoice (int numberOfChoices) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> dist(0, numberOfChoices-1);
        return dist(g);
    }

};
