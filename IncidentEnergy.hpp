#ifndef INCIDENTENERGY_HPP
#define INCIDENTENERGY_HPP 
#include"TF1.h"
#include<string>
#include<vector>
#include"TH1D.h"
#include<iostream>
#include"TFile.h"
#include<math.h>
#include<algorithm>
#include<numeric>
#include"TFitResult.h"
#include"TCanvas.h"

class IncidentEnergy {
private:
  double Energy;
  double OneOverSqrtE;
  std::string GaussFileName;
  std::string ProfilesFileName;
  std::vector<double>* vScint=NULL;
  std::vector<double>* vLead=NULL;
  TH1D* ScintHist=NULL;
  TH1D* LeadHist=NULL;
  bool debug=false;
  double ScintMean;
  double ScintRMS;
  double LeadRMS;
  double LeadMean;
  double FittedMean;
  double FittedWidth;
  double FittedMeanUncert;
  double FittedWidthUncert;
  double SigmaOverE;
  double SigmaOverEUncert;
  double CalcMean(std::vector<double>* v);
  double CalcRMS(std::vector<double>* v, double Mean);
  double CalcUncert(double n, double nuncert, double d, double duncert);
public:
  IncidentEnergy(std::string _GaussFileName, std::string _ProfilesFileName, double _Energy);
  void setDebug(bool _debug){debug=_debug;}
  TH1D* getScintHist()const {return ScintHist;}
  TH1D* getLeadHist() const {return LeadHist;}
  double getSigmaOverE() const {return SigmaOverE;}
  double getSigmaOverEUncert()const {return SigmaOverEUncert;}
  double getEnergy() const {return Energy;}
  double getOneOverSqrtE()const {return OneOverSqrtE;}
  
  bool operator < (const IncidentEnergy& En) const
  {
    std::cout<<" Energy = "<<Energy<<" En.Energy = "<<En.Energy<<std::endl;
    return (Energy < En.Energy);
  }

};


#endif
