#include "Board.h"
#include <random>
#include <algorithm>
#include <iostream>
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
        CellClass *Hermitzone = new CellClass(HERMITZONE, std::vector<int>{2, 3});
        CellClass *Gate = new CellClass(GATE, std::vector<int>{4, 5});
        CellClass *Church = new CellClass(CHURCH, std::vector<int>{6});
        CellClass *Graveyard = new CellClass(GRAVEYARD, std::vector<int>{8});
        CellClass *Woods = new CellClass(WOODS, std::vector<int>{9});
        CellClass *Altar = new CellClass(ALTAR, std::vector<int>{10});
        CellClass *Out = new CellClass(OUTSIDE, std::vector<int>{0});

        cellList = {Graveyard, Altar, Hermitzone, Woods, Gate, Church};
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cellList.begin(), cellList.end(), g);
        for (int i = 0; i < 3; i++)
        {
            cellList[2 * i]->setZone(i);
            cellList[2 * i + 1]->setZone(i);
        }

        playerList.emplace_back(new Werewolf(this));
        playerList.emplace_back(new Franklin(this));
        playerList.emplace_back(new Vampire(this));
        playerList.emplace_back(new Georges(this));
        for (auto &player : playerList)
        {
            player->position = Out;
        }
        playerList[0]->id = 0;
        playerList[1]->id = 1;
        playerList[2]->id = 2;
        playerList[3]->id = 3;

        Out->playersInCell = {playerList[0].get(), playerList[1].get(), playerList[2].get(), playerList[3].get()};
        cellList.emplace_back(Out);
        Out->setZone(-1);

        defineGameOrder(playerList);

        darkPack.shuffle();
        whitePack.shuffle();
        hermitPack.shuffle();
    }

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

    CardClass *Board::drawDark()
    {
        CardClass *drawnCard = darkPack.draw();
        notifyObservers(CARD_CHANGED);
        return drawnCard;
    }

    CardClass *Board::drawWhite()
    {
        CardClass *drawnCard = whitePack.draw();
        notifyObservers(CARD_CHANGED);
        return drawnCard;
    }

    CardClass *Board::drawHermit()
    {
        CardClass *drawnCard = hermitPack.draw();
        notifyObservers(CARD_CHANGED);
        return drawnCard;
    }

    std::vector<Player *> Board::getNeighbours(Player *player)
    {
        CellClass *pos1 = player->position;

        CellClass *pos2 = getOtherCellInSameZone(pos1);

        // list of players in the same zone
        std::vector<Player *> neighbours = {};

        std::find(pos1->playersInCell.begin(), pos1->playersInCell.end(), player);

        std::vector<Player *> temp1 = pos1->playersInCell;
        std::vector<Player *> temp2 = pos2->playersInCell;
        // check if they are neighbour or the player itself
        for (Player *neighbour : temp1)
        {
            if (neighbour != player)
            {
                neighbours.emplace_back(neighbour);
            }
        }
        if (pos1->cell != OUTSIDE)
        {
            for (Player *neighbour : temp2)
            {
                neighbours.emplace_back(neighbour);
            }
        }
        return neighbours;
    }

    void Board::movePlayerTo(Player *player, CellClass *newPos)
    {
        CellClass *oldPos = player->position;
        player->position = newPos;

        oldPos->playersInCell.erase(std::find(oldPos->playersInCell.begin(), oldPos->playersInCell.end(), player));

        newPos->playersInCell.emplace_back(player);
        notifyObservers(BOARD_CHANGED);
    }

    void Board::equipCard(Player &player, CardClass* card)
    {
        player.equipCards.push_back(card);
        notifyObservers(CARD_CHANGED);
    }

    CellClass *Board::dieToCell(int die)
    {
        //die must be different fromm 7
        const auto it = std::find_if(cellList.begin(), cellList.end(),
                                     [die](CellClass *c)
                                     { return (c->isDieToThisCell(die) == 1); });
        if (it != cellList.end())
        {
            return *it;
        }
        return cellList.back();
    }

    CellClass *Board::getOtherCellInSameZone(CellClass *cell)
    {
        int Zone = cell->zone;

        // get the other CEll in the same zone
        const auto it = std::find_if(cellList.begin(), cellList.end(),
                                     [Zone, cell](const CellClass *c)
                                     { return ((c->zone == Zone) and (c->cell != cell->cell)); });
        if (it != cellList.end())
        {
            return *it;
        }
        return cell;
    }

    /**
     * @brief define the order of the players in the game
     * by modifying the id of the player
     * @param playerList the list of players
     */
    void Board::defineGameOrder(std::vector<std::unique_ptr<Player>> &playerList)
    {
        // int nbPlayer = playerList.size();
        // std::vector<int> assignedIDs;

        // for(int i=0 ; i < nbPlayer; i++){
        //     assignedIDs.push_back(i);
        // }

        // for(int i=0;i<nbPlayer;i++){
        //     int randNb = std::rand() % assignedIDs.size();
        //     playerList[i]->id =assignedIDs[randNb];
        //     assignedIDs.erase(assignedIDs.begin()+randNb);
        // }

        // std::sort(playerList.begin(), playerList.end(),
        // [](const std::unique_ptr<Player>& a, const std::unique_ptr<Player>& b) {
        //     return a->id < b->id;
        // });

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(playerList.begin(), playerList.end(), g);
        for (int i = 0; i < playerList.size(); i++)
        {
            playerList[i]->id = i;
        }
    }

    void Board::discardCard(CardClass* card){
        switch (card->type)
        {
        case HERMIT:
            hermitPack.discard(card);
            break;
        case WHITE:
            whitePack.discard(card);
            break;
        case DARK:
            darkPack.discard(card);
            break;
        default:
        std::cout<<"error discarding"<<std::endl;
            break;
        }
        notifyObservers(CARD_CHANGED);
    }
}