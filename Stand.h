#pragma once 
#include "HagglundHeightSpruce.h"
#include "ErikssonStemDistributions.h"

class StandObject{
    public:
        double H100; //Site Index Hägglund 1972.
        std::vector<double> DominantHeight; //Current DominantHeight
        double Latitude; //Latitude of site
        double GroupStructureIndex{0}; //Coefficient of variation for the BA under bark on 5 meter circular plots inH100de the sample plot.. in percent.
        int Plantedbool{1}; // 1 if planted, 0 if natural regeneration.
        std::vector<int> Age; //Total stand age
        std::vector<int> AgeBH; //Age at breast height
        std::vector<double> BasalAreaM2; //Basal Area in m2
        std::vector<double> BasalAreaUnderBarkM2; // BA under bark. 
        std::vector<double> FormHeight; //Form Height of unthinned stand.
        std::vector<double> FormHeightThinned; //FH of thinned stand.
        std::vector<int> StemsStart; //Stems unthinned stand.
        std::vector<int> StemsThinned; //Stems removed.
        std::vector<int> StemsSelfThinned; //Stems removed from self-thinning.
        std::vector<int> StemsEnd; // Stems after thinning and self-thinning.
        std::vector<double> QmdCm; //QMD.
        std::vector<double> QmdUnderBarkCm; //QMD under bark.
        std::vector<double> QmdThinnedCm; //QMD of thinned stems.
        std::vector<double> QmdUnderBarkThinnedCm; // QMD under bark of thinned stems.
        std::vector<double> QmdSelfThinnedCm; //QMD of self-thinned stems.
        std::vector<double> Volume; //Volume of stand, m3sk. (cubic metres.)
        std::vector<double> ThinnedBA; //Thinned Basal Area.
        std::vector<double> ThinnedVOL; // Thinned Volume during the period.
        std::vector<double> ThinningPercent; //Thinning during the period as percent of the BA under bark.
        std::vector<double> SelfThinnedBA; //The expected BA to be self-thinned from mortality.
        std::vector<double> ThinningForm; // QMD removed / QMD stand before thinning.
        std::vector<double> MAI_VOL; //Mean Annual Increment - Volume.
        std::vector<double> MAI_BA; // ... - Basal Area.
        std::vector<double> CAI_VOL; // Current Annual Increment - Volume.
        std::vector<double> BAI; //BA increment under bark.
        std::vector<double> BAIoverBark; //BA increment over bark.
        
        //General functions
        void growth_period(); //growth section.
        void cut(double vol); //cutting action
        void result(); //get results..

        //Eriksson1976 specific helpers.
        int getIncrement(){return 5;};
        void initialBA(); // calculate initial basal area of stand.
        void formheight(); //the form height of the stand.
        void formheightthinned(); //Form height of the thinned stand.
        double bai(int increment); // Basal area increment during the period. Not suitable for stands with dominant height < 7 m.
        void selfthinning(); // Mortality from self-thinning.
        double barkprocentaddition(); // Bark percent addition to BA.
        double barkprocentremoval(); // Bark percent removal to BA.
        double getHeight(int requestedAge); //get height at age.

        //Constructor Destructor.
        StandObject(double h100, double startHeight, double latitude, int stems, bool planted); //Constructor
        ~StandObject(); //Destructor


};


void StandObject::growth_period(){
  /*
  int periodIncrement = getIncrement(); //request next increment.

  //Basal area increment under bark. OBS per annum!
  BAI.push_back(bai(periodIncrement)*periodIncrement); // Total increment is annual times number of periods.
  BasalAreaUnderBarkM2.push_back(BasalAreaUnderBarkM2.back()+BAI.back()); //increment Basal area under bark.
  QmdUnderBarkCm.push_back()
  BasalAreaM2.push_back(BasalAreaUnderBarkM2.back()+(barkprocentaddition()*BasalAreaUnderBarkM2.back()/100)) //bark procent addition.
  */


};


