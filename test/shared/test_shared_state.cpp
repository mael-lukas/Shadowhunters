#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"
#include <iostream>

using namespace state;

BOOST_AUTO_TEST_CASE(TestState) {
    PackOfCards whitePack(WHITECOUNT, WHITE);
    PackOfCards darkPack(DARKCOUNT-WHITECOUNT-1,DARK);
    PackOfCards hermitPack(HERMITCOUNT-DARKCOUNT-1,HERMIT);
    BOOST_CHECK_EQUAL(darkPack.listOfCards[0]->name,SPIDER1);
    hermitPack.shuffle();
    CardClass* drawnHermit = hermitPack.draw();
    std::cout << std::to_string(static_cast<Card>(drawnHermit->name)) << std::endl;
    BOOST_CHECK_EQUAL(whitePack.listOfCards.size(), 2);
    BOOST_CHECK_EQUAL(whitePack.discardPile.size(), 0);
    BOOST_CHECK_EQUAL(whitePack.type, WHITE);
    std::cout << "White draw size: " << whitePack.listOfCards.size() << std::endl;
    std::cout << "White discard size: " << whitePack.discardPile.size() << std::endl;
    CardClass* drawnWhite = whitePack.draw();
    BOOST_CHECK_EQUAL(drawnWhite->name, WHITE1);
    BOOST_CHECK_EQUAL(whitePack.listOfCards.size(), 1);
    whitePack.discard(drawnWhite);
    BOOST_CHECK_EQUAL(whitePack.discardPile.size(), 1);
    std::cout << "White draw size: " << whitePack.listOfCards.size() << std::endl;
    std::cout << "White discard size: " << whitePack.discardPile.size() << std::endl;
    CardClass* drawnWhite2 = whitePack.draw();
    BOOST_CHECK_EQUAL(drawnWhite->name, WHITE1);
    BOOST_CHECK_EQUAL(whitePack.listOfCards.size(), 0);
    whitePack.discard(drawnWhite2);
    BOOST_CHECK_EQUAL(whitePack.discardPile.size(), 2);
    std::cout << "White draw size: " << whitePack.listOfCards.size() << std::endl;
    std::cout << "White discard size: " << whitePack.discardPile.size() << std::endl;
    CardClass* drawnWhite3 = whitePack.draw();
    BOOST_CHECK_EQUAL(whitePack.discardPile.size(), 0);
    std::cout << "White draw size: " << whitePack.listOfCards.size() << std::endl;
    std::cout << "White discard size: " << whitePack.discardPile.size() << std::endl;
}

