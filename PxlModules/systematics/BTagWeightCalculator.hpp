#ifndef __BTAGWEIGHTS_H__
#define __BTAGWEIGHTS_H__

#include "Math/LorentzVector.h"
#include "TMath.h"

#include <stdexcept>
#include <algorithm>

namespace BWGHT
{

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

struct Jet
{
    double discriminatorValue;
    LorentzVector vector;
    
    Jet(double discriminatorValue=0,const LorentzVector& vector=LorentzVector()):
        discriminatorValue(discriminatorValue),
        vector(vector)
    {
    }
    
    inline double getDiscriminatorValue() const
    {
        return discriminatorValue;
    }
};

struct SYS
{
    enum TYPE
    {
        NOMINAL=0,
        UP=1,
        DOWN=2  
    };
};

class EfficiencyFunction
{
    public:
        virtual double getEfficiency(const Jet& jet,SYS::TYPE=SYS::NOMINAL) const = 0;
        virtual EfficiencyFunction* clone() const = 0;
};

class ScaleFactorFunction
{
    public:
        virtual double getScaleFactor(const Jet& jet,SYS::TYPE=SYS::NOMINAL) const = 0;
        virtual ScaleFactorFunction* clone() const = 0;
};

class ConstEfficiencyFunction:
    public EfficiencyFunction
{
    private:
        double _efficiency;
    public:
        ConstEfficiencyFunction(double efficiency):
            _efficiency(efficiency)
        {
        }
        
        virtual double getEfficiency(const Jet& jet,SYS::TYPE=SYS::NOMINAL) const
        {
            return _efficiency;
        }
        
        virtual EfficiencyFunction* clone() const
        {
            return new ConstEfficiencyFunction(_efficiency);
        }
};


class ConstScaleFactorFunction:
    public ScaleFactorFunction
{
    private:
        double _scaleFactor;
    public:
        ConstScaleFactorFunction(double scaleFactor):
            _scaleFactor(scaleFactor)
        {
        }
        
        virtual double getScaleFactor(const Jet& jet,SYS::TYPE=SYS::NOMINAL) const
        {
            return _scaleFactor;
        }
        
        virtual ScaleFactorFunction* clone() const
        {
            return new ConstScaleFactorFunction(_scaleFactor);
        }
};

class WorkingPoint
{
    private:
        double _discriminatorValue;
        EfficiencyFunction* _efficiencyFunction;
        ScaleFactorFunction* _scaleFactorFunction;
    
    public:
        
        struct less
        {
            bool operator()(const WorkingPoint& wp1, const WorkingPoint& wp2) const
            {
                return wp1.getDiscriminatorValue()<wp2.getDiscriminatorValue();
            }
        };
        
    
        WorkingPoint(double discriminatorValue):
            _discriminatorValue(discriminatorValue),
            _efficiencyFunction(nullptr),
            _scaleFactorFunction(nullptr)
        {
            if (_discriminatorValue<0)
            {
                throw std::runtime_error("Working point discriminator value needs to be >= 0");
            }
        }
        
        WorkingPoint(const WorkingPoint& workingPoint):
            _discriminatorValue(workingPoint._discriminatorValue),
            _efficiencyFunction(workingPoint._efficiencyFunction->clone()),
            _scaleFactorFunction(workingPoint._scaleFactorFunction->clone())
        {
        }
        
        inline void setDiscriminatorValue(double discriminatorValue)
        {
            if (_discriminatorValue<0)
            {
                throw std::runtime_error("Working point discriminator value needs to be >= 0");
            }
            _discriminatorValue=discriminatorValue;
        }
        
        void setEfficiencyFunction(EfficiencyFunction* fct)
        {
            if (_efficiencyFunction)
            {
                delete _efficiencyFunction;
            }
            _efficiencyFunction=fct;
        }
        
        void setScaleFactorFunction(ScaleFactorFunction* fct)
        {
            if (_scaleFactorFunction)
            {
                delete _scaleFactorFunction;
            }
            _scaleFactorFunction=fct;
        }
        
        inline double getDiscriminatorValue() const
        {
            return _discriminatorValue;
        }
        
        inline const EfficiencyFunction* getEfficiencyFunction() const
        {  
            return _efficiencyFunction;
        }
        
        inline const ScaleFactorFunction* getScaleFactorFunction() const
        {   
            return _scaleFactorFunction;
        }
        
};

class BTagWeightCalculator
{
    protected:
        std::vector<WorkingPoint> _workingPoints;
    public:
        BTagWeightCalculator()
        {
            WorkingPoint defaultMinimumWP(0.0);
            defaultMinimumWP.setEfficiencyFunction(new ConstEfficiencyFunction(1.0));
            defaultMinimumWP.setScaleFactorFunction(new ConstScaleFactorFunction(1.0));
            addWorkingPoint(defaultMinimumWP);
            
            WorkingPoint defaultMaximumWP(1.0);
            defaultMaximumWP.setEfficiencyFunction(new ConstEfficiencyFunction(0.0));
            defaultMaximumWP.setScaleFactorFunction(new ConstScaleFactorFunction(0.0));;
            addWorkingPoint(defaultMaximumWP);
        }
        