void StandObject::result(){
  //Print header for csv format.
  std::cout << "H100;Latitude;Planted;Age;AgeBH;DomH;BA;BAUB;FH;FH Thinned;StemsStart;StemsThinned;StemsSelfThinned;StemsEnd;QMD;QMDUB;QMDThinned;QMDUBThinned;QMDSelfThinned;Volume;ThinnedBA;ThinnedVOL;ThinningPercent;SelfThinnedBA;ThinningForm;MAI_VOL;MAI_BA;CAI_VOL;BAI;BAIoverBark;" << std::endl;

  for(int i=0; i<Volume.size(); i++){
    std::cout << H100 << ";"
              << Latitude << ";"
              << Plantedbool << ";"
              << Age[i] << ";" 
              << AgeBH[i] << ";" 
              << DominantHeight[i] << ";" 
              << BasalAreaM2[i] << ";"
              << BasalAreaUnderBarkM2[i] << ";"
              << FormHeight[i] << ";"
              << FormHeightThinned[i] << ";"
              << StemsStart[i] << ";"
              << StemsThinned[i] << ";"
              << StemsSelfThinned[i] << ";"
              << StemsEnd[i] << ";"
              << QmdCm[i] << ";"
              << QmdUnderBarkCm[i] << ";"
              << QmdThinnedCm[i] << ";"
              << QmdUnderBarkThinnedCm[i] << ";"
              << QmdSelfThinnedCm[i] << ";"
              << Volume[i] << ";"
              << ThinnedBA[i] << ";"
              << ThinnedVOL[i] << ";"
              << ThinningPercent[i] << ";"
              << SelfThinnedBA[i] << ";"
              << ThinnedBA[i] << ";"
              << ThinningForm[i] << ";"
              << MAI_VOL[i] << ";"
              << MAI_BA[i] << ";"
              << CAI_VOL[i] << ";"
              << BAI[i] << ";"
              << BAIoverBark[i] << ";" 
              << std::endl;

  }
  
}

//Update the basal area increment.
double StandObject::bai(int increment) 
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

  double VG = 13.778 * pow((AgeBH.back()/10),c1) * pow((StemsStart.back()/1000),-0.052) ; //Group structure index (p. 112), coef. variation for basal area under bark on 5 meter circular plots inside the sample plots.

  //Calculate expected basal area increment.  
  double baiVal = 2.635 * pow(BasalAreaUnderBarkM2.back(),b1) * pow((ThinningPercent.back()+0.01),b2) * pow(
        ((QmdUnderBarkThinnedCm.back()/(QmdUnderBarkCm.back())+0.1) * ((100*StemsThinned.back())/(StemsStart.back())+0.01)),0.024 //Note 0.024 is corrected from typo p. 63.
    ) * pow(increment,b3) * pow((DominantHeight.back()*10),b4) * pow(AgeBH.back(),b5) * pow(VG,b6)  ;

  return baiVal;  
};

//Update barkprocentaddition
double StandObject::barkprocentaddition() //Bark addition percent for basal area under bark at breast height.
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

    double procAdd = 45.08*(pow(QmdUnderBarkCm.back(),b1))*(pow(BasalAreaUnderBarkM2.back(),-0.281))*(pow(AgeBH.back(),0.125));

    return procAdd;

};



double StandObject::barkprocentremoval() //Bark removal percent for basal area at breast height.
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

    double procRemove =  9.50 * (pow(QmdCm.back(),b1)) * (pow(BasalAreaM2.back(),0.135)) * (pow(AgeBH.back(),0.112));

    return procRemove;
};



double StandObject::getHeight(int requestedAge)
{
    double Hval;

    if(Latitude>60){ //Norrland and Kopparbergs County (pre-1998 county reorganisation)
        Hval = HagglundHeightSpruceNorth(
        H100,
        (100-HagglundTimeToBreastHeightSpruceNorth(H100,Latitude,Plantedbool)),
        (requestedAge-HagglundTimeToBreastHeightSpruceNorth(H100,Latitude,Plantedbool)),
        Latitude,
        Plantedbool
        );
    } else { //Southern Sweden.
        Hval = HagglundHeightSpruceSouth(
        H100,
        (100-HagglundTimeToBreastHeightSpruceSouth(H100)),
        (requestedAge-HagglundTimeToBreastHeightSpruceSouth(H100))
        );
    }

    return Hval;

};


//Update the amount of self-thinning.
void StandObject::selfthinning() 
{
  double b1;
  double b2;
  double StemsStart2;
  
  //Get the QMD of the self-thinned stems (above bark), e.g. f. 8.1.1., p. 107.
  QmdSelfThinnedCm.push_back((0.039* pow((DominantHeight.back() * 10),0.517)) * QmdCm.back());
  
    //Modification required if StemsStart exceed 4500 and DominantHeight at same time is more than 15m.
    if(DominantHeight.back()>15 && StemsStart.back()>4500){
        StemsStart2 = StemsStart.back() + 0.1*(StemsStart.back()-4500);
    } else {
        StemsStart2 = StemsStart.back();
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
  
  //Update vectors.
  SelfThinnedBA.push_back(3.25 * (pow(10,-10)) * (pow((StemsStart2/1000),b1)) * (pow((DominantHeight.back()*10),b2)));
  StemsSelfThinned.push_back((SelfThinnedBA.back() * 40000)/(pow(QmdSelfThinnedCm.back(),2) * 3.14159265359));
};

//Update form height of stand.
void StandObject::formheight()
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

  FormHeight.push_back(pow(10,-1.141) * pow((DominantHeight.back()*10),b1) * pow(QmdCm.back(),0.123));
}

