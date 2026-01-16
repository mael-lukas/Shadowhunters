#include "CardClass.h"


namespace state {

    CardClass::CardClass(Card name, CardType type, CardEffectTime effectTimer) : name(name), type(type), effectTimer(effectTimer){
        if(type==HERMIT){
            if((name==HERMIT1)|(name==HERMIT2)){
                info=ISHUNTER;
                effect=RECEIVE1;
            }
            else if((name == HERMIT3)|(name==HERMIT4)){
                info=ISSHADOW;
                effect=RECEIVE1;
            }
        }
    }
}