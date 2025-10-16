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
    //Wait getAttacked to be implemented
    //BOOST_CHECK_EQUAL(wf.attackOther(emi),false);
    
}
