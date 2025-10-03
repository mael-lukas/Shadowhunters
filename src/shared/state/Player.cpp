#include "Player.h"

namespace state
{
    Player::Player(Board* board,int maxHP,Role role) : board(board),maxHP(maxHP),role(role)
    {
    }
    bool Player::attackOther(Player &player)
    {
        //check equipment TODO

        //roll the attack dice
        int damage =board->rollDice(DIFF);

        // check if reduction by equipment
        int damage_supposed = player.getAttacked(*this,damage);
        // inflict real damage and check if it killed
        int real_damage = damage_supposed;
        return dealDamage(damage_supposed,player);
    }

    int Player::getAttacked(Player& attacker,int damage)
    {

    }

    bool Player::dealDamage(int damage,Player& victim)
    {

    }

    bool Player::receiveDamage(int damage)
    {

    }

}
