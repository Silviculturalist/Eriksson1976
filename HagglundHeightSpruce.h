#pragma once

#include <math.h>
#include <iostream>

double HagglundHeightSpruceSouth(double heightm, double age, double age2) 
{
  double A2;
  double AI1 = 10;
  double AI2 = 600;
  double AI3;
  double H2;
  double H1=(heightm*10)-13;
  double RM;
  double RM2;
  double RK;
  double DIF;
  double T26;
  double T13;


  while(abs(AI1-AI2)>1){

    AI3 = (AI1+AI2)/2;
    RK = 0.042624-7.1145/pow(AI3,1.0068);
    A2 = 1.0017*pow(AI3,0.99808);
    RM = 0.15933+3.7*pow(10,6)/pow(AI3,3.156);

    if(RM>0.95)
    {
      RM = 0.95;
    }

    RM2  =  0.98822/(1-RM);

    if(RK<0.0001)
    {
      RK = 0.0001;
    }

    DIF = H1-A2*pow((1-exp((-age*RK))),RM2);

    AI2 = (DIF<=0)?AI3:AI2;
    AI1 = (DIF>0)?AI3:AI1;

    T26 = (-1/RK)*log(
      1-pow((13/A2),(1/RM2))
    );

    T13 = 4.9546+0.63934*T26+0.031992*T26*T26;
    
  }
/*
  if(A2>400) std::cout << "Too high productivity, outside of the material." << std::endl;
  if(A2<250) std::cout << "Too low productivity, outside of the material." << std::endl;
  if(A2>375 && H1>267) std::cout << "Too old stand, outside of the material." << std::endl;
  if(age>90) std::cout << "Too old stand, outside of the material." << std::endl;
 */
  H2  = (13+A2*pow((1-exp((-age2*RK))),RM2))/10 ;

  return H2;
}

double HagglundTimeToBreastHeightSpruceSouth(double H100) // H100 Total Age! 
{
  double A2;
  double AI1 = 10;
  double AI2 = 600;
  double AI3;
  double H1=(H100*10)-13;
  double RM;
  double RM2;
  double RK;
  double DIF;
  double T26;
  double T13;


  while(abs(AI1-AI2)>1){

    AI3 = (AI1+AI2)/2;
    RK = 0.042624-7.1145/pow(AI3,1.0068);
    A2 = 1.0017*pow(AI3,0.99808);
    RM = 0.15933+3.7*pow(10,6)/pow(AI3,3.156);

    if(RM>0.95)
    {
      RM = 0.95;
    }

    RM2  =  0.98822/(1-RM);

    if(RK<0.0001)
    {
      RK = 0.0001;
    }

    DIF = H1-A2*pow((1-exp((-100*RK))),RM2);

    AI2 = (DIF<=0)?AI3:AI2;
    AI1 = (DIF>0)?AI3:AI1;

    T26 = (-1/RK)*log(
      1-pow((13/A2),(1/RM2))
    );

    T13 = 4.9546+0.63934*T26+0.031992*T26*T26;
    
  }

  return T13;
}


//Calculates the expected dominant height of a Norway Spruce stand in northern Sweden. Culture is true if the stand was planted. From B. Hägglund 1972.
double HagglundHeightSpruceNorth(double heightm, double age, double age2, double latitude, bool culture) 
{
  int cultureint = (culture==true)?1:0;
  double P = pow(0.9175,cultureint);
  double B;
  double C;
  double D;
  double E;
  double A2;
  double AI1 = 10;
  double AI2 = 600;
  double AI3;
  double H2;
  double H1=(heightm*10)-13;
  double RM;
  double RM2;
  double RK;
  double DIF;
  double T26;
  double T13;

  if(latitude>=67 || latitude<=60)
  {
    //std::cout << "Warning - outside of latitudinal range, 60° <= L <= 67° N, using f. 8.4" << std::endl;
    B =  3.4501;
    C = 0.77518;
    D = -0.42579;
    E = 1.33935;

  } else {
    B = 3.3816;
    C = 0.77896;
    D = -1.24207 + 0.0014629*latitude*10;
    E = 1.25998;
  }

  while(abs(AI1-AI2)>1)
  {
    AI3 = (AI1+AI2)/2;

    RK = 0.001936+0.00004100*pow(AI3,1.0105);

    A2 = B*pow(AI3,C);

    RM2 = D+E/(0.56721+0.000008*pow(AI3,1.8008));

    DIF = H1-A2*pow((1-exp((-age*RK))),RM2);

    AI2 = (DIF<=0)?AI3:AI2;
    AI1 = (DIF>0)?AI3:AI1;

  }
/*
  if(A2>336) std::cout << "Warning - too high productivity, outside of the material" << std::endl;
  if(A2<189) std::cout << "Warning - too low productivity, outside of the material" << std::endl;
*/
  T26 = (-1/RK)*log((1-pow((13/A2),(1/RM2))));
  T13 = P*(7.0287+0.66118*T26);

  H2 = (13+A2*pow((1-exp(-age2*RK)),RM2))/10;

  return H2;
}

