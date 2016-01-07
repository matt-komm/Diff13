#ifndef __PYFIT_H__
#define __PYFIT_H__

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"

#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <limits>

namespace PyFit
{

class Parameter
{
    private:
        std::string _name;
        double _scaleFactor;
        
        double _min;
        double _max;
        
        TF1 _prior;
    public:
        Parameter(const char* name):
            _name(name),
            _scaleFactor(1.0),
            _min(std::numeric_limits<double>::lowest()),
            _max(std::numeric_limits<double>::max()),
            _prior((_name+"prior").c_str(),"1",_min,_max)
        {
        }
        
        inline std::string getName() const
        {
            return _name;
        }
        
        void setPrior(const TF1& prior)
        {
            _prior=prior;
            _prior.SetName((_name+"prior").c_str());
        }
                        
        void setLimits(double min, double max)
        {
            _min=min;
            _max=max;
        }
        
        double setScaleFactor(double scaleFactor)
        {
            return _scaleFactor = scaleFactor;
        }
        
        double logLikelihood()
        {
            return TMath::Log(_prior.Eval(_scaleFactor));
        }
        
        double getScaleFactor() const
        {
            return _scaleFactor;
        }
        
        virtual ~Parameter()
        {
        }
        
    ClassDef(PyFit::Parameter, 1)
};

struct Prediction
{
    std::vector<double> entries;
    Prediction(unsigned int N):
        entries(N)
    {
    }
    Prediction& operator+=(const Prediction& p)
    {
        if (entries.size()!=p.entries.size())
        {
            throw std::runtime_error("attempting to add predictions with different sizes");
        }
        for (unsigned int i = 0; i < entries.size(); ++i)
        {
            entries[i]+=p.entries[i];
        }
        return *this;
    }
};



class Component
{
    public:
        virtual void queryParameters(std::map<std::string, PyFit::Parameter*>& parameters) const = 0;
        virtual void getPrediction(Prediction* p) const = 0;
        
        virtual ~Component()
        {
        }
        
};



class ConstShapeComponent:
    public Component
{
    private:   
        TH1D* _hist; 
        std::vector<PyFit::Parameter*> _parameters; //!
    public: 
        ConstShapeComponent(TH1D* hist):
            _hist((TH1D*)hist->Clone())
        {
            _hist->SetDirectory(0);
        }
        
        void addSFParameter(PyFit::Parameter* parameter)
        {
            _parameters.push_back(parameter);
        }
        
        virtual void queryParameters(std::map<std::string, PyFit::Parameter*>& parameters) const
        {
            for (PyFit::Parameter* parameter: _parameters)
            {
                parameters[parameter->getName()]=parameter;
            }
        }
        
        virtual void getPrediction(Prediction* p) const
        {
            if ((int)p->entries.size()!=_hist->GetNbinsX())
            {
                throw std::runtime_error("histogram in component has different number of bins");
            }
            double combinedSF = 1.0;
            for (const PyFit::Parameter* parameter: _parameters)
            {
                combinedSF*=parameter->getScaleFactor();
            }
            for (unsigned int i = 0; i < p->entries.size(); ++i)
            {
                p->entries[i]+=combinedSF*_hist->GetBinContent(i+1);
            }
            
            
        }
        
        virtual ~ConstShapeComponent()
        {
            delete _hist;
        }
    ClassDef(PyFit::ConstShapeComponent, 1)
};



class Observable
{
    private:
        std::vector<PyFit::Component*> _components; //!
    public:
        Observable()
        {
        }
        
        void getPrediction(Prediction* p)
        {
            for (PyFit::Component* component: _components)
            {
                component->getPrediction(p);
            }
        }
        
        void addComponent(PyFit::Component* component)
        {
            _components.push_back(component);
        }
        
        void queryParameters(std::map<std::string, PyFit::Parameter*>& parameters) const
        {
            for (PyFit::Component* comp: _components)
            {
                comp->queryParameters(parameters);
            }
        }
        
        double nll(TH1D* data)
        {   
            Prediction prediction(data->GetNbinsX());
            this->getPrediction(&prediction);
            
            double ll = 0;
            for ( int ibin = 0; ibin < data->GetNbinsX(); ++ibin)
            {
                const double d = data->GetBinContent(ibin+1);
                const double p = prediction.entries[ibin];
                
                //using log of poisson distribution: x*log(lambda)-lambda
                //skip bins with no prediction
                if (p > 0.0)
                {
                     if (d > 0.0)
                     {
                         ll -= d * std::log(p)-p;
                     }
                 }
                 else if (d > 0.0)
                 {
                     return std::numeric_limits<double>::max();
                 }
            }
            return ll;
        }
        
        virtual ~Observable()
        {
        }
        
    ClassDef(PyFit::Observable, 1)
};
        
class Fit
{
    private:
        std::vector<std::pair<PyFit::Observable*,TH1D*>> _observableDataPairs; //!
        
    
    public:
        Fit()
        {
        }
        
        void addObservable(PyFit::Observable* observable, TH1D* data)
        {
            _observableDataPairs.push_back(std::make_pair(observable,data));
        }
        
        double globalNll() const
        {
            double nll = 0.0;
            
            for (auto pair: _observableDataPairs)
            {
                nll+=pair.first->nll(pair.second);
            }
            
            return nll;
        }
        
        void minimize()
        {
            std::map<std::string, PyFit::Parameter*> parameters;
            for (auto pair: _observableDataPairs)
            {
                pair.first->queryParameters(parameters);
            }
            // Choose method upon creation between:
            // kMigrad, kSimplex, kCombined, 
            // kScan, kFumili
            ROOT::Minuit2::Minuit2Minimizer min ( ROOT::Minuit2::kMigrad );

            min.SetMaxFunctionCalls(1000000);
            min.SetMaxIterations(100000);
            min.SetTolerance(0.001);

            ROOT::Math::Functor f(
                [](const double *x) -> double 
                {
                    return std::pow(x[0]-2,2);
                },
                1
            ); 

            min.SetFunction(f);

            // Set the free variables to be minimized!
            min.SetVariable(0,"x",0, 0.01); //num, 
            //min.SetVariable(1,"y",variable[1], step[1]);

            min.Minimize(); 

            const double *xs = min.X();
            std::cout<<xs[0]<<std::endl;

        }
        
        virtual ~Fit()
        {
        }
        
    ClassDef(Fit, 1)
};

}

#endif
