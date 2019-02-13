//   HL_Like: High Energy Physics Likelihoods
//
//   Module to construck likelihoods for Bifurcated gaussian distribution
//
//   author: Jihyun Bhom, Marcin Chrzaszcz
//////////////////////////////////////////////////


#include <cmath>
#include <limits>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "HL_Stats.h"
#include "HL_Constants.h"
#include "HL_BifurGaussian.h"

using namespace std;

void HL_BifurGaussian::read()
{
  if(! initialized)
    {
      std::cout << "TRYING TO READ WITHOUT GIVING ANY FILE!" << std::endl;
      return;
    }

  read_standard();

  if(config["Observables"])
    {
      YAML::Node node  = config["Observables"];
      ObsName=node[0][0].as<std::string>();
      HL_Central=node[0][1].as<double>();
      HL_Sigma_stat_R=node[0][2].as<double>();
      HL_Sigma_stat_L=(-1.)*(node[0][3].as<double>());
      HL_Sigma_syst_R=node[0][4].as<double>();
      HL_Sigma_syst_L=(-1.)*(node[0][5].as<double>());
      
      
    }
  else
    {
      std::cout<<"Error in the HL_BifurGaussian class, your yaml file has missing Observables"<<std::endl;
    }

  cout<<HL_Sigma_stat_R<<" "<<HL_Sigma_stat_L<<" "<<HL_Sigma_syst_R<<" "<<HL_Sigma_syst_L<<endl;
}
double HL_BifurGaussian::GetChi2(double theory, double theory_err)
{
  double delta=HL_Central-theory;
  double chi2=0.;
  if(delta>0.)
    {
      double err2=HL_Sigma_stat_L*HL_Sigma_stat_L+ HL_Sigma_syst_L*HL_Sigma_syst_L+theory_err*theory_err;    
      chi2=delta*delta/err2;
    }
  else
    {
      double err2=HL_Sigma_stat_R*HL_Sigma_stat_R+ HL_Sigma_syst_R*HL_Sigma_syst_R+theory_err*theory_err;
      chi2=delta*delta/err2;
    }
  
  return chi2;

}


double HL_BifurGaussian::GetLogLikelihood(double theory, double theory_err)
{

  double chi2=GetChi2(theory,theory_err);

  return -0.5*chi2;
}
double HL_BifurGaussian::GetLikelihood(double theory, double theory_err)
{
  double log_likelihood=GetLogLikelihood(theory,theory_err);
  return gsl_sf_exp(log_likelihood);
}





