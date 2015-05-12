#ifndef __FOXWOLFRAM_H__
#define __FOXWOLFRAM_H__

#include "pxl/core.hh"
#include "Math/SpecFuncMathMore.h"

#include <cmath>

//loosely based on http://arxiv.org/abs/1212.4436

class FoxWolfram
{
    protected:
        std::vector<pxl::LorentzVector> _eventVectors;
        double totalEnergy;
    public:
        enum WeightType
        {
            SHAT,PT,ETA,PSUM,PZ,ONE
        };
    
        FoxWolfram(const std::vector<pxl::LorentzVector>& eventVectors):
            _eventVectors(eventVectors)
        {
        }
        
        
        
        double getMoment(WeightType wType, unsigned int order)
        {
            switch (wType)
            {
                case SHAT:
                {
                    return getMomentShat(order);
                }
                case PT:
                {
                    return getMomentPt(order);
                }
                case ETA:
                {
                    return getMomentEta(order);
                }
                case PSUM:
                {
                    return getMomentPsum(order);
                }
                case PZ:
                {
                    return getMomentPz(order);
                }
                case ONE:
                {
                    return getMomentOne(order);
                } 
            }
            return -1;
        }
        
        double cosTheta(const pxl::LorentzVector& v1, const pxl::LorentzVector& v2)
        {
            //return std::cos(v1.getTheta())*std::cos(v2.getTheta())+std::sin(v1.getTheta())*std::sin(v2.getTheta())*std::cos(v1.getPhi()-v2.getPhi());
            return (v1.getX()*v2.getX()+v1.getY()*v2.getY()+v1.getZ()*v2.getZ())/sqrt(v1.getMag2()*v2.getMag2());
        }
       
        
        double getMomentShat(unsigned int order) 
        {
            double sum = 0.0;
            pxl::LorentzVector shat(0,0,0,0);
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                shat+=_eventVectors[i];
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=_eventVectors[i].getP()*_eventVectors[j].getP()*ROOT::Math::legendre(order,angle);
                }
            }
            
            return sum/shat.getP()/shat.getP();
        }
        
        double getMomentPt(unsigned int order) 
        {
            double sum = 0.0;
            double norm = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                norm+=_eventVectors[i].getPt();
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=_eventVectors[i].getPt()*_eventVectors[j].getPt()*ROOT::Math::legendre(order,angle);
                }
            }
            
            return sum/norm/norm;
        }
        
        double getMomentEta(unsigned int order) 
        {
            double avgEta = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                avgEta+=_eventVectors[i].getEta();
            }
            avgEta/= _eventVectors.size();
        
            double norm = 0.0;
            double sum = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                norm+=1.0/(fabs(_eventVectors[i].getEta()-avgEta));
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=1.0/(fabs(_eventVectors[i].getEta()-avgEta)*(_eventVectors[j].getEta()-avgEta))*ROOT::Math::legendre(order,angle);
                }
            }
            return sum/norm/norm;
        }
        
        double getMomentPsum(unsigned int order) 
        {
            double sum = 0.0;
            double norm = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                norm+=_eventVectors[i].getP();
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=_eventVectors[i].getP()*_eventVectors[j].getP()*ROOT::Math::legendre(order,angle);
                }
            }
            
            return sum/norm/norm;
        }
        
        double getMomentPz(unsigned int order) 
        {
            double sum = 0.0;
            double norm = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {  
                norm+=_eventVectors[i].getPz();
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=_eventVectors[i].getPz()*_eventVectors[j].getPz()*ROOT::Math::legendre(order,angle);
                }
            }

            return sum/norm/norm;
        }
        
        double getMomentOne(unsigned int order) 
        {
            double sum = 0.0;
            for (unsigned int i = 0; i < _eventVectors.size(); ++i)
            {
                for (unsigned int j = 0; j < _eventVectors.size(); ++j)
                {
                    double angle = cosTheta(_eventVectors[i],_eventVectors[j]);
                    sum+=ROOT::Math::legendre(order,angle);
                }
            }
            
            return sum;
        }
};

#endif