//Update form height for thinned stems.
void StandObject::formheightthinned()
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

  FormHeightThinned.push_back(pow(10,-1.141) * pow((DominantHeight.back()*10),b1) * pow(QmdThinnedCm.back(),0.123));
};

//Get an initial BA from stems and height. f. 7.2.1, p. 99.
void StandObject::initialBA() 
{
  double b1;
  double initBA;
  
  if(Plantedbool){
    b1 = 0.355;
  } else {
    b1 = 0.319;
  }

    initBA = 1.0111 * pow((DominantHeight.back() - 1.3),1.230) * pow((StemsStart.back()/1000),b1);
    BasalAreaM2.push_back(initBA);
}



//StandObject constructor
StandObject::StandObject
(
    double h100,
    double startHeight,
    double latitude,
    int stems,
    bool planted
)
{
        H100 = h100;
        Latitude = latitude;
        Plantedbool = planted;
        AgeBH.push_back((int) (BHAgeFinder(h100,latitude,startHeight)+0.5)); //round to integer.
        //Get total age by adding time to breast height, here with ternary operator to distinguish north and south sweden.
        Age.push_back((int) (((latitude>60)?(AgeBH[0] + HagglundTimeToBreastHeightSpruceNorth(h100,latitude,Plantedbool)):(AgeBH[0] + HagglundTimeToBreastHeightSpruceSouth(h100)))+0.5));
        StemsStart.push_back(stems);     

        //Set the starting height which fits truncated Ages.
        if(Latitude>60){ //Norrland and Kopparbergs County (pre-1998 county reorganisation)
            DominantHeight.push_back(HagglundHeightSpruceNorth(
              H100,
              (100-HagglundTimeToBreastHeightSpruceNorth(H100,Latitude,Plantedbool)),
              AgeBH[0],
              Latitude,
              Plantedbool
              ));
        } else { //Southern Sweden.
            DominantHeight.push_back(HagglundHeightSpruceSouth(
              H100,
              (100-HagglundTimeToBreastHeightSpruceSouth(H100)),
              AgeBH[0]
            ));
        }

        //Set inital basal area
        initialBA();

        //Calculate QMD (Area of the mean basal area stem.), in cm.
        QmdCm.push_back(
            2* sqrt(((BasalAreaM2.back()/StemsStart.back())*10000)/3.14159265359)
        );

        //Update form height of stand.
        formheight();

        //Set the volume of the stand.
        Volume.push_back(BasalAreaM2.back()*FormHeight.back());

    
        //Basal area under bark.
        BasalAreaUnderBarkM2.push_back(BasalAreaM2.back()-((BasalAreaM2.back()*barkprocentremoval())/100));

        //QMD under bark, in cm.
        QmdUnderBarkCm.push_back(2*sqrt(((BasalAreaUnderBarkM2.back()/StemsStart.back())*10000)/3.14159265359));
                    
        //First period never any thinning.
        ThinningPercent.push_back(0);
        ThinningForm.push_back(0);
        ThinnedVOL.push_back(0);
        ThinnedBA.push_back(0);
        StemsThinned.push_back(0);
        FormHeightThinned.push_back(0);
        QmdUnderBarkThinnedCm.push_back(0);
        QmdThinnedCm.push_back(0);

        //Get self-thinning (need to be multiplied by length of period..)
        selfthinning();

        //get increment
        int increment = getIncrement();

        //make sure self thinning is period length..
        SelfThinnedBA.back() = SelfThinnedBA.back()*increment;
        StemsSelfThinned.back() = StemsSelfThinned.back()*increment;

        //Stems end.
        StemsEnd.push_back(StemsStart.back()-StemsSelfThinned.back());

        //Annual basal area increment
        BAI.push_back(bai(increment));
        
        //BAI over bark 
        BAIoverBark.push_back(((barkprocentaddition()*BAI.back())/100 + BAI.back()));

        //Mean Annual Increments: 
        MAI_VOL.push_back(Volume.back()/Age.back());
        MAI_BA.push_back(BasalAreaM2.back()/Age.back());
        CAI_VOL.push_back(0); //cannot calculate current annual increment for first year.
                
};