double HagglundTimeToBreastHeightSpruceNorth(double H100, double latitude, bool culture)
{
  int cultureint = (culture==true)?1:0;
  double P = pow(0.9175,cultureint);
  double B;
  double C;
  double D;
  double E;
  double A2;
  double AI1 = 10;
  double AI2 = 600;
  double AI3;
  double H2;
  double H1=(H100*10)-13;
  double RM;
  double RM2;
  double RK;
  double DIF;
  double T26;
  double T13;

  if(latitude>=67 || latitude<=60)
  {
   // std::cout << "Warning - outside of latitudinal range, 60° <= L <= 67° N, using f. 8.4" << std::endl;
    B =  3.4501;
    C = 0.77518;
    D = -0.42579;
    E = 1.33935;

  } else {
    B = 3.3816;
    C = 0.77896;
    D = -1.24207 + 0.0014629*latitude*10;
    E = 1.25998;
  }

  while(abs(AI1-AI2)>1)
  {
    AI3 = (AI1+AI2)/2;

    RK = 0.001936+0.00004100*pow(AI3,1.0105);

    A2 = B*pow(AI3,C);

    RM2 = D+E/(0.56721+0.000008*pow(AI3,1.8008));

    DIF = H1-A2*pow((1-exp((-100*RK))),RM2);

    AI2 = (DIF<=0)?AI3:AI2;
    AI1 = (DIF>0)?AI3:AI1;

  }

  T26 = (-1/RK)*log((1-pow((13/A2),(1/RM2))));
  T13 = P*(7.0287+0.66118*T26);

  return T13;
}


//Always assumes planted. Decides on function with cutoff at lat 60°.
//This is not used to find time to breast height (1.3m). If targetHeight is 1.31m or less, return is age = 1.
double BHAgeFinder(double h100, double latitude, double targetHeight){ 
    double AgeEstimate{1};
    double currentHeight{1.31}; //try 1.3 m (breast height).
    int HeightTest{1};

    if(latitude>60){ // Northern Sweden
        do
        {
            try
            {
                currentHeight= HagglundHeightSpruceNorth(h100,(100-HagglundTimeToBreastHeightSpruceNorth(h100,latitude,1)),AgeEstimate,latitude,1);
                throw(1);
            }
            catch(int errorNum){
                // Nothing to be done.
            }
            
            HeightTest = (targetHeight > currentHeight)?1:0;
            AgeEstimate+=0.1;
        } while (HeightTest==1);
        
    } else { //Southern Sweden.
        do
        {
            try
            {
                currentHeight = HagglundHeightSpruceSouth(h100,(100-HagglundTimeToBreastHeightSpruceSouth(h100)),AgeEstimate);
                throw(1);
            }
            catch(int errorNum){
                //Nothing to be done.
            }
            
            HeightTest = (targetHeight > currentHeight)?1:0;
            AgeEstimate+=0.1;
        } while (HeightTest==1);

    }

    return AgeEstimate-0.1; //while loop will run one too many times.
}



/*
//The following can be copy-pasted into own file for testing.

#include "HagglundHeightSpruce.h"

int main()
{

  std::cout << "South G8 : " <<  HagglundTimeToBreastHeightSpruceSouth(8) << "\n"
            << "South G16 : " <<  HagglundTimeToBreastHeightSpruceSouth(16) << "\n"
            << "North G8, Lat 61, Planted : " <<  HagglundTimeToBreastHeightSpruceNorth(8,61,1) << "\n"
            << "North G16, Lat 61, Planted : " <<  HagglundTimeToBreastHeightSpruceNorth(16,61,1) << "\n"
            << std::endl;

  std::cout << "South G8, 100 yrs Total: " <<  HagglundHeightSpruceSouth(8,100-HagglundTimeToBreastHeightSpruceSouth(8),100-HagglundTimeToBreastHeightSpruceSouth(8)) << "\n"
            << "South G16, 100 yrs Total: " <<  HagglundHeightSpruceSouth(16,100-HagglundTimeToBreastHeightSpruceSouth(16),100-HagglundTimeToBreastHeightSpruceSouth(16)) << "\n"
            << "North G8, Lat 61, Planted, 100 yrs Total: " <<  HagglundHeightSpruceNorth(8,100-HagglundTimeToBreastHeightSpruceNorth(8,61,1),100-HagglundTimeToBreastHeightSpruceNorth(8,61,1),61,1) << "\n"
            << "North G16, Lat 61, Planted, 100 yrs Total: " <<  HagglundHeightSpruceNorth(16,100-HagglundTimeToBreastHeightSpruceNorth(16,61,1),100-HagglundTimeToBreastHeightSpruceNorth(16,61,1),61,1) << "\n"
            << std::endl;

  std::cout << "Lat 57, G32, -1 m at ageBH = " << TotalAgeFinder(32,57,-1) << "\n"
            << "Lat 61, G16, 10m at ageBH = " << TotalAgeFinder(16,61,10) << "\n"
            << std::endl;

    return 0 ;

  return 0;
}

*/