        void addWorkingPoint(const WorkingPoint& workingPoint)
        {
            if (workingPoint.getDiscriminatorValue()<0)
            {
                throw std::runtime_error("Working point discriminator value needs to be >= 0");
            }
            if (workingPoint.getEfficiencyFunction()==nullptr)
            {
                throw std::runtime_error("Efficiency function not set for working point:"+std::to_string(workingPoint.getDiscriminatorValue()));
            }
            if (workingPoint.getScaleFactorFunction()==nullptr)
            {
                throw std::runtime_error("Scale factor function not set for working point:"+std::to_string(workingPoint.getDiscriminatorValue()));
            }
            _workingPoints.push_back(workingPoint);
            std::sort(_workingPoints.begin(),_workingPoints.end(),WorkingPoint::less());
        }
        
        double getEventWeight(const std::vector<Jet>& jets, SYS::TYPE systematic=SYS::NOMINAL) const
        {
            std::vector<std::vector<const Jet*>> jetsPerWorkingPoint(_workingPoints.size());            std::vector<const Jet*> unassigned;
            
            //group jets according to discriminator values of the workingpoints
            //[0=wp0...wp1,wp1...wp2,...,wp-2...wp-1,wp-1...1]
            //jetsPerWorkingPoint[0] should not include any jets since WP counting starts at index 1
            for (unsigned int ijet = 0; ijet < jets.size(); ++ijet)
            {
                for (unsigned int iwp = 1; iwp < _workingPoints.size(); ++iwp)
                {
                    if (jets[ijet].getDiscriminatorValue()>_workingPoints[iwp-1].getDiscriminatorValue() and jets[ijet].getDiscriminatorValue()<_workingPoints[iwp].getDiscriminatorValue())
                    {
                        jetsPerWorkingPoint[iwp].push_back(&jets[ijet]);
                        break;
                    }
                }
            }
            
            for (unsigned int iwp = 1; iwp < _workingPoints.size(); ++iwp)
            {
                std::cout<<iwp<<": ["<<_workingPoints[iwp-1].getDiscriminatorValue()<<";"<<_workingPoints[iwp].getDiscriminatorValue()<<"] = ";
                for (unsigned int ijet = 0; ijet<jetsPerWorkingPoint[iwp].size();++ijet)
                {
                    std::cout<<jetsPerWorkingPoint[iwp][ijet]->getDiscriminatorValue()<<",";
                }
                std::cout<<"!"<<std::endl;
            }
            
            double weightMC = 1.0;
            double weightData = 1.0;
            for (unsigned int iwp=1; iwp<_workingPoints.size(); ++iwp)
            {
                const WorkingPoint& wp1 = _workingPoints[iwp-1];
                const WorkingPoint& wp2 = _workingPoints[iwp];
                
                for (unsigned int ijet = 0; ijet<jetsPerWorkingPoint[iwp].size();++ijet)
                {
                    const Jet* jet = jetsPerWorkingPoint[iwp][ijet];
                    
                    double mcEfficiency1 = wp1.getEfficiencyFunction()->getEfficiency(*jet,systematic);
                    double dataSF1 = wp1.getScaleFactorFunction()->getScaleFactor(*jet,systematic);
                    
                    double mcEfficiency2 = wp2.getEfficiencyFunction()->getEfficiency(*jet,systematic);
                    double dataSF2 = wp2.getScaleFactorFunction()->getScaleFactor(*jet,systematic);

                    /*
                    std::cout<<"MC: *( eff("<<mcEfficiency1<<"|wp"<<iwp-1<<"="<<wp1.getDiscriminatorValue()<<",jet"<<ijet<<"="<<jet->getDiscriminatorValue()<<") - eff("<<mcEfficiency2<<"|wp"<<iwp<<"="<<wp2.getDiscriminatorValue()<<",jet"<<ijet<<"="<<jet->getDiscriminatorValue()<<") = "<<(mcEfficiency1-mcEfficiency2)<<std::endl;
                    std::cout<<"data: *( eff*sf("<<dataSF1<<"| jet"<<ijet<<": pt="<<jet->vector.Pt()<<",eta="<<jet->vector.Eta()<<") - eff*sf("<<dataSF2<<"| jet"<<ijet<<": pt="<<jet->vector.Pt()<<",eta="<<jet->vector.Eta()<<") ) = "<<(dataSF1*mcEfficiency1-dataSF2*mcEfficiency2)<<std::endl; 
                    std::cout<<"w: *"<<(dataSF1*mcEfficiency1-dataSF2*mcEfficiency2)/(mcEfficiency1-mcEfficiency2)<<std::endl;
                    */
                    if ((mcEfficiency1-mcEfficiency2)<0)
                    {
                        throw std::runtime_error("Found higher efficiency at tighter working point. Check the efficiency functions!");
                    }
                    weightMC*=(mcEfficiency1-mcEfficiency2);
                    weightData*=(dataSF1*mcEfficiency1-dataSF2*mcEfficiency2);
                }
            }
            
            return weightData/weightMC;
        }
};

}

#endif

