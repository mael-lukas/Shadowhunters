#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"

// tester les destructeurs

using namespace state;

BOOST_AUTO_TEST_CASE(TestState) {
    PackOfCards whitePack(WHITECOUNT, WHITE);
    PackOfCards darkPack(DARKCOUNT-WHITECOUNT-1,DARK);
    BOOST_CHECK_EQUAL(whitePack.packSize, 2);
    BOOST_CHECK_EQUAL(whitePack.listOfCards.size(), 2);
    BOOST_CHECK_EQUAL(whitePack.type, WHITE);
    Card drawnCard = whitePack.draw();
    BOOST_CHECK_EQUAL(drawnCard, WHITE1);
    BOOST_CHECK_EQUAL(darkPack.listOfCards[0],DARK1);
    
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
    Board bd;
    Player ptest(&bd,14,SHADOW);
    Werewolf wf(&bd);
    //BOOST_CHECK_EQUAL(wf.getHp(),14);
    //BOOST_CHECK_EQUAL(wf.getType(),SHADOW);
}
