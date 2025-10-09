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

    bool Vampire::attackOther(Player& other){
        if(!revealed){
            Player::attackOther(other);
        }
        else{
            return 0; //TODO
        }
    }



}