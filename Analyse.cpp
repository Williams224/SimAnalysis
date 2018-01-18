#include<math.h>
#include"TGraphErrors.h"
#include<sstream>
#include<iomanip>
#include<iostream>
#include"IncidentEnergy.hpp"

std::string GetGaussFileName(std::string Dir, double Energy, int nEvents){
  int GeV=int(Energy);
  double MeV=(Energy-GeV)*100.0;
  std::stringstream ss;
  ss << std::fixed << std::setprecision(0) << MeV;
  std::string MeVString = ss.str();
  return Dir+"/Gauss_"+std::to_string(GeV)+"-"+MeVString+"_"+std::to_string(nEvents)+".root";
}

bool IESortPredicate(const IncidentEnergy* d1, const IncidentEnergy* d2)
{
  return d1->getEnergy() < d2->getEnergy();
}

double StraightResFunction(double *x/*x=1/sqrt(E)*/, double* par /*par[0] = A, par[1] = C*/){
  double PDF=0.0;
  //x=1/sqrt(E)
  //s/E=sqrt((ax)**2+c**2)
  double ATerm=(par[0]*x[0])*(par[0]*x[0]);
  double CTerm=par[1]*par[1];
  PDF=sqrt(ATerm+CTerm);
  return PDF;
}

double CurvedResFucntion(double *x/*x=E*/, double * par /*par[0] = A,par[1] = C*/){
  double PDF=0.0;
  double ATerm=(par[0]*par[0])/x[0];
  double CTerm=par[1]*par[1];
  PDF=sqrt(ATerm+CTerm);
  return PDF;
}

int main(){
  std::string ResultsDir="/home/tw/Sim_Thesis/Geant4_v95r2p8g2/Geant4/G4examples/extended/electromagnetic/G4TestEm3/Results";

  std::vector<IncidentEnergy*> IncidentEnergies;
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,400.0,10000),"ProfileName",400.0));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,177.78,10000),"ProfileName",177.78));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,100.00,10000),"ProfileName",100.0));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,44.44,10000),"ProfileName",44.44));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,25.00,10000),"ProfileName",25.00));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,16.00,10000),"ProfileName",16.00));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,11.11,10000),"ProfileName",11.11));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,8.16,10000),"ProfileName",8.16));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,6.25,10000),"ProfileName",6.25));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,4.0,10000),"ProfileName",4.0));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,4.94,10000),"ProfileName",4.94));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,3.31,10000),"ProfileName",3.31));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,2.78,10000),"ProfileName",2.78));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,2.37,10000),"ProfileName",2.37));
  IncidentEnergies.push_back(new IncidentEnergy(GetGaussFileName(ResultsDir,1.78,10000),"ProfileName",1.78));

  std::sort(IncidentEnergies.begin(),IncidentEnergies.end(),IESortPredicate);

  std::vector<double> vSigmaOverE;
  std::vector<double> vSigmaOverEUncert;
  std::vector<double> vE;
  std::vector<double> vOneOverSqrtE;
  
  for(auto IE: IncidentEnergies){
    vSigmaOverE.push_back(IE->getSigmaOverE());
    vSigmaOverEUncert.push_back(IE->getSigmaOverEUncert());
    vE.push_back(IE->getEnergy());
    vOneOverSqrtE.push_back(IE->getOneOverSqrtE());
  }
  
  // Gymastics to satisfy const requirement of TGraphErrors :(
  const std::vector<double> cvSigmaOverE(vSigmaOverE);
  const std::vector<double> cvSigmaOverEUncert(vSigmaOverEUncert);
  const std::vector<double> cvE(vE);
  const std::vector<double> cvOneOverSqrtE(vOneOverSqrtE);
  const std::vector<double> NoError(IncidentEnergies.size(),0.0); //Required to specify no error on E
  
  TGraphErrors * StraightResPlot= new TGraphErrors(int(IncidentEnergies.size()),cvOneOverSqrtE.data(),cvSigmaOverE.data(),NoError.data(),cvSigmaOverEUncert.data());

  TF1 * CaloResStraight = new TF1("CaloResStraight", StraightResFunction ,0.0,0.8,2);
  CaloResStraight->SetParName(0,"A");
  CaloResStraight->SetParName(1,"C");
  CaloResStraight->SetParameters(0.09,0.004);

  StraightResPlot->Fit(CaloResStraight,"S");

  TGraphErrors* CurvedResPlot= new TGraphErrors(int(IncidentEnergies.size()),cvE.data(),cvSigmaOverE.data(),NoError.data(),cvSigmaOverEUncert.data());

  TF1* CaloResCurved = new TF1("CaloResCurved",CurvedResFucntion,0.0,410.0,2);
  CaloResCurved->SetParName(0,"A");
  CaloResCurved->SetParName(1,"C");
  CaloResCurved->SetParameters(0.09,0.004);
  CaloResCurved->SetRange(-1.1,410.0);
  

  CurvedResPlot->Fit(CaloResCurved,"S");
  TCanvas CurvedResCanvas("CurvedResCanvas","CurvedResCanvas");
  CurvedResPlot->Draw("AP");
  CurvedResCanvas.SaveAs("CurvedResPlot.pdf");

  TCanvas StraightResCanvas("StraightResCanvas","StraightResCanvas");
  StraightResPlot->Draw("AP");
  StraightResCanvas.SaveAs("StraightResPlot.pdf");

  TCanvas FunctionOnlyCanvas("FunctionOnlyCanvas","FunctionOnlyCanvas");
  CaloResCurved->Draw();
  FunctionOnlyCanvas.SaveAs("FunctionOnlyCanvas.pdf");
    

    
  TFile * output = new TFile("Output.root","RECREATE");
  for(auto I : IncidentEnergies){
    //    std::cout<<I->getEnergy()<<std::endl;
    I->getScintHist()->Write();
  }
  StraightResPlot->Write();
  CurvedResPlot->Write();
  output->Close();
}
