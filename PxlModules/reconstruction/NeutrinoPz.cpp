#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <complex>
#include <cmath>

static pxl::Logger logger("NeutrinoPz");

class NeutrinoPz : public pxl::Module
{
    private:
    pxl::Source* _output;

    std::string _inputEventViewName;
    std::string _metName;
    std::string _leptonName;
    std::string _neutrinoName;
    

    public:
    NeutrinoPz() :
        Module(),
        _inputEventViewName("Reconstructed"),
        _metName("MET"),
        _leptonName("TightMuon"),
        _neutrinoName("Neutrino")

    {
        addSink("input", "Input");
        _output = addSource("output", "output");

        addOption("event view","name of the event view",_inputEventViewName);
        addOption("met name","name of the MET",_metName);
        addOption("lepton name","name of the lepton",_leptonName);
        addOption("neutrino name","name of the neutrino",_neutrinoName);

    }

    ~NeutrinoPz()
    {
    }

    // every Module needs a unique type
    static const std::string &getStaticType()
    {
        static std::string type ("NeutrinoPz");
        return type;
    }

    // static and dynamic methods are needed
    const std::string &getType() const
    {
        return getStaticType();
    }

    bool isRunnable() const
    {
        // this module does not provide events, so return false
        return false;
    }

    void initialize() throw (std::runtime_error)
    {
    }

    void beginJob() throw (std::runtime_error)
    {
        getOption("event view",_inputEventViewName);
        getOption("lepton name",_leptonName);
        getOption("met name",_metName);
        getOption("neutrino name",_neutrinoName);
    }

    void endJob()
    {
    }
    
    template <class T>
    std::vector< T > const EquationSolve(const T & a, const T & b,const T & c,const T & d)
    {
      std::vector<T> result;


      std::complex<T> x1;
      std::complex<T> x2;
      std::complex<T> x3;

      if (a != 0) {
        
        T q = (3*a*c-b*b)/(9*a*a);
        T r = (9*a*b*c - 27*a*a*d - 2*b*b*b)/(54*a*a*a);
        T Delta = q*q*q + r*r;

        std::complex<T> s;
        std::complex<T> t;

        T rho=0;
        T theta=0;
        
        if( Delta<=0){
          rho = sqrt(-(q*q*q));

          theta = acos(r/rho);

          s = std::polar<T>(sqrt(-q),theta/3.0);
          t = std::polar<T>(sqrt(-q),-theta/3.0);
        }
        
        if(Delta>0){
          s = std::complex<T>(cbrt(r+sqrt(Delta)),0);
          t = std::complex<T>(cbrt(r-sqrt(Delta)),0);
        }
      
        std::complex<T> i(0,1.0);
        
        
         x1 = s+t+std::complex<T>(-b/(3.0*a),0);
         x2 = (s+t)*std::complex<T>(-0.5,0)-std::complex<T>(b/(3.0*a),0)+(s-t)*i*std::complex<T>(sqrt(3)/2.0,0);
         x3 = (s+t)*std::complex<T>(-0.5,0)-std::complex<T>(b/(3.0*a),0)-(s-t)*i*std::complex<T>(sqrt(3)/2.0,0);

        if(fabs(x1.imag())<0.0001)result.push_back(x1.real());
        if(fabs(x2.imag())<0.0001)result.push_back(x2.real());
        if(fabs(x3.imag())<0.0001)result.push_back(x3.real());

        return result;
      }
      else{return result;}


      return result;
    }


    //..................................................................................................
    /*
    * Modification of code by Orso.
    * WARNING! Use only pz component of the returned 4-momenta as the function adjusts the transverse
    * component.
    */


