#include "Board.h"
#include <random>
#include <algorithm>
#include "Franklin.h"
#include "Emi.h"
#include "Georges.h"
#include "Werewolf.h"
#include "Vampire.h"
#include "Unknown.h"

namespace state
{
    Board::Board() : whitePack(WHITECOUNT, WHITE), darkPack(DARKCOUNT - WHITECOUNT - 1, DARK), hermitPack(HERMITCOUNT - DARKCOUNT - 1, HERMIT)
    {
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
        for (int i = 0; i < 3; i++)
        {
            cellToZone[temp[i]] = i;
            cellToZone[temp[i + 1]] = i;
        }

        
        playerList.emplace_back(new Werewolf(this));
        playerList.emplace_back(new Franklin(this));

        playerPos[GRAVEYARD] = {};
        playerPos[ALTAR] = {};
        playerPos[HERMITZONE] = {};
        playerPos[WOODS] = {};
        playerPos[GATE] = {};
        playerPos[CHURCH] = {};
        playerPos[OUTSIDE] = {playerList[0].get(),playerList[1].get()};
    }


    /** Create 2 dice one D4 and one D6 and return either the sum of both (rule=SUM),
     * the absolute difference of both (rule=DIFF),
     * only the D4 (rule=ONLYD4),
     * only the D6 (rule=ONLYD6).
     * @tparam rule  the type of result expected.
     */
    int Board::rollDice(RollRule rule)
    {

        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<int> D4(1, 4);
        std::uniform_int_distribution<int> D6(1, 6);
        int d4_result = D4(g);
        int d6_result = D6(g);
        if (DIFF == rule)
            return abs(d6_result - d4_result);
        else if (SUM == rule)
            return d6_result + d4_result;
        else if (ONLYD4 == rule)
            return d4_result;
        else if (ONLYD6 == rule)
            return d6_result;
        else
            return 0; // TODO : implement
    }

    Card Board::drawDark()
    {
        return darkPack.draw(); // TODO : implement
    }

    Card Board::drawWhite()
    {
        return whitePack.draw(); // TODO : implement
    }

    Card Board::drawHermit()
    {
        return hermitPack.draw(); // TODO : implement
    }

    std::vector<Player> Board::getNeighbours(Player &player)
    {
        // Get player pos
        Cell pos = player.position;
        Cell pos2;
        // Get zone
        int Zone = cellToZone[pos];
        
        
        std::vector<Player> neighbours = {};

        //get the other zone
        for (std::map<Cell, int>::iterator iter = cellToZone.begin(); iter != cellToZone.end(); ++iter)
        {
            Cell k = iter->first;
            // ignore value
            int v = iter->second;
            if (v = Zone and k != pos)
            {
                pos2 = k;
            }
        }
        // Get players in zone
        
        
        return {}; // TODO : implement
    }
}