#pragma once
#include <limits>
#include "Stand.h"
#include "thinningTask.h"

//Definition for a ThinningManager that controls thinning for a StandObject.
class ThinningManager {
    public:
        StandObject* stand; //ptr to the associated StandObject.
        double minThinInitBA{0}; //0 for nonlimiting.
        double maxThinInitBA{std::numeric_limits<double>::max()}; //max for nonlimiting.
        int thinInterval{0}; //zero for no interval
        double minThinRemove{0}; //smallest possible value for nonlimiting.
        double minLeaveBA{0}; //0 for not limiting, eg full harvest.
        int minThinAge{0}; //-1 for not limiting.. lowest acceptable age for thinning.
        int maxThinAge{std::numeric_limits<int>::max()};
        int firstThinYear{0}; //Counter for when first thinning was held.
        int thinCounter{0}; //to be able to set firstThinYear and report total thinnings.
        double repeatAbs{0}; //repeat thinning absolute value
        double repeatPerc{0}; //repeat thinning percentual value.
        std::vector<double> ThinBAArray = {0}; //only contains 1 element for start. A list of volumes to be removed at each period.
        std::vector<double> ThinBAAbsoluteArray = {0}; //to store absolute thinnings..
        std::vector<double> ThinBAPercentArray = {0}; //to store a fix percentage thinning.
        std::vector<double> ThinBARemainingArray = {-1}; //negative numbers if nil to avoid clear-felling.

        //Constructor and Destructor
        ThinningManager(StandObject* stand): stand(stand){}; //Takes a stand
        ~ThinningManager(){};

        double volumeDecision(); //Calculate volume to be removed.
        bool thinningOK(); //Check if thinning is allowed.
        void setBAAbsoluteThinningAgeBH(int agebh, double amount); //set a thinning by removing a fix amount.
        void setBAPercentThinningAgeBH(int agebh, double percent); //set a thinning with percent to remove.
        void setBARemainingThinningAgeBH(int agebh, double baRemaining); //set a thinning with remaining amount of basal area.

        void setMinThinAge(int age)
        {
            minThinAge = (age>0)?age:0;
        }

        void setRepeatAbs(double cut)
        {
            cut = (cut<0)?0:cut;//truncate to zero if less than zero.
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
            interval = (interval<0)?0:interval;
            thinInterval = interval;
        };

        void processThinning()
        { //process a thinning. calculate and conduct.

            double ba = volumeDecision();
            stand->cutAbsoluteDefault(ba);
        };
};


bool ThinningManager::thinningOK()
{
    bool thinningVal{false};

    thinningVal = 
        (
            (stand->BasalAreaM2.back()) >= minThinRemove && //at least minimum level of harvest exists.
            (stand->BasalAreaM2.back()) >= minThinInitBA && //at least initial volume of harvest exists.
            (stand->BasalAreaM2.back()) <= maxThinInitBA && //at most maximum volume at which thinning is allowed.
            (stand->Age.back()) >= minThinAge && //at least minimum thinning age.
            (stand->Age.back()) <= maxThinAge //at most maximum thinning age.
        )?true:false; //stock is smaller than the largest thinning is allowed at.
    
    return thinningVal;
}

double ThinningManager::volumeDecision()
{
    double baDecision{0}; //Default harvest nothing.
    bool decisionTree{false}; //has another setting taken a decision?

    //Can thinning be conducted?
    if(thinningOK())
    {
        //Scheduled harvesting takes priority.
        if((ThinBAArray[0]>0 && ThinBAArray.size()>stand->Volume.size()) | (ThinBAArray.size()>1 && ThinBAArray.size()>stand->Volume.size())) //If list of volumes to harvest is greater than default 0.. (limit zero)
        {
            if (ThinBAArray.at(stand->Volume.size())>0) //check if any stated value at latest year. 
            {
                baDecision = ThinBAArray.at(stand->Volume.size());
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
                    baDecision = std::max(repeatAbs,((repeatPerc/100)*(stand->Volume.back()))); //largest of repeatAbs or repeatPerc
                    decisionTree = true;
                    thinCounter++; //increment number of thinnings.

                }
            }

            //If first thinning has not been done.
            else if(thinCounter==0)
            {
                    baDecision = std::max(repeatAbs,((repeatPerc/100)*(stand->Volume.back()))); //largest of repeatAbs or repeatPerc
                    decisionTree = true;
                    firstThinYear = int(stand->Volume.size()); //set age at first thinning.
                    thinCounter++; //first increment of thinCounter.

            }

        }
    }

    //Check volDecision logic.
    baDecision = (baDecision<minThinRemove)?minThinRemove:baDecision;
    baDecision = ((stand->BasalAreaM2.back())-baDecision<minLeaveBA)?((stand->BasalAreaM2.back())-minLeaveBA):baDecision; //must leave at least minLeaveVol

    //return
    return baDecision;
    
}