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


    void Emi::useCapacity() {
        /*if(!revealed){
            // On effectue le déplacement vers l'autre cellule de la même zone
             std::vector<Player*> neighbours = board->getNeighbours(*this);

                for(Player* player : neighbours) {
                if(cell.getZone() == board->getCell(*this).getZone() &&
                cell.getId() != board->getCell(*this).getId()) {
                    board->movePlayerTo(*this, cell);
                    break; // Déplacement effectué, on arrête 
                }
            }
        }*/
    }


    void Franklin::useCapacity() {
        /*if (!revealed){
            if (capacityUsed) return; // déjà utilisé
            capacityUsed = true;

            int dmg = board->rollDice(ONLYD4);
            Player* target = board->chooseTarget(*this);
            if (target) target->receiveDamage(dmg);
        }*/
    }

    void Georges::useCapacity() {
       /* if (!revealed){
            if (capacityUsed) return; // déjà utilisé
            capacityUsed = true;

            int dmg = board->rollDice(ONLYD6);
            Player* target = board->chooseTarget(*this);
            if (target) target->receiveDamage(dmg);
        }*/
    }

    
    bool Vampire::attackOther(Player& other){
        if(!revealed){
            Player::attackOther(other);
        }
        else{
            
            int damage =board->rollDice(DIFF);
             // Calcul des dégâts après défense/équipement
            int damage_supposed = other.getAttacked(*this,damage);
            // Si des dégâts ont effectivement été infligés → soin de 2
            if (damage_supposed > 0){
                this->receiveDamage(-2);
                int real_damage = damage_supposed;
                return dealDamage(damage_supposed,other);
            }else{
                return  false;
            }
            
        }
    }


    



}