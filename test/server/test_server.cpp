#include <boost/test/unit_test.hpp>

#include "../../src/server/server.h"

using namespace server;
BOOST_AUTO_TEST_CASE(TestServer)
{
    Server serv = Server(); 
    serv.run();
}



