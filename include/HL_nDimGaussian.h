//   HEPLike: High Energy Physics Likelihoods
//
//   Header for nDimGaussian
//
//   author: Jihyun Bhom, Marcin Chrzaszcz
//////////////////////////////////////////////////
#ifndef HL_NDIMGAUSSIAN_H
#define HL_NDIMGAUSSIAN_H         

//C++ headers
#include <cmath>
#include <limits>
#include <fstream>
#include <iomanip>


//HL_Like headers
#include "HL_Stats.h"
#include "HL_Data.h"

//external:
#include "yaml-cpp/yaml.h"
#include "gsl/gsl_cdf.h"
#include "gsl/gsl_sf_erf.h"
#include "gsl/gsl_math.h"
#include "gsl/gsl_sf_exp.h"
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
//#include <boost/qvm/mat_operations.hpp>


class HL_nDimGaussian: public HL_Data
{

 public:

  explicit HL_nDimGaussian() :  HL_Data() {};
  explicit HL_nDimGaussian(std::string s) :  HL_Data(s) { };
  
  
  void Read();
  double GetChi2( std::vector<double> theory) ;
  double GetLikelihood( std::vector<double> theory) ;  
  double GetLogLikelihood(  std::vector<double> theory) ;  
  bool Restrict(std::vector<std::string>);
  
 private:
  
  int NoOfObservables;
  int size_restricted;

  std::vector<double> central;
  std::vector<double> stat_error;
  std::vector<double> syst_error;
  std::vector<std::string> Observables;

  boost::numeric::ublas::matrix<double> HL_cov;
  boost::numeric::ublas::matrix<double> HL_correlation;  
  boost::numeric::ublas::matrix<double> HL_cov_inv;

  std::vector<double> central_restricted;   
  boost::numeric::ublas::matrix<double> HL_cov_restricted;
  boost::numeric::ublas::matrix<double> HL_correlation_restricted;
  boost::numeric::ublas::matrix<double> HL_cov_inv_restricted;
  
  
  bool restricted;
  
  
};


#endif
