#include <math.h>
#include <iostream>
#include "HagglundHeightSpruce.h" //For height functions.

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
        double BAI{0};
        int StemsStart{0};
        int StemsThinned{0};
        int StemsSelfThinned{0};
        int StemsEnd{0};
        double QmdCm{0};
        double QmdUnderBarkCm{0};
        double QmdUnderBarkThinnedCm{0};
        double QmdSelfThinnedCm{0};
        double Volume{0};
        double IV{0};
        double DryWeightIncrementTons{0};
        int Period{0};
        int IncrementYears{5};
        double TotalBAThinned{0};
        double ThinnedBAPeriodX{0};
        double ThinningPercentPeriodXUnderBark{0};
        double SelfThinnedBAPeriod{0};
        double ThinnedBALastPeriod{0};
        double GroupStructureIndex{0}; //Coefficient of variation for the BA under bark on 5 meter circular plots inH100de the sample plot.. in percent.
        int plantedbool{1}; // 1 if planted, 0 if natural regeneration.
        
        void barkprocentaddition(); // Bark percent addition to BA.
        void barkprocentremoval(); //Bark percent removal to BA.
        void selfthinnedba(); // Mortality from self-thinning.
        void iv(); //Volume increment during the period.
        void bai(); // Basal Area Increment during the period.
        void dryweightincrement(); //Increment in Dry Weight (tons per hectare).
        void formheight(); //The Form-Height of a stand.
        void selfthinneddiameter(); //Calculate the diameter of the self-thinned stems.
        void stemsselfthinned(); //Calculate the number of self-thinned stems.
        void setHeight(); // Dominant height at age + increment.



        





};

//Specific functions.


void ErikssonStand::setHeight()
{
  if(Latitude>60){ //Norrland and Kopparbergs County (pre-1998 county reorganisation)
    DominantHeight = HagglundHeightSpruceNorth(
      H100,
      (100-HagglundTimeToBreastHeightSpruceNorth(H100,Latitude,plantedbool)),
      (Age-HagglundTimeToBreastHeightSpruceNorth(H100,Latitude,plantedbool)),
      Latitude,
      plantedbool
      );
  } else { //Southern Sweden.
    DominantHeight = HagglundHeightSpruceSouth(
      H100,
      (100-HagglundTimeToBreastHeightSpruceSouth(H100)),
      (Age-HagglundTimeToBreastHeightSpruceSouth(H100))
    );
  }

}



void ErikssonStand::stemsselfthinned() 
{
  StemsSelfThinned = (SelfThinnedBAPeriod * 40000)/(pow(QmdSelfThinnedCm,2) * 3.14159265359);
}



void ErikssonStand::selfthinneddiameter()
{
  QmdSelfThinnedCm = (0.039* pow((DominantHeight * 10),0.517)) * QmdCm;
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


void ErikssonStand::dryweightincrement()
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
    b1 = 0.320;
    b2 = -0.131;
    b3 = 0.065;
    b4 = 0.991;
    b5 = -0.842;
    b6 = -0.295;
    c1 = 0.447;
  } else if(H100<=21.9){
    //G20
    b1 = 0.455;
    b2 = -0.137;
    b3 = 0.064;
    b4 = 0.900;
    b5 = -0.866;
    b6 = -0.255;
    c1 = 0.488;
  } else if(H100<=25.9){
    //G24
    b1 = 0.523;
    b2 = -0.138;
    b3 = 0.092;
    b4 = 0.765;
    b5 = -0.702;
    b6 = -0.194;
    c1 = 0.498;
  } else if(H100<=29.9){
    //G28
    b1 = 0.399;
    b2 = -0.136;
    b3 = 0.012;
    b4 = 0.821;
    b5 = -0.786;
    b6 = -0.131;
    c1 = 0.568;
  } else if(H100>=30){
    //G32
    b1 =  0.442;
    b2 = -0.125;
    b3 = 0.021;
    b4 = 0.770;
    b5 = -0.814;
    b6 = -0.065;
    c1 = 0.763;
  }

    double VG = 13.778 * pow((Age/10),c1) * pow((StemsStart/1000),-0.052); //Group structure index (p. 112), coef. variation for basal area under bark on 5 meter circular plots inside the sample plots.


    DryWeightIncrementTons = 0.072 * pow(BasalAreaUnderBarkM2,b1) * pow((ThinningPercentPeriodXUnderBark+0.01),b2) * pow((QmdUnderBarkThinnedCm/(QmdUnderBarkCm+0.1)) * ((100*StemsThinned)/(StemsStart+0.01)),0.081) * pow(IncrementYears,b3) * pow((DominantHeight*10),b4) * pow(Age,b5) * pow(VG,b6);
};

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

  double VG = 13.778 * pow((Age/10),c1) * pow((StemsStart/1000),-0.052) ; //Group structure index (p. 112), coef. variation for basal area under bark on 5 meter circular plots inside the sample plots.


    BAI = 2.635 * pow(BasalAreaUnderBarkM2,b1) * pow((ThinningPercentPeriodXUnderBark+0.01),b2) * pow(
        (QmdUnderBarkThinnedCm/(QmdUnderBarkCm+0.1)) * ((100*StemsThinned)/(StemsStart+0.01)),0.024 //Note 0.024 is corrected from typo p. 63.
    ) * pow(IncrementYears,b3) * pow((DominantHeight*10),b4) * pow(Age,b5) * pow(VG,b6)  ;
}







