#include "Board.h"
#include <random>
#include <algorithm>

namespace state {
    Board::Board () : darkPack(WHITECOUNT, WHITE), whitePack(DARKCOUNT-WHITECOUNT-1, DARK), hermitPack(HERMITCOUNT-DARKCOUNT-1, HERMIT) {
        dieToCell[2] = HERMITZONE;
        dieToCell[3] = HERMITZONE;
        dieToCell[4] = GATE;
        dieToCell[5] = GATE;
        dieToCell[6] = CHURCH;
        dieToCell[8] = GRAVEYARD;
        dieToCell[9] = WOODS;
        dieToCell[10] = ALTAR;

        std::vector<Cell> temp = {GRAVEYARD, ALTAR, HERMITZONE, WOODS, GATE, CHURCH};
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(temp.begin(), temp.end(), g);
        for (int i = 0; i < 3; i++) {
            cellToZone[temp[i]] = i;
            cellToZone[temp[i+1]] = i;
        }

        
        Player p1(this,14,SHADOW);
        Player p2(this,12,HUNTER);

        playerPos[GRAVEYARD] = {};
        playerPos[ALTAR] = {};
        playerPos[HERMITZONE] = {};
        playerPos[WOODS] = {};
        playerPos[GATE] = {};
        playerPos[CHURCH] = {};
        playerPos[OUTSIDE] = {&p1,&p2};

    }
    
    int Board::rollDice (RollRule rule) {
        return 0; // TODO : implement
    }
    
    Card Board::drawDark () {
        return darkPack.listOfCards[0]; // TODO : implement
    }
    
    Card Board::drawWhite () {
        return whitePack.listOfCards[1]; // TODO : implement
    }
    
    Card Board::drawHermit () {
        return hermitPack.listOfCards[0]; // TODO : implement
    }
    
    std::vector<Player> Board::getNeighbours (Player& player) {
        return {}; // TODO : implement
    }
}