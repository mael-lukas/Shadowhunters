#include <iostream>
#include "../shared/state.h"
#include "render.h"
#include "client.h"

// The following lines are here to check that SFML is installed and working
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}
// end of test SFML

using namespace std;
using namespace state;
using namespace render;
using namespace client;
using namespace engine;

int main(int argc,char* argv[])
{
    Board bd;
    Engine eng(&bd);
    RenderManager rm(&bd);
    Client client(&rm, &eng);
    client.run();
    return 0;
}