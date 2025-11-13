#include "Client.h"
#include "../../shared/engine/DrawCardCommand.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace client {
    Client::Client(state::Board* board, render::RenderManager* renderMan, engine::Engine* engineGame) : 
    board(board), 
    renderMan(renderMan), engineGame(engineGame) {}

    void Client::run() {
        renderMan->init();
        while (renderMan->window.isOpen()) {
            sf::Event event;
            while (renderMan->window.pollEvent(event)) {
                renderMan->handleEvent(event, this);
            }

            renderMan->draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    void Client::moveTestClicked(std::string newLocation) {
        // Test for state update : player moved
        std::cout << "Client received move test click callback." << std::endl;
        board->movePlayerTo(*(board->playerPos[state::OUTSIDE][0]), state::GRAVEYARD);
    }

    void Client::damageTestClicked() {
        // Test for state update : first player in GRAVEYARD takes 3 damage
        std::cout << "Client received damage test click callback." << std::endl;


        if (board->playerPos[state::GRAVEYARD].size() > 0) {
            board->playerPos[state::GRAVEYARD][0]->receiveDamage(3);
        }
    }

    void Client::drawTestClicked(std::string cardDraw) {
        // Test for state update : draw card & equip it to first player in GRAVEYARD if any
        std::cout << "Client received draw test click callback." << std::endl;
        engine::Command* cmd ;

        if(cardDraw == "darkCard")
        {
            cmd =  new engine::DrawCardCommand(state::CardType::DARK);
        }
        else if(cardDraw == "whiteCard")
        {
            cmd =  new engine::DrawCardCommand(state::CardType::WHITE);
        }
        else if(cardDraw == "hermitCard")
        {
            cmd =  new engine::DrawCardCommand(state::CardType::HERMIT);
        }
        
        engineGame -> commands.push_back(cmd);
    }

    void Client::revealedClicked(){

    }
}