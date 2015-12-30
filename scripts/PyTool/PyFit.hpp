#ifndef __PYFIT_H__
#define __PYFIT_H__

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <limits>

class PyFit
{


    public:
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
    
    public:

        class Component
        {
            public:
                virtual void queryParameters(std::map<std::string, PyFit::Parameter*>& parameters) const = 0;
                virtual void getPrediction(TH1D* hist) const = 0;
                
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
                
                virtual void getPrediction(TH1D* hist) const
                {
                    
                    double combinedSF = 1.0;
                    for (const PyFit::Parameter* parameter: _parameters)
                    {
                        combinedSF*=parameter->getScaleFactor();
                    }
                    hist->Add(_hist,combinedSF);
                    
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
                
                void getPrediction(TH1D* hist)
                {
                }
                
                void queryParameters(std::map<std::string, PyFit::Parameter*>& parameters) const
                {
                    for (PyFit::Component* comp: _components)
                    {
                        comp->queryParameters(parameters);
                    }
                }
                
                double logLikelihood(TH1D* data)
                {
                    
                    return 0;
                }
                
                virtual ~Observable()
                {
                }
                
            ClassDef(PyFit::Observable, 1)
        };
        
    
    private:
        std::vector<PyFit::Observable*> _observables;
        
    
    public:
        PyFit()
        {
        }
        
        virtual ~PyFit()
        {
        }
        
    ClassDef(PyFit, 1)
};

#endif
