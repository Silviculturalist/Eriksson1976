#include <math.h> //math capabilities. cmath instead?
#include <iostream> //Input output functionality
#include "HagglundHeightSpruce.h" //For height functions.
#include "ErikssonStemDistributions.h" //For stem distribution outputs.
#include "Stand.h" //Class functionality.
#include <vector> //For std::vector
#include <limits> //For min max vals
#include <string> //Forr stod (string to double)

//main always returns 0 if everything OK.
int main()
{
  StandObject* erikssonStand = new StandObject(32,10,57,3500,true); 
    
  return 0;
}

