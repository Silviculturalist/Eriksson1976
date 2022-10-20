#pragma once 
#include "Stand.h"
#include "ThinningManager.h"

//Takes a stand and a ThinningManager
class StandManager{
    public:
        StandObject* stand; //pointer to stand
        ThinningManager* thinner; //pointer to ThinningManager

        //Constructor and Destructor
        StandManager(StandObject* stand): stand(stand){};
        StandManager(StandObject* stand, ThinningManager* thinner):
            stand(stand),
            thinner(thinner)
            {}; //constructor body after the initializer list.

        ~StandManager(){};
        
        int getIncrement();
        void increment(){ int period=getIncrement(); thinner->processThinning(); stand->growth_period(period); }; //increment the stand by a standard amount
        void increment(int periods) //increment by a set period
        {
            for(int i=0;i<periods;i++)
            {
                int period = getIncrement(); //get number of years to increment
                thinner->processThinning();
                stand->growth_period(period);

            }
        }; //increment several times.

        void reportManagedStands();
};

//get number of years to increment 
int StandManager::getIncrement(){
    return (stand->Age.back()<=29)?6:(stand->Age.back()<=49)?8:10; 
    
    //bool thinAtInterval = ((thinner->thinInterval)>0)?true:false; //does thinning happen at set interval?
    //bool thinAtDomHeight = ((thinner->))

}

void StandManager::reportManagedStands(){
    std::cout << "StandID: " << stand << std::endl;
    stand->result();
}