    void solveNu4Momentum(pxl::Particle* neutrino, const pxl::LorentzVector& lepton, const float& metpx, const float& metpy)
    {

        //solve real solution case
        bool useNegativeDeltaSolutions_ = true;
        //solve complex soultion case
        bool usePositiveDeltaSolutions_ = true;

        //in case of real solution use the abs min pz
        bool usePzMinusSolutions_ = false;
        bool usePzPlusSolutions_ = false;
        bool usePzAbsValMinimumSolutions_ = true;


        //vary px,py to find the solution in complex case
        bool usePxMinusSolutions_ = true;
        bool usePxPlusSolutions_ = true;

        //set root=0 in complex case
        bool useMetForNegativeSolutions_ = false;

        double const mW = 80.38;

        // double Wmt = sqrt(pow(lepton.et()+MET.pt(),2) - pow(lepton.getPx()+MET.getPx(),2) - pow(lepton.getPy()+MET.getPy(),2) );

        double MisET2 = (metpx*metpx + metpy*metpy);
        double mu = (mW*mW)/2 + metpx*lepton.getPx() + metpy*lepton.getPy();
        double a = (mu*lepton.getPz())/(lepton.getE()*lepton.getE() - lepton.getPz()*lepton.getPz());
        double a2 = pow(a,2.);
        double b = (pow(lepton.getE(),2.)*(MisET2) - pow(mu,2.))/(pow(lepton.getE(),2) - pow(lepton.getPz(),2));
        double pz1(0),pz2(0),pznu(0);
        //int nNuSol(0);

        pxl::LorentzVector p4nu_rec;
        pxl::LorentzVector p4lep_rec;

        p4lep_rec.setXYZ(lepton.getPx(),lepton.getPy(),lepton.getPz());
        p4lep_rec.setE(lepton.getE());

        neutrino->setUserRecord("radicand",a2-b);
        //two real solutions exist
        if((a2-b > 0)  && usePositiveDeltaSolutions_)
        {
            neutrino->setUserRecord("realsolution",true);


            double root = sqrt(a2-b);
            pz1 = a + root;
            pz2 = a - root;
            //nNuSol = 2;
            if(usePzPlusSolutions_)
            {
                pznu = pz1;
            }
            if(usePzMinusSolutions_)
            {
                pznu = pz2;
            }
            if(usePzAbsValMinimumSolutions_)
            {
                pznu = pz1;
                if(fabs(pz1)>fabs(pz2))
                {
                    pznu = pz2;
                }
            }

            double Enu = sqrt(MisET2 + pznu*pznu);
            p4nu_rec.setXYZ(metpx, metpy, pznu);
            p4nu_rec.setE(Enu);
            neutrino->setVector(p4nu_rec);

        }
        //two complex solutions exist
        else if ((a2-b < 0) && useNegativeDeltaSolutions_)
        {
            neutrino->setUserRecord("realsolution",false);
            // double xprime = sqrt(mW;

            double ptlep = lepton.getPt(),pxlep=lepton.getPx(),pylep=lepton.getPy();

            double EquationA = 1;
            double EquationB = -3*pylep*mW/(ptlep);
            double EquationC = mW*mW*(2*pylep*pylep)/(ptlep*ptlep)+mW*mW-4*pxlep*pxlep*pxlep*metpx/(ptlep*ptlep)-4*pxlep*pxlep*pylep*metpy/(ptlep*ptlep);
            double EquationD = 4*pxlep*pxlep*mW*metpy/(ptlep)-pylep*mW*mW*mW/ptlep;

            std::vector<long double> solutions = EquationSolve<long double>((long double)EquationA,(long double)EquationB,(long double)EquationC,(long double)EquationD);

            std::vector<long double> solutions2 = EquationSolve<long double>((long double)EquationA,-(long double)EquationB,(long double)EquationC,-(long double)EquationD);


            double deltaMin = 14000*14000;
            double zeroValue = -mW*mW/(4*pxlep);
            double minPx=0;
            double minPy=0;

            // std::cout<<"a "<<EquationA << " b " << EquationB <<" c "<< EquationC <<" d "<< EquationD << std::endl;

            if(usePxMinusSolutions_)
            {
                for( int i =0; i< (int)solutions.size();++i)
                {
                    if(solutions[i]<0 )
                    {
                        continue;
                    }
                    double p_x = (solutions[i]*solutions[i]-mW*mW)/(4*pxlep);
                    double p_y = ( mW*mW*pylep + 2*pxlep*pylep*p_x -mW*ptlep*solutions[i])/(2*pxlep*pxlep);
                    double Delta2 = (p_x-metpx)*(p_x-metpx)+(p_y-metpy)*(p_y-metpy);

                    // std::cout<<"intermediate solution1 met x "<<metpx << " min px " << p_x <<" met y "<<metpy <<" min py "<< p_y << std::endl;

                    if(Delta2< deltaMin && Delta2 > 0)
                    {
                        deltaMin = Delta2;
                        minPx=p_x;
                        minPy=p_y;
                    }
                    // std::cout<<"solution1 met x "<<metpx << " min px " << minPx <<" met y "<<metpy <<" min py "<< minPy << std::endl;
                }
            }

            if(usePxPlusSolutions_)
            {
                for( int i =0; i< (int)solutions2.size();++i)
                {
                    if(solutions2[i]<0 )
                    {
                        continue;
                    }
                    double p_x = (solutions2[i]*solutions2[i]-mW*mW)/(4*pxlep);
                    double p_y = ( mW*mW*pylep + 2*pxlep*pylep*p_x +mW*ptlep*solutions2[i])/(2*pxlep*pxlep);
                    double Delta2 = (p_x-metpx)*(p_x-metpx)+(p_y-metpy)*(p_y-metpy);
                    // std::cout<<"intermediate solution2 met x "<<metpx << " min px " << minPx <<" met y "<<metpy <<" min py "<< minPy << std::endl;
                    if(Delta2< deltaMin && Delta2 > 0)
                    {
                        deltaMin = Delta2;
                        minPx=p_x;
                        minPy=p_y;
                    }
                    // std::cout<<"solution2 met x "<<metpx << " min px " << minPx <<" met y "<<metpy <<" min py "<< minPy << std::endl;
                }
            }

            double pyZeroValue= ( mW*mW*pxlep + 2*pxlep*pylep*zeroValue);
            double delta2ZeroValue= (zeroValue-metpx)*(zeroValue-metpx) + (pyZeroValue-metpy)*(pyZeroValue-metpy);

            if(deltaMin<14000*14000)
            {
                // else std::cout << " test " << std::endl;

                if(delta2ZeroValue < deltaMin)
                {
                    deltaMin = delta2ZeroValue;
                    minPx=zeroValue;
                    minPy=pyZeroValue;
                }

                // std::cout<<" MtW2 from min py and min px "<< sqrt((minPy*minPy+minPx*minPx))*ptlep*2 -2*(pxlep*minPx + pylep*minPy) <<std::endl;
                /// ////Y part

                double mu_Minimum = (mW*mW)/2 + minPx*pxlep + minPy*pylep;
                double a_Minimum = (mu_Minimum*lepton.getPz())/(lepton.getE()*lepton.getE() - lepton.getPz()*lepton.getPz());
                pznu = a_Minimum;

                if(!useMetForNegativeSolutions_)
                {
                    double Enu = sqrt(minPx*minPx+minPy*minPy + pznu*pznu);
                    p4nu_rec.setXYZ(minPx, minPy, pznu);
                    p4nu_rec.setE(Enu);
                }
                else
                {
                    pznu = a;
                    double Enu = sqrt(metpx*metpx+metpy*metpy + pznu*pznu);
                    p4nu_rec.setXYZ(metpx, metpy, pznu);
                    p4nu_rec.setE(Enu);
                }
                neutrino->setVector(p4nu_rec);
            }
        }
    }
    
