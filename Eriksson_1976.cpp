/*
#include <math.h>
#include <iostream>
#include "HagglundHeightSpruce.h" //For height functions.
#include "ErikssonStemDistributions.h" //For stem distribution outputs.
#include <vector>

//Background function

//Get an initial BA from stems and height. f. 7.2.1, p. 99.
double initialBA(double dominantheight, double stems, bool plantedbool=true) 
{
  double b1{0};
  
  if(plantedbool == true){
    b1 = 0.355;
  } else {
    b1 = 0.319;
  }

  return 1.0111 * pow((dominantheight - 1.3),1.230) * pow((stems/1000),b1);
}


//Define the stand class.



class ErikssonStand{
    public:
        double H100{0};
        double DominantHeight{0};
        double Latitude{0};
        int Age{0};
        int AgeBH{0};
        double BasalAreaM2{0};
        double BasalAreaUnderBarkM2{0};
        double BarkProcentAddition{0};
        double BarkProcentRemoval{0};
        double FormHeight{0};
        double FormHeightThinned{0};
        double BAI{0};
        int StemsStart{0};
        int StemsThinned{0};
        int StemsSelfThinned{0};
        int StemsEnd{0};
        double QmdCm{0};
        double QmdUnderBarkCm{0};
        double QmdThinnedCm{0};
        double QmdUnderBarkThinnedCm{0};
        double QmdSelfThinnedCm{0};
        double Volume{0}; //Volume of stand, m3sk. (cubic metres.)
        int Period{0}; //Period counter.
        int IncrementYears{5}; // Years to increment by.
        double ThinnedBA{0}; //Thinned Basal Area.
        double ThinnedVOL{0}; // Thinned Volume during the period.
        double ThinningPercent{0}; //Thinning during the period as percent of the BA under bark.
        double SelfThinnedBA{0}; //The expected BA to be self-thinned from mortality.
        double ThinningForm{0}; // QMD removed / QMD stand before thinning.
        double GroupStructureIndex{0}; //Coefficient of variation for the BA under bark on 5 meter circular plots inH100de the sample plot.. in percent.
        int Plantedbool{1}; // 1 if planted, 0 if natural regeneration.
        double MAI_VOL{0}; //Mean Annual Increment - Volume.
        double MAI_BA{0}; // ... - Basal Area.
        double CAI_VOL{0}; // Current Annual Increment - Volume.
        double CAI_BA{0}; // ... - Basal Area.
        
        //Constructors
        ErikssonStand(double h100, double startHeight, double latitude, int stems, int increment=5){
          IncrementYears = increment;
          H100 = h100;
          Latitude= latitude;
          AgeBH  = (int) (BHAgeFinder(h100,latitude,startHeight)+0.5); //round to integer.
          //Get total age by adding time to breast height, here with ternary operator to distinguish north and south sweden.
          Age = (int) (((latitude>60)?(AgeBH + HagglundTimeToBreastHeightSpruceNorth(h100,latitude,1)):(AgeBH + HagglundTimeToBreastHeightSpruceSouth(h100)))+0.5);
          StemsStart = stems;

          //Set the starting height which fits truncated Ages.
          if(Latitude>60){ //Norrland and Kopparbergs County (pre-1998 county reorganisation)
            DominantHeight = HagglundHeightSpruceNorth(
              H100,
              (100-HagglundTimeToBreastHeightSpruceNorth(H100,Latitude,Plantedbool)),
              (AgeBH),
              Latitude,
              Plantedbool
              );
          } else { //Southern Sweden.
            DominantHeight = HagglundHeightSpruceSouth(
              H100,
              (100-HagglundTimeToBreastHeightSpruceSouth(H100)),
              (AgeBH)
            );
          }

          //Fetch the initial basal area.
          BasalAreaM2 = initialBA(DominantHeight,StemsStart,1); // Always planted.

          //Calculate QMD (Area of the mean basal area stem.), in cm.
          QmdCm = 2* sqrt(((BasalAreaM2/StemsStart)*10000)/3.14159265359);

          //Form height
          this->formheight();

          //Volume of the stand.
          Volume = BasalAreaM2*FormHeight;

          //Mean Annual Increments: 
          MAI_VOL = Volume/Age;
          MAI_BA = BasalAreaM2/Age;
                    
          //Bark procent removal
          this->barkprocentremoval();

          //Basal area under bark.
          BasalAreaUnderBarkM2 = BasalAreaM2-((BasalAreaM2*BarkProcentRemoval)/100);

          //QMD under bark, in cm.
          QmdUnderBarkCm = 2* sqrt(((BasalAreaUnderBarkM2/StemsStart)*10000)/3.14159265359);
                    
          //First period always no thinning.
          ThinningPercent=0;

          //Annual basal area increment
          this->bai();

          this->barkprocentaddition();

          //BAI over bark 
          CAI_BA = ((BarkProcentAddition*BAI)/100 + BAI);
        };
        
        void barkprocentaddition(); // Bark percent addition to BA.
        void barkprocentremoval(); //Bark percent removal to BA.
        void selfthinning(); // Mortality from self-thinning.
        void bai(); // Basal Area Increment during the period. Not suitable for stands with a dominant height < 7 m.
        void formheight(); //The Form-Height of a stand.
        void formheightthinned(); //The Form-height of the thinned stems.
        void setHeight(); // Dominant height at age + increment.
        void report(); //Simple terminal output.
        void period(double thinningpercent=0, double thinningform=0); //Updating function.
        //void thinningGuide(); // A class holding thinning instructions to an ErikssonStand


};

//ThinningGuide
class thinningGuide:ErikssonStand{
  public:
    int thinAgeMin; //Minimum thinning age
    int thinAgeMax; //Maximum thinning age
    double thinHeightMin; //Minimum dominant height for thinning
    double thinHeightMax; // Maximum dominant height for thinning
    double thinMinBA; //Minimum required basal area.
    double minRemovalBA; //Minimum required removal (Basal Area).
    double minRemovalVol; //Minimum required removal (volume cu.m.)
    double FellingMinAge; //Minimum felling age allowed.
    double FellingMaxAge; //Maximum felling age allowed.

  void queueThinning(double thinQuotient, double thinStrength); //Queue a thinning at first possible time.
  void queueThinningAge(double thinQuotient, double thinStrength, double ageBH); //Queue a thinning at age.
  void queueThinningHeight(double thinQuotient, double thinStrength, double atHeight); //Queue a thinning at height.
  void queueThinningRule(); //Thin if evaluates to true.
  void queueRecurringThinning(double thinQuotient, double thinStrength, double intervalYears); //Recurring thinning.
  void queueFellingRule(); //Felling allowed if evaluates to true.

  private:
    std::vector<double> thinningQueue;

}


//Specific functions.

void ErikssonStand::period(double thinningpercent, double thinningform){
  //Update age.
  Age+=IncrementYears;
  AgeBH+=IncrementYears;

  //update Dominant height
  this->setHeight();

  //Update basal area under bark at starting time.
  BasalAreaUnderBarkM2+=BAI;

  //Update bark procent addition.
  this->barkprocentaddition();

  //Update BasalAreaM2 at starting time.
  BasalAreaM2=BasalAreaUnderBarkM2 + (BasalAreaUnderBarkM2*BarkProcentAddition)/100;

  //Update QMDcm
  QmdUnderBarkCm = 2* sqrt(((BasalAreaUnderBarkM2/StemsStart)*10000)/3.14159265359);

  QmdCm = 2* sqrt(((BasalAreaM2/StemsStart)*10000)/3.14159265359);                


  //Update form height
  this->formheight();


  //Calculate initial volume
  Volume = BasalAreaM2*FormHeight;

  //Mean Annual Increments: 
  MAI_VOL = Volume/Age;
  MAI_BA = BasalAreaM2/Age;
  
  //Apply thinnings.
  ThinningPercent = thinningpercent;

  //If thinningform == 0, calculate thinningform.
  ThinningForm = (thinningform==0)? ((DominantHeight>284)?1:(0.768 + 0.00082*DominantHeight)):thinningform;
  
  //Calculate the QMD of the thinned stems.
  QmdThinnedCm = ThinningForm*QmdCm; //Over bark.
  QmdUnderBarkThinnedCm = ThinningForm*QmdUnderBarkCm; //Under bark.

  //Closest number of removable stems. 
  //NB truncation to integer.!
  //NB using QMD of thinned stems to get mean basal area stem which is thinned. 
  StemsThinned = (int) (thinningpercent*BasalAreaUnderBarkM2)/(3.14159265359*pow((QmdUnderBarkThinnedCm/2),2));

  //The Actual Thinned BA.
  ThinnedBA = StemsThinned*(3.14159265359*pow((QmdSelfThinnedCm/2),2));

  //Form height of thinned?
  this->formheightthinned();

  //Get Thinned volume..
  ThinnedVOL = ThinnedBA*FormHeightThinned;

  //Self-thinning for period.
  this->selfthinning();


  //Get the expected basal area increment.
  this->bai();

}





void ErikssonStand::report()
{
  std::cout << "Spruce stand G" << H100 << "Latitude " << Latitude << "\n"
            << "N = " << StemsStart << "\n"
            << "Total Age = " << Age << "\n"
            << "Age BH = "<< AgeBH << "\n"
            << "Dominant Height = "<< DominantHeight << "\n"
            << "BA = " << BasalAreaM2 << "\n"
            << "BAI = " << BAI << "\n"
            << "CAI BA = " << CAI_BA << "\n"
            << "Form Height = " << FormHeight << "\n"
            << "Volume m3sk = " << Volume << "\n"
            << "CAI VOL = " << CAI_VOL << "\n"
            << "QMDcm = " << QmdCm << "\n"
            << "QMD UB cm = " << QmdUnderBarkCm <<  "\n" 
            << "MAI VOL = " << MAI_VOL << "\n"
            << "MAI BA = " << MAI_BA << "\n"
            << "QMD SELF THIN = " << QmdSelfThinnedCm << std::endl;
}


void ErikssonStand::setHeight()
{
  if(Latitude>60){ //Norrland and Kopparbergs County (pre-1998 county reorganisation)
    DominantHeight = HagglundHeightSpruceNorth(
      H100,
      (100-HagglundTimeToBreastHeightSpruceNorth(H100,Latitude,Plantedbool)),
      (Age-HagglundTimeToBreastHeightSpruceNorth(H100,Latitude,Plantedbool)),
      Latitude,
      Plantedbool
      );
  } else { //Southern Sweden.
    DominantHeight = HagglundHeightSpruceSouth(
      H100,
      (100-HagglundTimeToBreastHeightSpruceSouth(H100)),
      (Age-HagglundTimeToBreastHeightSpruceSouth(H100))
    );
  }

}



void ErikssonStand::selfthinning() 
{
  double b1;
  double b2;
  double StemsStart2;
  
  //Get the QMD of the self-thinned stems (above bark), e.g. f. 8.1.1., p. 107.
  QmdSelfThinnedCm = (0.039* pow((DominantHeight * 10),0.517)) * QmdCm;
  
    //Modification required if StemsStart exceed 4500 and DominantHeight at same time is more than 15m.
    if(DominantHeight>15 && StemsStart>4500){
        StemsStart2 = StemsStart + 0.1*(StemsStart-4500);
    } else {
        StemsStart2 = StemsStart;
    }

    if(H100<=17.9){
    //G16
    b1 = 0.524;
    b2 = 3.505;

  } else if(H100<=21.9){
    //G20
    b1 = 1.858;
    b2 = 3.376;

  } else if(H100<=25.9){
    //G24
    b1 = 2.613;
    b2 = 3.356;

  } else if(H100<=29.9){
    //G28
    b1 = 2.872;
    b2 = 3.347;

  } else if(H100<=33.9){
    //G32
    b1 = 3.229;
    b2 = 3.458;

  } else if(H100>=34){
    //G36
    b1 = 2.646;
    b2 = 3.479;
  }
  
  SelfThinnedBA = 3.25 * (pow(10,-10)) * (pow((StemsStart2/1000),b1)) * (pow((DominantHeight*10),b2)) ;



  StemsSelfThinned = (SelfThinnedBA * 40000)/(pow(QmdSelfThinnedCm,2) * 3.14159265359);

  
}



void ErikssonStand::formheight()
{
  double b1;

  if(H100<=17.9){
    //G16
    b1 = 0.835;

  } else if(H100<=21.9){
    //G20
    b1 = 0.833; 

  } else if(H100<=25.9){
    //G24
    b1 = 0.838;

  } else if(H100<=29.9){
    //G28
    b1 = 0.838;

  } else if(H100<=33.9){
    //G32
    b1 =  0.839;

  } else if(H100>=34){
  //G36
  b1 = 0.844;
  }


  FormHeight = pow(10,-1.141) * pow((DominantHeight*10),b1) * pow(QmdCm,0.123);
}

void ErikssonStand::formheightthinned()
{
  double b1;

  if(H100<=17.9){
    //G16
    b1 = 0.835;

  } else if(H100<=21.9){
    //G20
    b1 = 0.833; 

  } else if(H100<=25.9){
    //G24
    b1 = 0.838;

  } else if(H100<=29.9){
    //G28
    b1 = 0.838;

  } else if(H100<=33.9){
    //G32
    b1 =  0.839;

  } else if(H100>=34){
  //G36
  b1 = 0.844;
  }


  FormHeightThinned = pow(10,-1.141) * pow((DominantHeight*10),b1) * pow(QmdThinnedCm,0.123);
}



void ErikssonStand::bai() 
{
    double b1;
    double b2;
    double b3;
    double b4;
    double b5;
    double b6;
    double c1;
    

    if(H100<=17.9){
    //G16
    b1 = 0.250;
    b2 = -0.046;
    b3 = 0.049;
    b4 = 0.033;
    b5 = -0.748;
    b6 = -0.308;
    c1 = 0.447;
  } else if(H100<=21.9){
    //G20
    b1 = 0.213;
    b2 = -0.055;
    b3 = 0.085;
    b4 = 0.460;
    b5 = -0.875;
    b6 = -0.313;
    c1 = 0.488;
  } else if(H100<=25.9){
    //G24
    b1 = 0.304;
    b2 = -0.056;
    b3 = 0.126;
    b4 = 0.035;
    b5 = -0.571;
    b6 = -0.154;
    c1 = 0.498;
  } else if(H100<=29.9){
    //G28
    b1 = 0.260 ;
    b2 = -0.059;
    b3 = 0.034;
    b4 = 0.112;
    b5 = -0.667;
    b6 = -0.066;
    c1 = 0.568;
  } else if(H100>=30){
    //G32
    b1 =  0.337;
    b2 = -0.046;
    b3 = 0.056;
    b4 = 0.120;
    b5 = -0.789;
    b6 = -0.013;
    c1 = 0.763;
  }

  double VG = 13.778 * pow((AgeBH/10),c1) * pow((StemsStart/1000),-0.052) ; //Group structure index (p. 112), coef. variation for basal area under bark on 5 meter circular plots inside the sample plots.


    BAI = 2.635 * pow(BasalAreaUnderBarkM2,b1) * pow((ThinningPercent+0.01),b2) * pow(
        ((QmdUnderBarkThinnedCm/(QmdUnderBarkCm)+0.1) * ((100*StemsThinned)/(StemsStart)+0.01)),0.024 //Note 0.024 is corrected from typo p. 63.
    ) * pow(IncrementYears,b3) * pow((DominantHeight*10),b4) * pow(AgeBH,b5) * pow(VG,b6)  ;
}


void ErikssonStand::barkprocentaddition() //Bark addition percent for basal area under bark at breast height.
{

double b1{0};

if(H100 <= 17.9){
    //G16
    b1 = -0.224;

  } else if(H100<=21.9){
    //G20
    b1 = -0.247;

  } else if(H100<=25.9){
    //G24
    b1 = -0.249;

  } else if(H100<=29.9){
    //G28
    b1 = -0.266;

  } else if(H100<=33.9){
    //G32
    b1 = -0.255;

  } else if(H100>=34){
    //G36
    b1 = -0.280;
  }

  BarkProcentAddition = 45.08*(pow(QmdUnderBarkCm,b1))*(pow(BasalAreaUnderBarkM2,-0.281))*(pow(AgeBH,0.125));

}

void ErikssonStand::barkprocentremoval() //Bark removal percent for basal area at breast height.
{

double b1;

if(H100<=17.9){
    //G16
    b1 = -0.138;

  } else if(H100<=21.9){
    //G20
    b1 = -0.183;

  } else if(H100<=25.9){
    //G24
    b1 = -0.206;

  } else if(H100<=29.9){
    //G28
    b1 = -0.236;

  } else if(H100<=33.9){ //Note overlap with earlier if statements.
    //G32
    b1 = -0.243;

  } else if(H100>=34){
    //G36
    b1 = -0.243;
  }

    BarkProcentRemoval = 9.50 * (pow(QmdCm,b1)) * (pow(BasalAreaM2,0.135)) * (pow(AgeBH,0.112));
}








//main always returns 0 if everything OK.
int main()
{

  ErikssonStand a = ErikssonStand(32,10,57,3500);
  a.period();
  a.report();

  return 0;
}


*/