#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"

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
    BOOST_CHECK_EQUAL(bd.rollDice(SUM),0);
    BOOST_CHECK_EQUAL(bd.drawDark(),DARK1);
    BOOST_CHECK_EQUAL(bd.drawWhite(),WHITE2);
    BOOST_CHECK_EQUAL(bd.drawHermit(),HERMIT1);
    //BOOST_CHECK_EQUAL(bd.getNeighbours(*(playerPose[OUTSIDE][0])));

}

BOOST_AUTO_TEST_CASE(TestStatePlayer) {
    Board bd;
    Player ptest(&bd,14,SHADOW);
    //Player p1=bd.playerPos[OUTSIDE][0];
    //BOOST_CHECK_EQUAL(p1.getPosition(),14);
}