    bool analyse(pxl::Sink *sink) throw (std::runtime_error)
    {
        try
        {
            pxl::Event *event  = dynamic_cast<pxl::Event *> (sink->get());
            if (event)
            {
                std::vector<pxl::EventView*> eventViews;
                event->getObjectsOfType(eventViews);
                for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                {
                    pxl::EventView* eventView = eventViews[ieventView];
                    if (eventView->getName()==_inputEventViewName)
                    {
                        std::vector<pxl::Particle*> particles;
                        eventView->getObjectsOfType(particles);
                        pxl::Particle* met=0;
                        pxl::Particle* lepton=0;
                        pxl::Particle* neutrino=0;
                        for (unsigned iparticle=0; iparticle<particles.size();++iparticle)
                        {
                            pxl::Particle* particle = particles[iparticle];
                            if (met==0 &&(particle->getName()==_metName))
                            {
                                met=particle;
                            }
                            if (lepton==0 &&(particle->getName()==_leptonName))
                            {
                                lepton=particle;
                            }
                        }
                        if (met!=0 && lepton!=0)
                        {
                            neutrino=eventView->create<pxl::Particle>();
                            neutrino->setName(_neutrinoName);
                            solveNu4Momentum(neutrino,lepton->getVector(),met->getPx(),met->getPy());
                            pxl::Particle p1;
                            pxl::Particle p2;
                            p1.getVector()+=met->getVector();
                            p1.getVector()+=lepton->getVector();
                            p2.getVector()+=neutrino->getVector();
                            p2.getVector()+=lepton->getVector();
                            neutrino->setUserRecord("mtw_beforePzSolution",sqrt(p1.getMass()*p1.getMass()+p1.getPx()*p1.getPx()+p1.getPy()*p1.getPy()));
                            neutrino->setUserRecord("mtw_afterPzSolution",sqrt(p2.getMass()*p2.getMass()+p2.getPx()*p2.getPx()+p2.getPy()*p2.getPy()));
                            //neutrino->setUserRecord("wmass_afterPzSolution",p2.getMass());
                        }
                    }
                }

                _output->setTargets(event);
                return _output->processTargets();
            }
        }
        catch(std::exception &e)
        {
            throw std::runtime_error(getName()+": "+e.what());
        }
        catch(...)
        {
            throw std::runtime_error(getName()+": unknown exception");
        }

        logger(pxl::LOG_LEVEL_ERROR , "Analysed event is not an pxl::Event !");
        return false;
    }

    void shutdown() throw(std::runtime_error)
    {
    }

    void destroy() throw (std::runtime_error)
    {
        delete this;
    }
};

PXL_MODULE_INIT(NeutrinoPz)
PXL_PLUGIN_INIT

