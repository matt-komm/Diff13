#ifndef __PYUNFOLD_H__
#define __PYUNFOLD_H__

#include "TUnfold.h"
#include "TUnfoldSys.h"

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TMatrixD.h"
#include "TMath.h"

#include "PyUtils.hpp"

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <stdexcept>

class Utils
{
    private:
        Utils() {}
        virtual ~Utils() {}
    public:
        static TMatrixD convert2DHistToMatrix(const TH2* hist);
        static std::vector<double> getBinByBinCorrelations(const TH2* hist);

};

class PyUnfold
{
    private:
        TUnfoldSys _tunfold;
        
        std::vector<std::string> _backgroundNames;
        std::vector<const TH1*> _backgroundHists;
        
        const TH1* _dataHist;
        const TH2* _responseHist;
    

    public:

        PyUnfold(TH2* responseHist):
            _tunfold(responseHist,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature),
            _responseHist(responseHist)
        {
            //_tunfold.SetBias(responseHist->ProjectionX());
        }
        
        void addBackground(const TH1* background, const char* name, double scale=1.0, double error=0.00001)
        {
            _backgroundNames.push_back(name);
            _backgroundHists.push_back(background);
            _tunfold.SubtractBackground(background,name,scale,error);
        }
        
        void setData(const TH1* data)
        {
            _dataHist=data;
            _tunfold.SetInput(data);
            
        }
        
        inline int getNRecoBins() const
        {
            return _responseHist->GetNbinsY();
        }
        
        inline double getRecoMin() const
        {
            return _responseHist->GetYaxis()->GetXmin();
        }
        
        inline double getRecoMax() const
        {
            return _responseHist->GetYaxis()->GetXmax();
        }
        
        inline int getNGenBins() const
        {
            return _responseHist->GetNbinsX();
        }
        
        inline double getGenMin() const
        {
            return _responseHist->GetXaxis()->GetXmin();
        }
        
        inline double getGenMax() const
        {
            return _responseHist->GetXaxis()->GetXmax();
        }
        
        void doUnfolding(double tau, TH1* output, TH2* covariance=nullptr, bool includeStat=true, bool includeMCStat=true, bool includeFit=true)
        {
            if (output==nullptr)
            {
                throw std::runtime_error("Given output histogram to store the result is null");
            }
        
            _tunfold.DoUnfold(tau);
            _tunfold.GetOutput(output);
            
            if (covariance!=nullptr)
            {
	            if (includeStat)
	            {
	                _tunfold.GetEmatrixInput(covariance, 0, false);
	            }
	            if (includeMCStat)
	            {
	                _tunfold.GetEmatrixSysUncorr(covariance ,0, false);  
	                for (const std::string& backgroundName: _backgroundNames)
                    {
                        //add error through limited MC of subtracted background histograms
                        _tunfold.GetEmatrixSysBackgroundUncorr(covariance,backgroundName.c_str() ,0, false);
                    } 
	            }
                if (includeFit)
                {
                    std::string name = covariance->GetName();
                    name+="_fit";
                    TH2* fitMatrix = (TH2*)covariance->Clone(name.c_str());
                    fitMatrix->Scale(0.0);
                    for (const std::string& backgroundName: _backgroundNames)
                    {
                        //add error through fit of background normalization
                        _tunfold.GetEmatrixSysBackgroundScale(fitMatrix,backgroundName.c_str(), 0, false); 
                    }
                    /*
                    for (int i = 0; i < REBIN_GEN; ++i)
                    {
                        for (int j = 0; j < REBIN_GEN; ++j)
                        {
                            if (i!=j)
                            {
                                //ignore tiny correlations
                                fitMatrix->SetBinContent(i+1,j+1,0.0);
                            }
                        }
                    }
                    */
                    covariance->Add(fitMatrix);
                    delete fitMatrix;
	            }
            }
        }
        
        double scanTau(double logBegin=-7, double logEnd=1, unsigned int N=1000)
        {
                        
            double minRho=1.0;
            double bestTau = TMath::Power(10.0,logBegin);
            for (unsigned int iscan = 0; iscan < N; ++iscan)
            {
                const double tau = TMath::Power(10.0,1.0*(iscan/(1.0*N)*(logEnd-logBegin)+logBegin));
                TH1D output("output","",getNGenBins(),getGenMin(),getGenMax());
                TH2D covariance("covariance","",getNGenBins(),getGenMin(),getGenMax(),getNGenBins(),getGenMin(),getGenMax());

                doUnfolding(tau,&output,&covariance);
                std::vector<double> rhos = PyUtils::getBinByBinCorrelations(&covariance);
                if (minRho>rhos[0])
                {
                    minRho=rhos[0];
                    bestTau=tau;
                }
            }
            return bestTau;
        }
        
        virtual ~PyUnfold()
        {
        }

    ClassDef(PyUnfold, 1)
};

#endif


