#ifndef __PYUNFOLD_H__
#define __PYUNFOLD_H__

#include "TUnfold.h"
#include "TUnfoldSys.h"

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TMatrixD.h"
#include "TMath.h"

#include <iostream>
#include <functional>
#include <vector>
#include <string>

class PyUnfold
{
    private:
        TUnfoldSys _tunfold;
    
        static TMatrixD convertHistToMatrix(const TH2D& from)
        {
            TMatrixD matrix(from.GetNbinsX(),from.GetNbinsY());
            for (int row=0; row<from.GetNbinsX();++row){
                for (int col=0; col<from.GetNbinsY(); ++col) {
                    (matrix)[row][col]=(from).GetBinContent(row+1,col+1);
                }
            }
            return matrix;
        }
    public:

        PyUnfold(TH2* responseHist):
            _tunfold(responseHist,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature)
        {
            _tunfold.SetBias(responseHist->ProjectionX());
        }
        
        void addBackground(const TH1* background, const char* name, double scale=1.0, double error=0.00001)
        {
            _tunfold.SubtractBackground(background,name,scale,error);
        }
        
        void setData(const TH1* data)
        {
            _tunfold.SetInput(data);
        }
        
        double scanTau(std::vector<double>& tau, std::vector<std::vector<double> >& rho, double logBegin=-7, double logEnd=-2, unsigned int steps=2000)
        {
            tau.resize(steps);
            rho.resize(steps);
            for (unsigned int itau = 0; itau < steps; ++itau)
            {
                tau[itau]=TMath::Power(10.0,1.0*(itau/(1.0*steps)*(logEnd-logBegin)+logBegin));
            }
            
            return 0;
        }
        
        virtual ~PyUnfold()
        {
        }

    ClassDef(PyUnfold, 1)
};

#endif


