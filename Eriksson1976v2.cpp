#include "ErikssonStemDistributions.h" //For stem distribution outputs.
#include "Stand.h" //Class functionality.


//main always returns 0 if everything OK.
int main()
{
  StandObject* erikssonStand = new StandObject(32,10,57,3500,true);
  erikssonStand->result();
    
  return 0;
}

