#include "GateCommand.h"
#include "Engine.h"
#include "DrawCardCommand.h"
#include <iostream>
namespace engine{


    GateCommand::GateCommand(Engine& engine):engine(engine){}


    void GateCommand::execute(){
        if (isWaitingForSelection) {
            engine.isWaitingForGatePrompt = true;
            engine.waitingCommand = this;
            return;
        }
        if (isPromptCancelled){
            engine.currentTurnPhase = BATTLE_PHASE;
            engine.isWaitingForWoodsPrompt = false;
        }
        else{
            engine.commands.emplace_back(new DrawCardCommand(engine,typeSelected,not isPromptCancelled));
        }
        isDone=true;
    }


    void GateCommand::receivePromptAnswer(void* answer){
        int targetType = *static_cast<int*>(answer);
        if (targetType == 3){
            std::cout<< "type 3 selected" << std::endl;
            isPromptCancelled = true;
            typeSelected=state::WHITE;
        }
        else {
            typeSelected = *static_cast<state::CardType*>(answer);

        }
        isWaitingForSelection=false;
        engine.waitingCommand = nullptr;
        engine.isWaitingForGatePrompt = false;
    }

}