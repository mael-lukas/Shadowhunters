#include "Player.h"

namespace state
{
    Player::Player(Board* board,int maxHP,Role role) : board(board),maxHP(maxHP),role(role)
    {
        position= board->cellList.back();
    }


    int Player::getAttacked(Player& attacker,int damage){
        // check if equipment reduce damage TODO
        return damage; // par défaut, pas de défense
    }

    bool Player::receiveDamage(int damage) {
        wounds += damage;
        if (wounds < 0) wounds = 0;
        if (wounds >= maxHP) {
            wounds = maxHP;
            isAlive = false;
        }
        notifyObservers(StateEventID::PLAYER_CHANGED);
        return isAlive;
    }

    bool Player::dealDamage(int damage,Player& victim) {
        bool alive = victim.receiveDamage(damage);
        return alive;
    }
    
    bool Player::attackOther(Player &player) {
        //check equipment TODO

        //roll the attack dice
        int damage = board->rollDice(DIFF);

        // check if reduction by equipment
        int damage_supposed = player.getAttacked(*this,damage);
        // inflict real damage and check if it killed
        int real_damage = damage_supposed;
        return dealDamage(damage_supposed,player);
    }

    void Player::useCapacity() {
    // Par défaut : rien
    }

}
