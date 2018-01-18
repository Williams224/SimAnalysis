#include"IncidentEnergy.hpp"

IncidentEnergy::IncidentEnergy(std::string _GaussFileName, std::string _ProfilesFileName, double _Energy):GaussFileName(_GaussFileName),ProfilesFileName(_ProfilesFileName),Energy(_Energy),OneOverSqrtE(1./sqrt(_Energy)){
  
  std::cout<<"Constructror Called for energy = "<<Energy<<std::endl;
  TFile * GaussFile=new TFile(GaussFileName.data());
  vScint=(std::vector<double>*)GaussFile->Get("VScintDepos");
  vLead=(std::vector<double>*)GaussFile->Get("VLeadDepos");
  if(!vScint){
    std::cout<<"Unable to get scint vector from file"<<std::endl;
  }
  if(!vLead){
    std::cout<<"Unable to get lead vector from file"<<std::endl;
  }
  std::cout<<"Loaded "<<vScint->size()<<" incident energies"<<std::endl;
    
    

  /*  double smallestScint=5000.0;
  double largestScint=0.0;
  double smallestLead=5000.0;
  double largestLead=0.0;

  for(auto s : *vScint){
    if(s<smallestScint)smallestScint=s;
    if(s>largestScint)largestScint=s;
  }
  for(auto l : *vLead){
    if(l<smallestLead)smallestLead=l;
    if(l>largestLead)largestLead=l;
    }*/

  ScintMean=CalcMean(vScint);
  LeadMean=CalcMean(vLead);
  ScintRMS=CalcRMS(vScint,ScintMean);
  //LeadRMS=CalcRMS(vLead,LeadMean);
  std::cout<<" Scint Mean = "<<ScintMean<<" Scint RMS "<<ScintRMS<<std::endl;

  double ScintLowerLim=ScintMean-6.0*ScintRMS;
  double ScintUpperLim=ScintMean+6.0*ScintRMS;
  std::cout<<"Histogram lower limit "<<ScintLowerLim<<" and upper limit "<<ScintUpperLim<<std::endl;

  double LeadLowerLim=LeadMean-6.0*LeadRMS;
  double LeadUpperLim=LeadMean+6.0*LeadRMS;
  
  std::string ScintName="ScintHist_"+std::to_string(Energy);
  std::string LeadName="LeadHist_"+std::to_string(Energy);
  ScintHist=new TH1D(ScintName.data(),ScintName.data(),50,ScintLowerLim,ScintUpperLim);
  LeadHist=new TH1D(LeadName.data(),LeadName.data(),50,LeadLowerLim,LeadUpperLim);

  for(auto s : *vScint){
    //    if(!isnan(s)){
      //      std::cout<<"not a nan apparently "<<s<<std::endl;
    ScintHist->Fill(s);
    //    }else{
    //std::cout<<"Where did this nan come from?"<<std::endl;
    //    }
    //    std::cout<<s<<",";
    
  }
  
  for(auto l : *vLead){
    LeadHist->Fill(l);
  }
  //  std::cout<<ScintHist->GetXaxis()->GetXmin()<<std::endl;
  std::cout<<"Scint Hist Entries= "<<ScintHist->GetEntries()<<std::endl;
  TCanvas C;
  ScintHist->Draw("E");
  C.SaveAs("TestCanvas.pdf");
  TFitResultPtr ScintResult=ScintHist->Fit("gaus","S");
  std::cout<<"After Scint Hist Fit"<<std::endl;
  //  ScintResult->Print("v");
  //  gStyle->SetOptFit();
  
  TF1 *ScintFit=ScintHist->GetFunction("gaus");
  FittedMean=ScintFit->GetParameter(1);
  FittedMeanUncert=ScintFit->GetParError(1);
  FittedWidth=ScintFit->GetParameter(2);
  FittedWidthUncert=ScintFit->GetParError(2);
  
  SigmaOverE=FittedWidth/FittedMean;
  SigmaOverEUncert=CalcUncert(FittedWidth,FittedWidthUncert,FittedMean,FittedMeanUncert);
}

double IncidentEnergy::CalcMean(std::vector<double>* v){
  double Sum=std::accumulate(v->begin(), v->end(), 0.0);
  std::cout<<" Sum= "<<Sum<<std::endl;
  return Sum/double(v->size());
}

double IncidentEnergy::CalcRMS(std::vector<double>* v, double Mean){
  int counter=0;
  double SumOfSquares=std::accumulate(v->begin(),v->end(),0.0,[&](double a, double b){
      return a+((b-Mean)*(b-Mean));
    });
  double Variance=SumOfSquares/double(v->size());
  return sqrt(Variance);
}

double IncidentEnergy::CalcUncert(double n, double nuncert, double d, double duncert){
  double UncertA=nuncert/d;
  double UncertB=(n*duncert)/(d*d);
  return sqrt((UncertA*UncertA)+(UncertB*UncertB));
}

  


