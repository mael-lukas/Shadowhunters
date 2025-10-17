#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"

// tester les destructeurs

using namespace state;

BOOST_AUTO_TEST_CASE(TestState) {
    PackOfCards whitePack(WHITECOUNT, WHITE);
    PackOfCards darkPack(DARKCOUNT-WHITECOUNT-1,DARK);
    PackOfCards hermitPack(HERMITCOUNT-DARKCOUNT-1,HERMIT);
    BOOST_CHECK_EQUAL(darkPack.listOfCards[0],DARK1);
    hermitPack.shuffle();
    Card drawnHermit = hermitPack.draw();
    std::cout << static_cast<Card>(drawnHermit) << std::endl;
    BOOST_CHECK_EQUAL(whitePack.listOfCards.size(), 2);
    BOOST_CHECK_EQUAL(whitePack.discardPile.size(), 0);
    BOOST_CHECK_EQUAL(whitePack.type, WHITE);
    std::cout << "White draw size: " << whitePack.listOfCards.size() << std::endl;
    std::cout << "White discard size: " << whitePack.discardPile.size() << std::endl;
    Card drawnWhite = whitePack.draw();
    BOOST_CHECK_EQUAL(drawnWhite, WHITE1);
    BOOST_CHECK_EQUAL(whitePack.listOfCards.size(), 1);
    whitePack.discard(drawnWhite);
    BOOST_CHECK_EQUAL(whitePack.discardPile.size(), 1);
    std::cout << "White draw size: " << whitePack.listOfCards.size() << std::endl;
    std::cout << "White discard size: " << whitePack.discardPile.size() << std::endl;
    Card drawnWhite2 = whitePack.draw();
    BOOST_CHECK_EQUAL(drawnWhite, WHITE1);
    BOOST_CHECK_EQUAL(whitePack.listOfCards.size(), 0);
    whitePack.discard(drawnWhite2);
    BOOST_CHECK_EQUAL(whitePack.discardPile.size(), 2);
    std::cout << "White draw size: " << whitePack.listOfCards.size() << std::endl;
    std::cout << "White discard size: " << whitePack.discardPile.size() << std::endl;
    Card drawnWhite3 = whitePack.draw();
    BOOST_CHECK_EQUAL(whitePack.discardPile.size(), 0);
    std::cout << "White draw size: " << whitePack.listOfCards.size() << std::endl;
    std::cout << "White discard size: " << whitePack.discardPile.size() << std::endl;
}

BOOST_AUTO_TEST_CASE(TestStateBoard) {
    Board bd;
    BOOST_CHECK((2 <= bd.rollDice(SUM) && bd.rollDice(SUM) <= 10));
    BOOST_CHECK_EQUAL(bd.drawDark(),DARK1);
    BOOST_CHECK_EQUAL(bd.drawWhite(),WHITE1);
    BOOST_CHECK_EQUAL(bd.drawHermit(),HERMIT1);

    std::vector<Player*> supposed_neighbours = bd.playerPos[OUTSIDE];
    supposed_neighbours.erase(supposed_neighbours.begin());
    BOOST_CHECK_EQUAL(bd.getNeighbours(*(bd.playerPos[OUTSIDE][0])).size(), supposed_neighbours.size());

    bd.playerPos[OUTSIDE][0]->receiveDamage(2);
    std::cout << "Dealt " << bd.playerPos[OUTSIDE][0]->wounds << " damage to Werewolf in OUTSIDE" << std::endl;
    bd.movePlayerTo(*(bd.playerPos[OUTSIDE][0]),GRAVEYARD);
    BOOST_CHECK_EQUAL(bd.getNeighbours(*(bd.playerPos[GRAVEYARD][0])).size(), 0);
    BOOST_CHECK_EQUAL(bd.getNeighbours(*(bd.playerPos[OUTSIDE][0])).size(), 0);
    BOOST_CHECK_EQUAL(bd.playerPos[OUTSIDE].size(),1);
    BOOST_CHECK_EQUAL(bd.playerPos[GRAVEYARD].size(),1);
    BOOST_CHECK_EQUAL(bd.playerPos[GRAVEYARD][0]->position,GRAVEYARD);

    std::cout << static_cast<Cell>(bd.getOtherCellInSameZone(GRAVEYARD)) << std::endl;
    std::cout << bd.playerPos[GRAVEYARD][0]->wounds << std::endl;
    std::cout << bd.playerPos[OUTSIDE][0]->wounds << std::endl;
    std::cout << "Werewolf in GRAVEYARD HAS " << bd.playerPos[GRAVEYARD][0]->wounds << " damages" << std::endl;
    
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
    BOOST_CHECK_EQUAL(wf.position,OUTSIDE);


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
    BOOST_CHECK_EQUAL(wf.position,OUTSIDE);
    BOOST_CHECK_EQUAL(wf.equipCards.size(),0);

    BOOST_CHECK_EQUAL(emi.wounds,0);
    BOOST_CHECK_EQUAL(emi.isAlive,true);
    BOOST_CHECK_EQUAL(emi.revealed,false);
    BOOST_CHECK_EQUAL(emi.capacityUsed,false);
    BOOST_CHECK_EQUAL(emi.position,OUTSIDE);
    BOOST_CHECK_EQUAL(emi.equipCards.size(),0);

    BOOST_CHECK_EQUAL(franklin.wounds,0);
    BOOST_CHECK_EQUAL(franklin.isAlive,true);
    BOOST_CHECK_EQUAL(franklin.revealed,false);
    BOOST_CHECK_EQUAL(franklin.capacityUsed,false);
    BOOST_CHECK_EQUAL(franklin.position,OUTSIDE);
    BOOST_CHECK_EQUAL(franklin.equipCards.size(),0);

    BOOST_CHECK_EQUAL(georges.wounds,0);
    BOOST_CHECK_EQUAL(georges.isAlive,true);
    BOOST_CHECK_EQUAL(georges.revealed,false);
    BOOST_CHECK_EQUAL(georges.capacityUsed,false);
    BOOST_CHECK_EQUAL(georges.position,OUTSIDE);
    BOOST_CHECK_EQUAL(georges.equipCards.size(),0);

    BOOST_CHECK_EQUAL(unknown.wounds,0);
    BOOST_CHECK_EQUAL(unknown.isAlive,true);
    BOOST_CHECK_EQUAL(unknown.revealed,false);
    BOOST_CHECK_EQUAL(unknown.capacityUsed,false);
    BOOST_CHECK_EQUAL(unknown.position,OUTSIDE);
    BOOST_CHECK_EQUAL(unknown.equipCards.size(),0);

    BOOST_CHECK_EQUAL(vampire.wounds,0);
    BOOST_CHECK_EQUAL(vampire.isAlive,true);
    BOOST_CHECK_EQUAL(vampire.revealed,false);
    BOOST_CHECK_EQUAL(vampire.capacityUsed,false);
    BOOST_CHECK_EQUAL(vampire.position,OUTSIDE);
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

    bd.movePlayerTo(emi,GATE);
    std::cout << "Emi is in Cell " << static_cast<Cell>(emi.position) << std::endl;
    std::cout << "Next to Cell " << static_cast<Cell>(emi.position) << " is the Cell " << bd.getOtherCellInSameZone(emi.position) << std::endl;
    emi.revealed = true;
    emi.useCapacity();
    std::cout << "Emi is in Cell " << static_cast<Cell>(emi.position) << std::endl;

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
