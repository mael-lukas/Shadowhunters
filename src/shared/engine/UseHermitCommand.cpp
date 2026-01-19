#include "UseHermitCommand.h"
#include "Engine.h"
#include <algorithm>
#include "../../client/client/HermitGiveReceive.h"
#include <iostream>
using namespace engine;
using namespace state;

UseHermitCommand::UseHermitCommand(Engine& engine, state::CardClass card)
    : engine(engine), card(card)
{
    bool isWaitingForInfo     = false;
    bool isWaitingForTarget     = true;
    state::Player* target     = nullptr;
}

void UseHermitCommand::execute()
{

    if (isWaitingForTarget) {
            engine.isWaitingForHermitTargetPrompt = true;
            engine.waitingCommand = this;
            return;
        }
    else if(isWaitingForInfo) {
            engine.isWaitingForHermitInfoPrompt = true;
            engine.isWaitingForHermitTargetPrompt = false;
            return;
        }   
    else {
        state::Player& currentPlayer = engine.getCurrentPlayer();
        engine.board->discardCard(&card);
        currentPlayer.equipCards.erase(std::find(currentPlayer.equipCards.begin(),currentPlayer.equipCards.end(),&card));
        target->equipCards.erase(std::find(target->equipCards.begin(),target->equipCards.end(),&card));
        engine.currentTurnPhase = BATTLE_PHASE;
        isDone=true;
    }
    
       }
void UseHermitCommand::receivePromptAnswer(void* answer)
    {
        if (isWaitingForTarget) {
            state::Player& currentPlayer = engine.getCurrentPlayer();
            int targetId =*static_cast<int*>(answer);
            target= engine.board->playerList[targetId].get();
            target->equipACard(&card);
            engine.hermitId = targetId;
            engine.isWaitingForHermitTargetPrompt = false;
            isWaitingForTarget = false;
            isWaitingForInfo=true;
            std::cout<<"part 2 of hermit"<< std::endl;
        }
        else if (isWaitingForInfo){
            client::HermitGiveReceive* info= static_cast<client::HermitGiveReceive*>(answer);
            switch (card.effect)
            {
            case HEALORRECEIVE1:
            case RECEIVE1: 
            case RECEIVE2:{
                
                target->receiveDamage(info->receive);
                break;}
            case GIVEORRECEIVE1:{
                client::HermitGiveReceive* answer;
                if(info->choice==1){// 0 is none 1 is receive 2 is give
                    target->receiveDamage(info->receive);
                };
                break;}
            case SHOW:
                break;
            default:
                break;
            
            } 
            engine.waitingCommand = nullptr;
            engine.isWaitingForHermitInfoPrompt = false;
            isWaitingForInfo=false;
            std::cout<<"last part"<<std::endl;
        }
        else {
            engine.waitingCommand = nullptr;
            engine.isWaitingForHermitInfoPrompt = false;
            engine.isWaitingForHermitTargetPrompt = false;
            isWaitingForTarget = false;
        }
    }
