#pragma once
#include <limits>
#include "Stand.h"

//Definition for a ThinningManager that controls thinning for a StandObject.
class ThinningManager {
    public:
        StandObject* stand; //ptr to the associated StandObject.
        double minThinInitVol{0}; //0 for nonlimiting.
        double maxThinInitVol{std::numeric_limits<double>::max()}; //max for nonlimiting.
        int thinInterval{0}; //zero for no interval
        double minThinRemove{0}; //smallest possible value for nonlimiting.
        double minLeaveVol{0}; //0 for not limiting, eg full harvest.
        int minThinAge{0}; //-1 for not limiting.. lowest acceptable age for thinning.
        int maxThinAge{std::numeric_limits<int>::max()};
        int firstThinYear{0}; //Counter for when first thinning was held.
        int thinCounter{0}; //to be able to set firstThinYear and report total thinnings.
        double repeatAbs{0}; //repeat thinning absolute value
        double repeatPerc{0}; //repeat thinning percentual value.
        std::vector<double> ThinVolArray = {0}; //only contains 1 element for start. A list of volumes to be removed at each period.

        //Constructor and Destructor
        ThinningManager(StandObject* stand): stand(stand){}; //Takes a stand
        ~ThinningManager(){};

        double volumeDecision(); //Calculate volume to be removed.
        bool thinningOK(); //Check if thinning is allowed.

        void setMinThinAge(int age)
        {
            minThinAge = (age>0)?age:0;
        }

        void setRepeatAbs(double cut)
        {
            repeatAbs = (repeatAbs<0)?0:repeatAbs;//truncate to zero if less than zero.
            repeatAbs= cut;
            repeatPerc= 0;
        };

        void setRepeatPerc(double percentage)
        {
            percentage = (percentage<0)?0:percentage; //truncate to zero if less than zero.
            percentage = (percentage>100)?100:percentage; //truncate to 100 percent if more than 100 percent.
            repeatPerc= percentage;
            repeatAbs=0;
        };

        void setThinInterval(int interval)
        {
            (interval<0)?0:interval;
            thinInterval = interval;
        };

        void processThinning()
        { //process a thinning. calculate and conduct.

            double vol = volumeDecision();
            stand->cut(vol);
        };
};

bool ThinningManager::thinningOK()
{
    bool thinningVal{false};

    thinningVal = 
        (
            (stand->Volume.back()) >= minThinRemove && //at least minimum level of harvest exists.
            (stand->Volume.back()) >= minThinInitVol && //at least initial volume of harvest exists.
            (stand->Volume.back()) <= maxThinInitVol && //at most maximum volume at which thinning is allowed.
            (stand->Volume.size()) >= minThinAge && //at least minimum thinning age.
            (stand->Volume.size()) <= maxThinAge //at most maximum thinning age.
        )?true:false; //stock is smaller than the largest thinning is allowed at.
    
    return thinningVal;
}

double ThinningManager::volumeDecision()
{
    double volDecision{0}; //Default harvest nothing.
    bool decisionTree{false}; //has another setting taken a decision?

    //Can thinning be conducted?
    if(thinningOK())
    {
        //Scheduled harvesting takes priority.
        if((ThinVolArray[0]>0 && ThinVolArray.size()>stand->Volume.size()) | (ThinVolArray.size()>1 && ThinVolArray.size()>stand->Volume.size())) //If list of volumes to harvest is greater than default 0.. (limit zero)
        {
            if (ThinVolArray.at(stand->Volume.size())>0) //check if any stated value at latest year. 
            {
                volDecision = ThinVolArray.at(stand->Volume.size());
                decisionTree = true; //indicate a decision has been taken.
                thinCounter++; //Increment number of thinnings.
            }
        }
        
        if(decisionTree==false && thinInterval!=0)
        {
            //If first thinning has been done
            if(thinCounter>0)
            {
                if((((stand->Volume.size())-firstThinYear) % thinInterval)==0) //time since first thinning evenly divisible by interval?
                {
                    volDecision = std::max(repeatAbs,((repeatPerc/100)*(stand->Volume.back()))); //largest of repeatAbs or repeatPerc
                    decisionTree = true;
                    thinCounter++; //increment number of thinnings.

                }
            }

            //If first thinning has not been done.
            else if(thinCounter==0)
            {
                    volDecision = std::max(repeatAbs,((repeatPerc/100)*(stand->Volume.back()))); //largest of repeatAbs or repeatPerc
                    decisionTree = true;
                    firstThinYear = int(stand->Volume.size()); //set age at first thinning.
                    thinCounter++; //first increment of thinCounter.

            }

        }
    }

    //Check volDecision logic.
    volDecision = (volDecision<minThinRemove)?minThinRemove:volDecision;
    volDecision = ((stand->Volume.back())-volDecision<minLeaveVol)?((stand->Volume.back())-minLeaveVol):volDecision; //must leave at least minLeaveVol

    //return
    return volDecision;
    
}