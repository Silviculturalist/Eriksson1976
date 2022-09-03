#pragma once

#include <math.h>
#include <cmath>
#include <iostream>
#include <vector>

double Phi0(double X){
    return (1/sqrt(2*3.141593))*exp(-(pow(X,2)/2));
}

double Phi3(double X){
    return (-pow(X,3)+3*X)*Phi0(X);
}

double Phi4(double X){
    return (pow(X,4)-6*pow(X,2)+3)*Phi0(X);
}

double Phi6(double X)
{
    return (pow(X,6)-15*pow(X,4)+45*pow(X,2)-15)*Phi0(X);
}

//Cramer numeric estimation of probability density function of skewed normal distribution.
double CramerNormalPDF(
    double x,
    double mean,
    double sd,
    double skew,
    double kurtosis,
    double total)
{

    x = (x-mean)/sd; //x is number of sd from mean.
    return (
        (total/sd)*(
            Phi0(x)-(skew/(1*2*3))*Phi3(x)+
            (kurtosis/(1*2*3*4))*Phi4(x) +
            (10*pow(skew,2))/(1*2*3*4*5*6)*Phi6(x)
            )
    ) ;
}

/*
std::vector<double> ErikssonGetStemOutput(
*/
std::vector<double> ErikssonGetStemOutput(
    double ageBH,
    double dominantHeight,
    double QMD, 
    double volume, 
    int stems
    )
{

    double dominantHeightDM = dominantHeight*10;

    //SD of the volume distribution.
    double volumeSD = 0.028*pow(dominantHeightDM,1.043)*pow(QMD,-0.260)*pow(ageBH,0.076);

    //Coefficient of asymmetry for volume distribution
    double volumeAsym = 9.757*pow(dominantHeightDM,-0.289)*pow(QMD,0.048)*pow(ageBH,-0.035)*(pow((volume/100),0.024))*pow(volumeSD,0.205)-3;

    //Coefficient of excess for volume distribution
    double volumeExcess = 4.851*pow((volumeAsym+3),-0.387)-3;

    //Developed relation from Great Yield investigation plots.
    double volumeMean =  QMD + (0.000022*pow(dominantHeightDM,3.238)*pow(QMD,-2.118)*pow(ageBH,0.239)*pow((volume/100),-0.274));

    //SD of diameter distribution
    double diameterSD = 0.01*pow(dominantHeightDM,1.671)*pow(QMD,-0.855)*pow((stems/1000),-0.182);

    //Coefficient of asymmetery for diameter distribution
    double diameterAsym = 1.528*pow(dominantHeightDM,0.405)*pow(QMD,-0.530)*pow((stems/1000),-0.017)-3;

    //Coefficient of excess for volume distribution
    double diameterExcess = 1.844*pow(dominantHeightDM,0.276)*pow(QMD,0.014)*pow((stems/1000),-0.163)*pow(ageBH,-0.050)*pow(diameterSD,-0.477)-3;

    //Get arithmetic mean diameter (Cajanus theorem).
    double diameterArithmetic = sqrt((pow(QMD,2)-pow(diameterSD,2)));

    //Vector doesn't care about length.
    std::vector<double> stemVector;

    //Fill vector with Class, Stem Frequency,Volume Frequency.

    //Push Diameter class 0.5 first (not normal width).
    stemVector.push_back(0.5);
    stemVector.push_back((CramerNormalPDF(0.5,diameterArithmetic,diameterSD,diameterAsym,diameterExcess,200)>=0.1)?CramerNormalPDF(0.5,diameterArithmetic,diameterSD,diameterAsym,diameterExcess,200):0);
    stemVector.push_back((CramerNormalPDF(0.5,volumeMean,volumeSD,volumeAsym,volumeExcess,200)>=0.1)?CramerNormalPDF(0.5,volumeMean,volumeSD,volumeAsym,volumeExcess,200):0);

    for(int i=0;i< 25 ;i++) // (maxClass 49.5- minClass 1.5)/2 == 24
    {
        stemVector.push_back(1.5+i*2);
        stemVector.push_back((CramerNormalPDF((1.5+2*i),diameterArithmetic,diameterSD,diameterAsym,diameterExcess,200)>=0.1)?CramerNormalPDF((1.5+2*i),diameterArithmetic,diameterSD,diameterAsym,diameterExcess,200):0);
        stemVector.push_back((CramerNormalPDF((1.5+2*i),volumeMean,volumeSD,volumeAsym,volumeExcess,200)>=0.1)?CramerNormalPDF((1.5+2*i),volumeMean,volumeSD,volumeAsym,volumeExcess,200):0);
    }

    return stemVector;
    
} 
