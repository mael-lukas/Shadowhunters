#include "RandomAI.h"
#include "../../shared/engine/DrawCardCommand.h"
#include "../../shared/engine/MoveCommand.h"
#include "../../shared/engine/AttackCommand.h"
#include "../../shared/engine/GateCommand.h"
#include "../../shared/engine/WoodsCommand.h"
#include "../../shared/engine/StealEquipCommand.h"

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
            cmd -> draw = randomChoice(2);
            engine -> commands.push_back(cmd);
        }
    }

    void RandomAI::move() {
        if (!engine->isBusy)
        {
            std::cout << "Move Function Called from the RandomAI" << std::endl;
            auto* cmd = new engine::MoveCommand(*engine);
            int die;
            //Modifier move pour choisir une position aléatoire
            //Je dois modifier les variables des autres classes en fonction de ma position choisie
            
            currentPos = engine->getCurrentPlayer().position;
            
            do {
                
                do {
                    die = engine->board->rollDice(state::RollRule::SUM);
                    if(die == 7) {
                        std::cout << "Die rolled 7: re-rolling" << std::endl;
                    }
                } while (die == 7);
                newPos = engine->board->dieToCell(die);
                /*std::cout << "Die rolled 1: " << die << " => Cell: " << newPos->cell << std::endl;
                newPos = engine->board->dieToCell(die);
                std::cout << "Die rolled 2: " << die << " => Cell: " << newPos->cell << std::endl;*/
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
                cmd -> attacked = nullptr; // choose no target
                engine -> commands.push_back(cmd);
                return;
            }
            targetID = randomChoice((neighboursPlayers.size()+1));
            state::Player* attacked = neighboursPlayers[targetID];
            if(targetID == neighboursPlayers.size()) {
                cmd -> attacked = nullptr; // choose no target
                return;
            }
            cmd = new engine::AttackCommand(*engine, attacker);
            cmd -> attacked = attacked;
            engine -> commands.push_back(cmd);
            neighboursPlayers.clear();
        }
    }

    void RandomAI::cellEffect() {
        switch(newPos->cell) {
                case state::GATE:{
                    auto* gate = new engine::GateCommand(*engine);
                    gate->gate_Choice = (engine::CardChoice)randomChoice(4); 
                    engine->commands.push_back(gate);
                    break;
                }
                case state::WOODS:{
                    auto* woods = new engine::WoodsCommand(*engine);
                    woods->woods_Choice = (engine::Choice)randomChoice(3); 
                    woods->target = engine->board->playerList[randomChoice(engine->board->playerList.size())].get();
                    engine->commands.push_back(woods);
                    break;
                }
                case state::ALTAR:{
                    auto* steal = new engine::StealEquipCommand(*engine, &engine->getCurrentPlayer());
                    steal->target = engine->board->playerList[randomChoice(engine->board->playerList.size())].get();
                    engine->commands.push_back(steal);
                    break;
                }
                case state::HERMITZONE:{
                    auto* hermit = new engine::DrawCardCommand(*engine, state::HERMIT);
                    engine->commands.push_back(hermit);
                    break;
                }
                case state::CHURCH:{
                    auto* church = new engine::DrawCardCommand(*engine, state::WHITE);
                    engine->commands.push_back(church);
                    break;

                }
                case state::GRAVEYARD:{
                    auto* grave = new engine::DrawCardCommand(*engine, state::DARK);
                    engine->commands.push_back(grave);
                    break;
                }
                default:
                    std::cerr << "Unknown cell effect!" << std::endl;
                    break;
            }
    }

    int RandomAI::randomChoice (int numberOfChoices) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> dist(0, numberOfChoices-1);
        return dist(g);
    }

};
