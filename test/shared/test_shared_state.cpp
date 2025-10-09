#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"

using namespace state;

BOOST_AUTO_TEST_CASE(TestState) {
    Board testBoard;
    BOOST_CHECK_EQUAL(testBoard.dieToCell[2], HERMITZONE);
}