BOOST_AUTO_TEST_CASE(TestStateBoard) {
    Board bd;
    BOOST_CHECK((2 <= bd.rollDice(SUM) && bd.rollDice(SUM) <= 10));
    BOOST_CHECK_EQUAL(bd.drawDark()->name,SPIDER1);
    BOOST_CHECK_EQUAL(bd.drawWhite()->name,WHITE1);
    BOOST_CHECK_EQUAL(bd.drawHermit()->name,HERMIT1);

    std::vector<Player*> supposed_neighbours = bd.cellList[OUTSIDE]->playersInCell;
    supposed_neighbours.erase(supposed_neighbours.begin());
    std::cout << "There are " << supposed_neighbours.size() << " neighbours in OUTSIDE cell" << std::endl;
    std::cout << "There are " << bd.getNeighbours((bd.cellList[OUTSIDE]->playersInCell[0])).size() << " neighbours found by getNeighbours()" << std::endl;
    BOOST_CHECK_EQUAL(bd.getNeighbours((bd.cellList[OUTSIDE]->playersInCell[0])).size(), supposed_neighbours.size());
    bd.cellList[OUTSIDE]->playersInCell[0]->receiveDamage(2);
    std::cout << "Dealt " << bd.cellList[OUTSIDE]->playersInCell[0]->wounds << " damage to Werewolf in OUTSIDE" << std::endl;
    bd.movePlayerTo((bd.cellList[OUTSIDE]->playersInCell[0]),bd.cellList[0]);
    BOOST_CHECK_EQUAL(bd.getNeighbours((bd.cellList[0]->playersInCell[0])).size(), 0);
    BOOST_CHECK_EQUAL(bd.getNeighbours((bd.cellList[OUTSIDE]->playersInCell[0])).size(), 0);
    BOOST_CHECK_EQUAL(bd.cellList[OUTSIDE]->playersInCell.size(),1);
    BOOST_CHECK_EQUAL(bd.cellList[0]->playersInCell.size(),1);
    BOOST_CHECK_EQUAL(bd.cellList[0]->playersInCell[0]->position->cell,bd.cellList[0]->cell);

    std::cout << static_cast<Cell>(bd.getOtherCellInSameZone(bd.cellList[0])->cell) << std::endl;
    std::cout << bd.cellList[0]->playersInCell[0]->wounds << std::endl;
    std::cout << bd.cellList[OUTSIDE]->playersInCell[0]->wounds << std::endl;
    std::cout << "Werewolf in t'inquiète HAS " << bd.cellList[0]->playersInCell[0]->wounds << " damages" << std::endl;
     
    
    //===================================================
    //Test of defineGameOrder
    std::cout << "Player order after defineGameOrder: " << std::endl;
    for (const auto& player : bd.playerList) {
        std::cout << "Player Name: " << player->getRole() << ", ";
        std::cout << "Player ID: " << player->id << std::endl;  
    }

    bd.defineGameOrder(bd.playerList);

    std::cout << "Player order after defineGameOrder: " << std::endl;
    for (const auto& player : bd.playerList) {
        std::cout << "Player Name: " << player->getRole() << ", ";
        std::cout << "Player ID: " << player->id << std::endl;  
    }
    //===================================================

}

BOOST_AUTO_TEST_CASE(TestStatePlayer) {
    //Initialize variable
    Board bd;
    Werewolf wf(&bd);
    Emi emi(&bd);


    //Test Attributes
    BOOST_CHECK_EQUAL(wf.wounds,0);
    BOOST_CHECK_EQUAL(wf.isAlive,true);
    BOOST_CHECK_EQUAL(wf.revealed,false);
    BOOST_CHECK_EQUAL(wf.capacityUsed,false);
    BOOST_CHECK_EQUAL(wf.position->cell,bd.cellList[OUTSIDE]->cell);


    //Tests Functions
    BOOST_CHECK_EQUAL(wf.receiveDamage(10),true);
    BOOST_CHECK_EQUAL(wf.dealDamage(9,emi),true);
    //Wait for getAttacked to be implemented
    //BOOST_CHECK_EQUAL(wf.attackOther(emi),false);
}

