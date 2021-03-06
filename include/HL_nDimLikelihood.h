//   HEPLike: High Energy Physics Likelihoods
//
//   Header for ndim Likelihood
//
//   author: Jihyun Bhom, Marcin Chrzaszcz
//////////////////////////////////////////////////


#ifndef HL_NDIMLIKELIHOOOD_H
#define HL_NDIMLIKELIHOOOD_H

//C++ headers
#include <cmath>
#include <limits>
#include <fstream>
#include <iomanip>


//HL_Like headers
#include "HL_Stats.h"
#include "HL_Data.h"

//external:
#include "TFile.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "yaml-cpp/yaml.h"
#include "gsl/gsl_cdf.h"
#include "gsl/gsl_sf_erf.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_sf_exp.h"
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
//#include <boost/qvm/mat_operations.hpp>
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/IFunction.h"  
#include "TRandom2.h"
#include "TError.h"



class MyFunction2D: public ROOT::Math::IBaseFunctionMultiDim{
 public:
  double DoEval(const double* theory_nuisance) const{
    double loglikelihood_penalty=1e10;
    int bin;
    bin=likelihood->FindBin(theory_nuisance[0], theory_nuisance[1]);
    double xmin=likelihood->GetXaxis()->GetXmin();
    double xmax=likelihood->GetXaxis()->GetXmax();
    double ymin=likelihood->GetYaxis()->GetXmin();
    double ymax=likelihood->GetYaxis()->GetXmax();
    if(theory_nuisance[0]>xmax) return loglikelihood_penalty;
    if(theory_nuisance[0]<xmin) return loglikelihood_penalty;
    if(theory_nuisance[1]>ymax) return loglikelihood_penalty;
    if(theory_nuisance[1]<ymin) return loglikelihood_penalty;
    
    
    double negative_LL=likelihood->GetBinContent(bin);
    
    double chi2=0.;
    double diff[2];
    for(unsigned i=0; i < 2; i++)
      {
        diff[i]=theory_nuisance[i]-theory_mean[i];
      }
    boost::numeric::ublas::matrix<double>theory_cov_inv(2,2);
    
    HL_Stats::InvertMatrix(theory_cov, theory_cov_inv);
    
    
    for(unsigned i=0; i <2 ; i++)
      {
        for(unsigned j=0; j< 2 ;  j++)
          {
            chi2+=diff[i] * theory_cov_inv(i,j)*diff[j] ; 
          }
      }
    //
    double deltaLL= 0.5*chi2;

    return (deltaLL+negative_LL); // here both are negative likelihoods that needs to be mimalized

  }
  ROOT::Math::IBaseFunctionMultiDim* Clone() const{
    return new MyFunction2D();
  }
  void SetLikelihood(TH2D *h)
  {
    likelihood=h;
  }
  void SetTheory(vector<double> mean, boost::numeric::ublas::matrix<double> cov)
  {
    theory_mean=mean;
    theory_cov=cov;
  }
  unsigned int NDim() const{
    return 2;
  }
 private:

  vector <double> theory_mean;
  boost::numeric::ublas::matrix<double> theory_cov;
  TH2D *likelihood;

};





class HL_nDimLikelihood: public HL_Data
{

 public:

  explicit HL_nDimLikelihood() :  HL_Data() {};
  explicit HL_nDimLikelihood(std::string s) :  HL_Data(s) { };
  
  
  void Read();
  double GetChi2( std::vector<double> theory) ;
  double GetChi2( std::vector<double> theory,  boost::numeric::ublas::matrix<double> theory_cov);
  double GetLikelihood( std::vector<double> theory) ;  
  double GetLikelihood(std::vector<double> theory, boost::numeric::ublas::matrix<double> theory_cov);
  double GetLogLikelihood(  std::vector<double> theory) ;  
  double GetLogLikelihood(std::vector<double> theory, boost::numeric::ublas::matrix<double> theory_cov);

  
  void Profile();
  double GetChi2_profile( double theory, std::string);
  double GetLikelihood_profile( double theory, std::string axis) ;
  double GetLogLikelihood_profile(  double theory, std::string X);
  
  std::vector<std::string> GetObservables(){ return Observables;};  
  
  double loglikelihood_penalty;
  TH1* GetHist(){ return hist;}; 
  
 private:


  std::string HL_RootFile;
  std::string HL_PATH;
  std::vector<std::string> Observables;

  
  int NoOfObservables;
  int size_restricted;

  TH2D *hist2D;
  TH3D *hist3D;  

  TH1 *hist;
  
  double xmin;
  double xmax;
  double ymin;
  double ymax;
  double zmin;
  double zmax;
    
  std::vector<double> central_mes_val;
  int dim;

  TH1D *hist_profileX;
  TH1D *hist_profileY;  
  TH1D *hist_profileZ;

    
  int n_binsX;  
  int n_binsY;
  int n_binsZ;  


  bool profiled;


  // for minimaization
  ROOT::Math::Minimizer* gmin;
  MyFunction2D fun;   
  
  
  
};


#endif
