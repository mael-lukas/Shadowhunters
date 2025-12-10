#include "CellClass.h"
#include <algorithm>

namespace state{
    CellClass::CellClass(Cell cell,std::vector<int> dices):cell(cell),listOfDices(dices){

    }
    void CellClass::setZone(int zone){
        this->zone=zone;
    }

    int CellClass::isDieToThisCell(int die){
        auto it = std::find(listOfDices.begin(),listOfDices.end(),die);
        if(it !=listOfDices.end()){
            return 1;
        }
        return 0;
    }
}