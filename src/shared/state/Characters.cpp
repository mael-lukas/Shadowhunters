#include "Emi.h"
#include "Franklin.h"
#include "Unknown.h"
#include "Georges.h"
#include "Werewolf.h"
#include "Vampire.h"
#include <iostream>

namespace state{
    // Define the different characters and overwrite some function


    // Define the constructor of each character

    // Define Emi : 10 Hp, hunter
    Emi::Emi(Board* board):Player::Player(board,10,HUNTER){};
    // Franklin : 12 Hp, hunter
    Franklin::Franklin(Board* board):Player::Player(board,12,HUNTER){};
    // Georges: 14 Hp, hunter
    Georges::Georges(Board* board):Player::Player(board,14,HUNTER){};
    //Vampire: 13 Hp, shadow
    Vampire::Vampire(Board* board):Player::Player(board,13,SHADOW){};
    //Unknown: 11 Hp, shadow
    Unknown::Unknown(Board* board):Player::Player(board,11,SHADOW){};
    //Werewolf: 14 Hp, shadow
    Werewolf::Werewolf(Board* board):Player::Player(board,14,SHADOW){};

    //Overwrite some function

    void Emi::useCapacity()
    {
        if (revealed)
        {
            CellClass* currentCell = position;

            CellClass* otherCell = board->getOtherCellInSameZone(currentCell);

            if (otherCell->cell != currentCell->cell)
            {
                board->movePlayerTo(this, otherCell);
            }
        }
    }

    void Franklin::useCapacity()
    {

        if (revealed)
        {
            if (capacityUsed)
            {
                return;
            }
            capacityUsed = true;

            // Fait par l'engine
            // Player *target = engine->chooseTarget(*this);
            // int dmg = board->rollDice(ONLYD4);
            // if (target)
            //  target->receiveDamage(dmg);
        }
    }

    void Georges::useCapacity()
    {

        if (revealed)
        {
            if (capacityUsed) {
                return;
            }
            capacityUsed = true;

            // Fait par l'engine
            // Player *target = engine->chooseTarget(*this);
            // int dmg = board->rollDice(ONLYD6);
            //: if (target)
            // target->receiveDamage(dmg);
        }
    }

    bool Vampire::attackOther(Player &other)
    {
        if (revealed == false)
        {
            return Player::attackOther(other);
        }
        else
        {
            int damage = board->rollDice(DIFF);
            // Calcul des dégâts après défense/équipement
            int damage_supposed = other.getAttacked(*this, damage);
            // Si des dégâts ont effectivement été infligés → soin de 2
            if (damage_supposed > 0)
            {
                this->receiveDamage(-2);
                int real_damage = damage_supposed;
                return dealDamage(damage_supposed, other);
            }
            else
            {
                return false;
            }
        }
    }

    int Werewolf::getAttacked(Player &source, int damage)
    {
        if (revealed == false)
        {
            std::cout << "[CAPACITY] Werewolf not revealed, normal damage" << std::endl;
            return Player::getAttacked(source, damage);
        }
        else
        {
            std::cout << "[CAPACITY] Werewolf counter attacks" << std::endl;
            int counter = board->rollDice(DIFF);
            dealDamage(counter, source);
            return Player::getAttacked(source, damage);
        }
    }
}