BOOST_AUTO_TEST_CASE(TestStateCharacters) {
    Board bd;
    Werewolf wf(&bd);
    Emi emi(&bd);
    Franklin franklin(&bd);
    Georges georges(&bd);
    Unknown unknown(&bd);
    Vampire vampire(&bd);

    BOOST_CHECK_EQUAL(wf.wounds,0);
    BOOST_CHECK_EQUAL(wf.isAlive,true);
    BOOST_CHECK_EQUAL(wf.revealed,false);
    BOOST_CHECK_EQUAL(wf.capacityUsed,false);
    BOOST_CHECK_EQUAL(wf.position->cell,bd.cellList[OUTSIDE]->cell);
    BOOST_CHECK_EQUAL(wf.equipCards.size(),0);

    BOOST_CHECK_EQUAL(emi.wounds,0);
    BOOST_CHECK_EQUAL(emi.isAlive,true);
    BOOST_CHECK_EQUAL(emi.revealed,false);
    BOOST_CHECK_EQUAL(emi.capacityUsed,false);
    BOOST_CHECK_EQUAL(emi.position->cell,bd.cellList[OUTSIDE]->cell);
    BOOST_CHECK_EQUAL(emi.equipCards.size(),0);

    BOOST_CHECK_EQUAL(franklin.wounds,0);
    BOOST_CHECK_EQUAL(franklin.isAlive,true);
    BOOST_CHECK_EQUAL(franklin.revealed,false);
    BOOST_CHECK_EQUAL(franklin.capacityUsed,false);
    BOOST_CHECK_EQUAL(franklin.position->cell,bd.cellList[OUTSIDE]->cell);
    BOOST_CHECK_EQUAL(franklin.equipCards.size(),0);

    BOOST_CHECK_EQUAL(georges.wounds,0);
    BOOST_CHECK_EQUAL(georges.isAlive,true);
    BOOST_CHECK_EQUAL(georges.revealed,false);
    BOOST_CHECK_EQUAL(georges.capacityUsed,false);
    BOOST_CHECK_EQUAL(georges.position->cell,bd.cellList[OUTSIDE]->cell);
    BOOST_CHECK_EQUAL(georges.equipCards.size(),0);

    BOOST_CHECK_EQUAL(unknown.wounds,0);
    BOOST_CHECK_EQUAL(unknown.isAlive,true);
    BOOST_CHECK_EQUAL(unknown.revealed,false);
    BOOST_CHECK_EQUAL(unknown.capacityUsed,false);
    BOOST_CHECK_EQUAL(unknown.position->cell,bd.cellList[OUTSIDE]->cell);
    BOOST_CHECK_EQUAL(unknown.equipCards.size(),0);

    BOOST_CHECK_EQUAL(vampire.wounds,0);
    BOOST_CHECK_EQUAL(vampire.isAlive,true);
    BOOST_CHECK_EQUAL(vampire.revealed,false);
    BOOST_CHECK_EQUAL(vampire.capacityUsed,false);
    BOOST_CHECK_EQUAL(vampire.position->cell,bd.cellList[OUTSIDE]->cell);
    BOOST_CHECK_EQUAL(vampire.equipCards.size(),0);

    vampire.receiveDamage(3);
    BOOST_CHECK_EQUAL(vampire.wounds,3);
    std::cout << "Vampire has " << vampire.wounds << " damage" << std::endl;
    vampire.attackOther(emi);
    BOOST_CHECK_EQUAL(vampire.wounds,3);
    std::cout << "Vampire has " << vampire.wounds << " damage" << std::endl;
    vampire.revealed = true;
    vampire.attackOther(emi);
    BOOST_CHECK_EQUAL(vampire.wounds, 1);
    std::cout << "Vampire has " << vampire.wounds << " damage" << std::endl;
    vampire.receiveDamage(15);
    BOOST_CHECK_EQUAL(vampire.isAlive,false);

    bd.movePlayerTo(&emi,bd.cellList[1]);
    std::cout << "Emi is in Cell " << static_cast<Cell>(emi.position->cell) << std::endl;
    std::cout << "Next to Cell " << static_cast<Cell>(emi.position->cell) << " is the Cell " << bd.getOtherCellInSameZone(emi.position)->cell << std::endl;
    emi.revealed = true;
    emi.useCapacity();
    std::cout << "Emi is in Cell " << static_cast<Cell>(emi.position->cell) << std::endl;
    franklin.useCapacity();
    BOOST_CHECK_EQUAL(franklin.capacityUsed,false);
    franklin.revealed = true;
    franklin.useCapacity();
    BOOST_CHECK_EQUAL(franklin.capacityUsed,true);

    georges.useCapacity();
    BOOST_CHECK_EQUAL(georges.capacityUsed,false);
    georges.revealed = true;
    georges.useCapacity();
    BOOST_CHECK_EQUAL(georges.capacityUsed,true);

    vampire.wounds = 10;
    std::cout << "Werewolf has " << wf.wounds << " damage" << std::endl;
    std::cout << "Vampire has " << vampire.wounds << " damage" << std::endl;
    vampire.attackOther(wf);
    std::cout << "Werewolf has " << wf.wounds << " damage" << std::endl;
    std::cout << "Vampire has " << vampire.wounds << " damage" << std::endl;

    wf.revealed = true;
    std::cout << "Werewolf has " << wf.wounds << " damage" << std::endl;
    std::cout << "Vampire has " << vampire.wounds << " damage" << std::endl;
    vampire.attackOther(wf);
    std::cout << "Werewolf has " << wf.wounds << " damage" << std::endl;
    std::cout << "Vampire has " << vampire.wounds << " damage" << std::endl;
}
