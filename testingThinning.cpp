#include "StandManager.h"
#include "thinningTask.h"
#include <iostream>

int main(){
    StandObject erikssonStand(32,10,57,3500,true); //new StandObject and pointer.
    StandObject* standPtr =  &erikssonStand;//pointer to stand
    double* BAPtr = &erikssonStand.BasalAreaM2.back(); //pointer to last element of Basal Area
    int* agePtr = &erikssonStand.AgeBH.back(); //pointer to last element of AgeBH.
    
    thinningList ThinningList = thinningList();//instantiate new thinningList

    ThinningList.addDblTask(BAPtr,22,1,5);//add double limit
    ThinningList.addIntTask(agePtr,15,2,32,0.3);//add integer limit

    task<int>* intMatch = ThinningList.getMatchInt();//check for match
    task<double>* dblMatch = ThinningList.getMatchDbl();//check for match  

    std::cout   << "Double " << dblMatch
                << "Int " << intMatch 
                << std::endl;

    return 0;
}