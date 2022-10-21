#pragma once 
#include "Stand.h"
#include <vector>
#include <string>
#include <limits>
#include <iomanip>

//Templated class for several types of variables to be watched.
template <class T>
class task
{
    public:
        T* Trigger; //Pointer to variable 
        T Limit; //Limit for variable
        int ThinningType; //1 is absolute, 2 is percent, 3 is remaining amount, 4 is none.
        double ThinningStrength{0}; //of type...
        double Form{0}; //if given, else 0.

        task(T* trigger, T limit, int thinningtype, double strength, double form): Trigger(trigger),Limit(limit),ThinningType(thinningtype),ThinningStrength(strength),Form(form){};
        task(T* trigger, T limit, int thinningtype, double strength): Trigger(trigger),Limit(limit),ThinningType(thinningtype),ThinningStrength(strength){};
        ~task(){};

        //returns true if Trigger equals or has surpassed Limit.
        bool isTriggered(){

            bool triggerbool=false;
    
            triggerbool = (*Trigger>=Limit)?true:false;
    
            return triggerbool;
        };

};

//to print task
template<class T> inline std::ostream& operator<<(std::ostream& out, const task<T>* val)
{
    std::vector<std::string> thinType{"Absolute","Percent","Remainder","No thinning"};
    int thinElem;
    switch(val->ThinningType)
    {
        case 1:
            thinElem = 0;
            break;
        case 2:
            thinElem = 1;
            break;
        case 3:
            thinElem = 2;
            break;
        default:
            thinElem = 3;
            break;
    };

    out << std::fixed << std::setprecision(0)
        << "Trigger : " << *(val->Trigger) << "\n"
        << "Limit : " << val->Limit << "\n" 
        << "Specifications  : " << thinType[thinElem] << "\n"
        << "Strength : " << val->ThinningStrength << "\n"
        << "Form : " << val->Form << "\n"
        << "\n";
    return out;
};


class thinningList
{
    public:
        //two containers with pointers to tasks.
        std::vector<task<double>*> taskListDbl{};
        std::vector<task<int>*> taskListInt{};

        thinningList(){};
        ~thinningList(){};

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

        task<int>* getMatchInt()
        {
            int matchedElement = -1;
            //Default nulltask.
            int zeroVal = 0;
            int* zeroPtr = &zeroVal;
            task<int>* matchedPtr = new task<int>(zeroPtr,std::numeric_limits<int>::max(),4,0);

            //Find possible match.
            for(int i=0;i<taskListInt.size();i++)
            {
                if(taskListInt.at(i)->isTriggered() && matchedElement==-1)
                {
                    matchedPtr = taskListInt.at(i);
                    matchedElement = i;
                }
            }

            return matchedPtr;
        };

        task<double>* getMatchDbl()
        {
            int matchedElement = -1;
            //Default nulltask.
            double zeroVal = 0.0;
            double* zeroPtr = &zeroVal;
            task<double>* matchedPtr = new task<double>(zeroPtr,std::numeric_limits<double>::max(),4,0);

            //Find possible match.
            for(int i=0;i<taskListDbl.size();i++)
            {
                if(taskListDbl.at(i)->isTriggered() && matchedElement==-1)
                {
                    matchedPtr = taskListDbl.at(i);
                    matchedElement = i;
                }
            }

            return matchedPtr;    
        }

    
};