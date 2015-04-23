#ifndef __HMC_H__
#define __HMC_H__

#include <functional>
#include <array>
#include <random>
#include <sstream>
#include <iomanip>

#include "vdtMath.h"

template<unsigned int N>
class HMC
{
    public:
        class Vector
        {
            private:
                std::vector<double*> _elements;
            public:
                Vector():
                    _elements(N)
                {
                    for (unsigned int i=0; i<N;++i)
                    {
                        _elements[i]=new double(0.0);
                    }
                }
                
                Vector(const Vector& vector):
                    _elements(N)
                {
                    for (unsigned int i=0; i<N;++i)
                    {
                        _elements[i]=new double(*vector._elements[i]);
                    }
                }
                
                Vector& operator=(const Vector& vector)
                {
                    for (unsigned int i=0; i<N;++i)
                    {
                        *_elements[i]=*vector._elements[i];
                    }
                    return *this;
                }
                
                Vector(Vector&& vector):
                    _elements(std::move(vector._elements))
                {
                }
                
                Vector& operator=(Vector&& vector)
                {
                    for (unsigned int i=0; i<N;++i)
                    {
                        delete _elements[i];
                        _elements[i]=vector._elements[i];
                        vector._elements[i]=nullptr;
                    }
                    vector._elements.clean();
                    return *this;
                }
                
                inline double& operator[](const unsigned int index)
                {
                    return *(_elements[index]);
                }
                
                inline double* address(const unsigned int index)
                {
                    return _elements[index];
                }
                
                inline const double& operator[](const unsigned int index) const
                {
                    return *_elements[index];
                }
                
                
                std::string toString() const
                {
                    std::stringstream ss;
                    for (unsigned int i=0; i<N; ++i)
                    {
                        ss<<std::setw(2)<<i<<"="<<std::setw(5)<<std::setiosflags(std::ios::fixed) << std::setprecision(2)<<(*this)[i]<<", ";
                    }
                    return std::move(ss.str());
                }
                
                ~Vector()
                {
                    /*
                    for (unsigned int i=0; i<_elements.size();++i)
                    {
                        if (_elements[i])
                        {
                            delete _elements[i];
                        }
                    }
                    _elements.clear();
                    */
                }
                
        };
    
    
        
        typedef std::function<double(const Vector&)> Function;
        typedef std::function<double(unsigned int direction,const Vector&)> ForceFunction;
        

    protected:
        std::random_device randomDevice;
        std::mt19937 randomGenerator;
        std::normal_distribution<double> normalDistribution;
        std::uniform_real_distribution<double> uniformDistribution;
        
        unsigned int accepted;
        unsigned int rejected;
        
    public:
        HMC():
            randomGenerator(randomDevice()),
            normalDistribution(0,1),
            uniformDistribution(0,1),
            accepted(0),
            rejected(0)
        {
        }
        void leapfrog(
                ForceFunction force, 
                Vector& position, 
                Vector& momentum, 
                unsigned int L, 
                const std::array<double,N>& epsilon
            )
        {
            
            for (unsigned int idim=0;idim<N;++idim)
            {
                momentum[idim]=momentum[idim]-0.5*epsilon[idim]*force(idim,position);
            }
            /*
                for parallel execution one would 
                    1. allocate several mem blocks 
                    2. caluclate the force in parallel (max up to N)
                    3. sync after each iteration using events
            */
            for (unsigned int iteration=0; iteration<L;++iteration)
            {
                for (unsigned int idim=0;idim<N;++idim)
                {
                    momentum[idim]=momentum[idim]+epsilon[idim]*force(idim,position);
                    position[idim]=position[idim]+epsilon[idim]*momentum[idim];
                }
            }
            for (unsigned int idim=0;idim<N;++idim)
            {
                momentum[idim]=momentum[idim]+0.5*epsilon[idim]*force(idim,position);
            }
        }
        
        double getEfficiency()
        {
            return 1.0*accepted/(1.0*(rejected+accepted));
        }
        
        void sample(Function function, Vector& startPosition, unsigned int L, const std::array<double,N>& epsilon)
        {
            Function potential = [function](Vector position){ return -vdt::fast_log(function(position));};
            ForceFunction force = [potential](unsigned int direction, const Vector& vector)
            {   
                Vector forward(vector);
                Vector backward(vector);
                forward[direction]+=0.00001;
                backward[direction]-=0.00001;
                return -(potential(forward)-potential(backward))/0.00002;
            };
            Vector momentum;
            for (unsigned int idim=0; idim<N;++idim)
            {
                momentum[idim]=normalDistribution(randomGenerator);
            }
            Vector new_momentum(momentum);
            Vector new_position(startPosition);
            //std::cout<<"start begin: "<<startPosition.toString().c_str()<<std::endl;
            leapfrog(force,new_position,new_momentum,L,epsilon);
            
            double u=potential(startPosition);
            //std::cout<<"start: "<<startPosition.toString().c_str()<<std::endl;
            double new_u=potential(new_position);
            //std::cout<<"new: "<<new_position.toString().c_str()<<std::endl;
            double k=0.0;
            double new_k=0.0;
            for (unsigned int idim=0; idim<N;++idim)
            {
                k+=0.5*momentum[idim]*momentum[idim];
                new_k+=0.5*new_momentum[idim]*new_momentum[idim];
            }
            if (uniformDistribution(randomGenerator)<vdt::fast_exp(u-new_u+k-new_k))
            {
                for (unsigned int idim=0; idim<N;++idim)
                {
                    startPosition[idim]=new_position[idim];
                }
                accepted+=1;
                //printf("accept\n");
            }
            else
            {
                rejected+=1;
                //printf("reject\n");
            }
           
        }

};

#endif

