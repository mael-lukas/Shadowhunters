#include "Player.h"

namespace state
{
    Player::Player(Board* board,int maxHP,Role role, CharacterName name) : board(board),maxHP(maxHP),role(role),name(name)
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
            board->movePlayerTo(this, board->cellList.back());
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

    Role Player::getRole() {
        return role;
    }

    void Player::equipACard(CardClass* card) {
        equipCards.push_back(card);
        notifyObservers(StateEventID::PLAYER_CHANGED);
    }

}
