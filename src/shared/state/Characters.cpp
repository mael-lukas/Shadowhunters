#include "Emi.h"
#include "Franklin.h"
#include "Unknown.h"
#include "Georges.h"
#include "Werewolf.h"
#include "Vampire.h"

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
            // Récupère la cellule actuelle d'Emi
            Cell currentCell = position;

            Cell otherCell = board->getOtherCellInSameZone(currentCell);

            // Si une autre cellule a bien été trouvée (différente)
            if (otherCell != currentCell)
            {
                board->movePlayerTo(*this, otherCell);
            }
        }
    }

    void Franklin::useCapacity()
    {

        if (revealed)
        {
            if (capacityUsed)
                return; // déjà utilisé
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
            if (capacityUsed)
                return; // déjà utilisé
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
        if (!revealed)
        {
            Player::attackOther(other);
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
        if (!revealed)
        {
            return Player::getAttacked(source, damage);
        }
        else
        {
            return Player::getAttacked(source, damage);
            int counter = board->rollDice(DIFF);
            return dealDamage(counter, source);
        }
    }
}