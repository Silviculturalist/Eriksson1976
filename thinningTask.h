#pragma once 
#include "Stand.h"
#include <vector>

//Templated class for several types of variables to be watched.
template <class T>
class task
{
    public:
        T* Trigger; //Pointer to variable 
        T Limit; //Limit for variable
        int ThinningType; //1 is absolute, 2 is percent, 3 is remaining amount.
        double ThinningStrength{0}; //of type...
        double Form{0}; //if given, else 0.

        task(T* trigger, T limit, int thinningtype, double strength, double form): Trigger(trigger),Limit(limit),ThinningType(thinningtype),ThinningStrength(strength),Form(form){};
        task(T* trigger, T limit, int thinningtype, double strength): Trigger(trigger),Limit(limit),ThinningType(thinningtype),ThinningStrength(strength){};
        ~task();

        //returns true if Trigger equals or has surpassed Limit.
        bool isTriggered(){

            bool triggerbool=false;
    
            triggerbool = (*Trigger>=Limit)?true:false;
    
            return triggerbool;
        };

};

class thinningList
{
    public:
        //two containers with pointers to tasks.
        std::vector<task<double>*> taskListDbl{};
        std::vector<task<int>*> taskListInt{};

        thinningList(){};
        ~thinningList(){};

        //Check if any in containers have a match and return position.
        //int if there is a match (else -1)
        int hasMatchInt()
        {
            int matchElement{-1};

            for(int i=0;i<taskListInt.size();i++)
            {
                matchElement = (taskListInt.at(i)->isTriggered())?i:matchElement;
            }

            return matchElement;
        };

        int hasMatchDbl()
        {
            int matchElement{-1};
            
            for(int i=0;i<taskListDbl.size();i++)
            {
                matchElement = (taskListDbl.at(i)->isTriggered())?i:matchElement;
            }

            return matchElement;        
        }



        //add integer task to container.
        void addIntTask(int* intPtr, int intlimit,int thinningtype, double strength, double form)
        {
            task<int>* taskPtr = new task<int>(intPtr, intlimit, thinningtype, strength, form);
            taskListInt.push_back(taskPtr);
        };

        //with default form...
        void addIntTask(int* intPtr, int intlimit,int thinningtype, double strength)
        {
            task<int>* taskPtr = new task<int>(intPtr, intlimit, thinningtype, strength);
            taskListInt.push_back(taskPtr);
        };

        //add double task to container.
        void addDblTask(double* dblPtr, double dblLimit,int thinningtype, double strength, double form)
        {
            task<double>* taskPtr = new task<double>(dblPtr,dblLimit, thinningtype, strength, form);
            taskListDbl.push_back(taskPtr);
        };

        //with default form...
        void addDblTask(double* dblPtr, double dblLimit,int thinningtype, double strength)
        {
            task<double>* taskPtr = new task<double>(dblPtr,dblLimit, thinningtype, strength);
            taskListDbl.push_back(taskPtr);
        };
    
};