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
    //BOOST_CHECK_EQUAL(bd.getNeighbours(*(playerPose[OUTSIDE][0])));

}

BOOST_AUTO_TEST_CASE(TestStatePlayer) {
    Board bd;
    Player ptest(&bd,14,SHADOW);
    Werewolf wf(&bd);
    //BOOST_CHECK_EQUAL(wf.getHp(),14); 
    //BOOST_CHE
}
