#ifndef __BTAGWEIGHTS_H__
#define __BTAGWEIGHTS_H__

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <functional>

namespace BWGHT
{

struct Jet
{
    double discriminatorValue;
    unsigned int flavor;
    double pt;
    double eta;
    
    
    Jet(double discriminatorValue=0, unsigned int flavor = 0, double pt = 0.0, double eta = 0.0):
        discriminatorValue(discriminatorValue),
        flavor(flavor),
        pt(pt),
        eta(eta)
    {
    }
};

struct SYS
{
    enum TYPE
    {
        NOMINAL=0,
        BC_UP=1,
        BC_DOWN=2,
        L_UP=3,
        L_DOWN=4
    };
};

class EfficiencyFunction
{
    public:
        virtual double getEfficiency(const Jet& jet, SYS::TYPE=SYS::NOMINAL) const = 0;
        virtual EfficiencyFunction* clone() const = 0;
};

class ScaleFactorFunction
{
    public:
        virtual double getScaleFactor(const Jet& jet, SYS::TYPE=SYS::NOMINAL) const = 0;
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
        
        virtual double getEfficiency(const Jet& jet, SYS::TYPE=SYS::NOMINAL) const
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
        
        virtual double getScaleFactor(const Jet& jet, SYS::TYPE=SYS::NOMINAL) const
        {
            return _scaleFactor;
        }
        
        virtual ScaleFactorFunction* clone() const
        {
            return new ConstScaleFactorFunction(_scaleFactor);
        }
};


class LambdaScaleFactorFunction:
    public ScaleFactorFunction
{
    private:
        std::function<double(const Jet&, SYS::TYPE)> _fct;
    public:
        LambdaScaleFactorFunction(const std::function<double(const Jet&, SYS::TYPE)>& fct):
            _fct(fct)
        {
        }
        
        virtual double getScaleFactor(const Jet& jet, SYS::TYPE sys=SYS::NOMINAL) const
        {
            return _fct(jet,sys);
        }
        
        virtual ScaleFactorFunction* clone() const
        {
            return new LambdaScaleFactorFunction(_fct);
        }
};

class WorkingPoint
{
    private:
        double _discriminatorValue;
        const EfficiencyFunction* _efficiencyFunction;
        const ScaleFactorFunction* _scaleFactorFunction;
    
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
        }
        
        WorkingPoint(const WorkingPoint& workingPoint):
            _discriminatorValue(workingPoint._discriminatorValue),
            _efficiencyFunction(workingPoint._efficiencyFunction),
            _scaleFactorFunction(workingPoint._scaleFactorFunction)
        {
        }
        
        inline void setDiscriminatorValue(double discriminatorValue)
        {
            _discriminatorValue=discriminatorValue;
        }
        
        void setEfficiencyFunction(const EfficiencyFunction* fct)
        {
            _efficiencyFunction=fct;
        }
        
        void setScaleFactorFunction(const ScaleFactorFunction* fct)
        {
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
            WorkingPoint defaultMinimumWP(-1000.0);
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
            std::vector<std::vector<const Jet*>> jetsPerWorkingPoint(_workingPoints.size()-1);
            
            //group jets according to discriminator values of the workingpoints
            //[0=wp0...wp1,wp1...wp2,...,wp-2...wp-1,wp-1...1]
            for (unsigned int ijet = 0; ijet < jets.size(); ++ijet)
            {
                for (unsigned int iwp = 0; iwp < (_workingPoints.size()-1); ++iwp)
                {
                    if (jets[ijet].discriminatorValue>_workingPoints[iwp].getDiscriminatorValue() and jets[ijet].discriminatorValue<_workingPoints[iwp+1].getDiscriminatorValue())
                    {
                        jetsPerWorkingPoint[iwp].push_back(&jets[ijet]);
                        break;
                    }
                }
            }
            /*
            for (unsigned int iwp = 0; iwp < (_workingPoints.size()-1); ++iwp)
            {
                std::cout<<iwp<<": ["<<_workingPoints[iwp].getDiscriminatorValue()<<";"<<_workingPoints[iwp+1].getDiscriminatorValue()<<"] = ";
                for (unsigned int ijet = 0; ijet<jetsPerWorkingPoint[iwp].size();++ijet)
                {
                    std::cout<<jetsPerWorkingPoint[iwp][ijet]->discriminatorValue<<",";
                }
                std::cout<<std::endl;
            }
            */
            double weightMC = 1.0;
            double weightData = 1.0;
            for (unsigned int iwp=0; iwp<(_workingPoints.size()-1); ++iwp)
            {
                const WorkingPoint& wp1 = _workingPoints[iwp];
                const WorkingPoint& wp2 = _workingPoints[iwp+1];
                
                //std::cout<<"WP"<<iwp<<" : ["<<wp1.getDiscriminatorValue()<<","<<wp2.getDiscriminatorValue()<<"]"<<std::endl;
                
                for (unsigned int ijet = 0; ijet<jetsPerWorkingPoint[iwp].size();++ijet)
                {
                    
                    const Jet* jet = jetsPerWorkingPoint[iwp][ijet];
                    //std::cout<<"   jet"<<ijet<<" : d="<<jet->discriminatorValue<<std::endl;
                    
                    double mcEfficiency1 = wp1.getEfficiencyFunction()->getEfficiency(*jet,systematic);
                    double dataSF1 = wp1.getScaleFactorFunction()->getScaleFactor(*jet,systematic);
                    
                    double mcEfficiency2 = wp2.getEfficiencyFunction()->getEfficiency(*jet,systematic);
                    double dataSF2 = wp2.getScaleFactorFunction()->getScaleFactor(*jet,systematic);
                    
                    if ((mcEfficiency1-mcEfficiency2)<=0)
                    {
                        throw std::runtime_error("Found equal or higher efficiency at tighter working point. Check the efficiency functions!");
                    }
                    if ((dataSF1*mcEfficiency1-dataSF2*mcEfficiency2)<0)
                    {
                        throw std::runtime_error("Found negative b-tagging weight. Check the SF functions!");
                    }
                    
                    //std::cout<<"     eff@wp"<<iwp<<" - eff@wp"<<(iwp+1)<<" = "<<mcEfficiency1<<" - "<<mcEfficiency2<<" = "<<mcEfficiency1-mcEfficiency2<<std::endl; 
                    //std::cout<<"     sf*eff@wp"<<iwp<<" - sf*eff@wp"<<(iwp+1)<<" = "<<mcEfficiency1*dataSF1<<" - "<<mcEfficiency2*dataSF2<<" = "<<mcEfficiency1*dataSF1-mcEfficiency2*dataSF2<<std::endl; 
                    //std::cout<<"     w = "<<(mcEfficiency1*dataSF1-mcEfficiency2*dataSF2)/(mcEfficiency1-mcEfficiency2)<<std::endl;
                    
                    weightMC*=(mcEfficiency1-mcEfficiency2);
                    weightData*=(dataSF1*mcEfficiency1-dataSF2*mcEfficiency2);
                }
            }
            
            return weightData/weightMC;
        }
};

}

#endif

