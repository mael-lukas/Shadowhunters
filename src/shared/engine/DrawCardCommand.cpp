#include "DrawCardCommand.h"
#include "state/CardClass.h"
#include <iostream>

namespace engine
{

    DrawCardCommand::DrawCardCommand(state::CardType type)
    {
        cardType = type;
    }
    void DrawCardCommand::execute(Engine &engine)
    {
        if (cardType == state::WHITE)
            engine.getCurrentPlayer().equipCards.push_back(engine.board->drawDark());
        if (cardType == state::DARK)
            engine.getCurrentPlayer().equipCards.push_back(engine.board->drawDark());
        if (cardType == state::HERMIT){
            std::cout << "HERMIT card draw" << std::endl;
            engine.board->hermitPack.discard(engine.board->drawHermit());

            //Test vector discard hermit
            std::cout << "HERMIT card discard" << std::endl;
            for(auto& card : engine.board->hermitPack.discardPile){
                std::cout << card->name << " ";
            }
        }
            
        
        //Vérifie les carte équipé
        std::cout << "DrawCardCommand" << std::endl;
        std::cout << "Card of the player: ";
        for (auto& card : engine.getCurrentPlayer().equipCards) {
            std::cout << card->name << " ";
            std::cout << card->type<< " ";
        }
        std::cout << std::endl;


        isDone = true;
    }
        
}