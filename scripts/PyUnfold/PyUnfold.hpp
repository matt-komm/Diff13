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
        
        std::vector<std::string> _backgroundNames;
        
        
        const TH1* _dataHist;
        const TH2* _responseHist;
    
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
            _tunfold(responseHist,TUnfold::kHistMapOutputHoriz,TUnfold::kRegModeCurvature),
            _responseHist(responseHist)
        {
            _tunfold.SetBias(responseHist->ProjectionX());
        }
        
        void addBackground(const TH1* background, const char* name, double scale=1.0, double error=0.00001)
        {
            _backgroundNames.push_back(name);
            _tunfold.SubtractBackground(background,name,scale,error);
        }
        
        void setData(const TH1* data)
        {
            _dataHist=data;
            _tunfold.SetInput(data);
            
        }
        
        void unfold(double tau, TH1* output, TH2* correlations, bool includeStat=true, bool includeMCStat=true, bool includeFit=true)
        {
            _tunfold.DoUnfold(tau);
            _tunfold.GetOutput(output);
            
            
        }
        
        std::vector<double> getUnfoldingCorretation(double tau)
        {
            return {};
        }
        
        virtual ~PyUnfold()
        {
        }

    ClassDef(PyUnfold, 1)
};

#endif