void ErikssonStand::iv() // Calculate Annual volume increment under bark per hectare m^3.
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
    b1 = 0.253;
    b2 = -0.100;
    b3 = 0.067;
    b4 = 1.103;
    b5 = -0.906;
    b6 = -0.296;
    c1 = 0.447;
  } else if(H100<=21.9){
    //G20
    b1 = 0.370;
    b2 = -0.107;
    b3 = 0.080;
    b4 = 1.015;
    b5 = -0.939;
    b6 = -0.233;
    c1 = 0.488;
  } else if(H100<=25.9){
    //G24
    b1 = 0.435;
    b2 = -0.109;
    b3 = 0.109;
    b4 = 0.831;
    b5 = -0.730;
    b6 = -0.125;
    c1 = 0.498;
  } else if(H100<=29.9){
    //G28
    b1 = 0.322;
    b2 = -0.108;
    b3 = 0.042;
    b4 = 0.907;
    b5 = -0.795;
    b6 = -0.109;
    c1 = 0.568;
  } else if(H100>=30){
    //G32
    b1 =  0.358;
    b2 = -0.097;
    b3 = 0.035;
    b4 = 0.868;
    b5 = -0.861;
    b6 = -0.042;
    c1 = 0.763;
  }




    double VG = 13.778 * pow((Age/10),c1) * pow((StemsStart/1000),-0.052) ; //Group structure index (p. 112), coef. variation for basal area under bark on 5 meter circular plots inside the sample plots.

    IV = 0.141 * (pow(BasalAreaUnderBarkM2,b1)) * (pow((ThinningPercentPeriodXUnderBark-0.01),b2)) * pow(((QmdUnderBarkThinnedCm / (QmdUnderBarkCm + 0.1))*((100*StemsThinned)/(StemsStart+0.01))),0.062) * pow((IncrementYears/10),b3) * pow((DominantHeight*10),b4) * pow((Age/10),b5) * pow(VG,b6) ;



}

void ErikssonStand::selfthinnedba() //Calculate annual self-thinning (mortality) in basal area per hectare, m^2. 
{
    double b1;
    double b2;
    double StemsStart2;

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
    SelfThinnedBAPeriod = 3.25 * (pow(10,-10)) * (pow((StemsStart2/1000),b1)) * (pow((DominantHeight*10),b2)) ;
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

  BarkProcentAddition = 45.08*(pow(QmdUnderBarkCm,b1))*(pow(BasalAreaUnderBarkM2,-0.281))*(pow(Age,0.125));

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

    BarkProcentRemoval = 9.50 * (pow(QmdCm,b1)) * (pow(BasalAreaM2,0.135)) * (pow(Age,0.112));
}








//main always returns 0 if everything OK.
int main()
{
  
  //Southern Sweden
  std::cout << "12 m at 20 yr at 20 yr should be 12: " << HagglundHeightSpruceSouth(12,20,20) << "\n"
            << "20 m at 40 yr at 100 yr should be G32: " << HagglundHeightSpruceSouth(20.7,40,100) << "\n"
            << "G34 at 70 yr should be 30.6 : " << HagglundHeightSpruceSouth(34,100,70) << std::endl;
  
  return 0;
}
