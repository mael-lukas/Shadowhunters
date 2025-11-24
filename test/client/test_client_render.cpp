#include <boost/test/unit_test.hpp>
#include <SFML/Graphics.hpp>
#include "../../src/shared/state.h"
#include "../../src/client/render.h"
#include "../../src/client/client.h"

using namespace state;
using namespace render;
using namespace client;
using namespace engine;

/*BOOST_AUTO_TEST_CASE(TestRender)
{
  Board bd;
  RenderManager rm(&bd);
  rm.init();
  bool hasMovedPlayer = false;
  bool hasEquippedCard = false;
  while (rm.window.isOpen()) {
    sf::Event event;
    while (rm.window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        rm.window.close();
      }
      //rm.handleEvent(event);
    }
    rm.window.clear();
    rm.board_render.draw();
    rm.player_render.draw();
    rm.card_render.draw();
    rm.window.display();

    static int counter = 1;

    if (counter++ % 200 == 0) {
      bd.playerPos[OUTSIDE][0]->receiveDamage(1);
    }
    if (!hasMovedPlayer && counter % 400 == 0) {
      bd.movePlayerTo(*(bd.playerPos[OUTSIDE][0]), GRAVEYARD);
      hasMovedPlayer = true;
    }
    if (!hasEquippedCard && counter % 600 == 0) {
      Card c = bd.drawDark();
      bd.playerPos[GRAVEYARD][0]->equipCards.push_back(c);
      hasEquippedCard = true;
    }
    sf::sleep(sf::milliseconds(15));
  }
}*/

BOOST_AUTO_TEST_CASE(TestRenderWithClient)
{
  Board bd;
  Engine eng(&bd);
  RenderManager rm(&bd);
  Client client(&bd, &rm, &eng);
  for (state::Cell cell : bd.listOfCells) {
      std::cout << "Cell in board: " << static_cast<int>(cell) << std::endl;
  }
  client.run();
}


