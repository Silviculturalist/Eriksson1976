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

        void increment(){ thinner->processThinning(); stand->growth_fn(); }; //increment the stand.
        void increment(int periods) //increment multiple times at once.
        {
            for(int i=0;i<periods;i++)
            {
                thinner->processThinning();
                stand->growth_fn();

            }
        }; //increment several times.

        void reportManagedStands();
};

void StandManager::reportManagedStands(){
    std::cout << "StandID: " << stand << std::endl;
    stand->result();
}