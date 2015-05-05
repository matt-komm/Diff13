#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TRandom2.h"
#include "TError.h"

#include "TCanvas.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLine.h"
#include "TMath.h"

#include "hmc.hpp"

#include <iostream>

#include <string>
#include <unordered_set>
#include <vector>
#include <functional>
#include <array>

static pxl::Logger logger("Chi2TopSolver");

static const double sqrt2pi = std::sqrt(2*3.14159265358979323846);

static const std::function<double(double)> logNormal = [] (double x) -> double
{
    if (x<0)
    {
        return 0;
    }
    else
    {
        return vdt::fast_exp(-x/50.0)/(1+vdt::fast_exp((10.0-x)/2.0));
    }
};

struct FitResult
{
    std::array<double,9> xmin;
    std::array<double,9> xerr;
    double chi2;
    bool success;

    inline const pxl::LorentzVector neutrino() const
    {
        const double neutrinoPt = fabs(xmin[0]);
        const double neutrinoPhi = xmin[1];
        const double neutrinoEta = xmin[2];

        const double neutrinoPx = std::cos(neutrinoPhi)*neutrinoPt;
        const double neutrinoPy = std::sin(neutrinoPhi)*neutrinoPt;
        const double neutrinoPz = std::sinh(neutrinoEta)*neutrinoPt;
        const double neutrinoE = std::sqrt(neutrinoPx*neutrinoPx+neutrinoPy*neutrinoPy+neutrinoPz*neutrinoPz);

        pxl::LorentzVector vec(neutrinoPx,neutrinoPy,neutrinoPz,neutrinoE);
        return vec;
    }

    inline const pxl::LorentzVector lepton() const
    {
        const double leptonPt =fabs(xmin[3]);
        const double leptonPhi = xmin[4];
        const double leptonEta = xmin[5];
        const double leptonPx = std::cos(leptonPhi)*leptonPt;
        const double leptonPy = std::sin(leptonPhi)*leptonPt;
        const double leptonPz = std::sinh(leptonEta)*leptonPt;
        const double leptonE = std::sqrt(leptonPx*leptonPx+leptonPy*leptonPy+leptonPz*leptonPz);
        pxl::LorentzVector vec(leptonPx,leptonPy,leptonPz,leptonE);
        return vec;
    }

    inline const pxl::LorentzVector bjet() const
    {
        const double bjetPt = fabs(xmin[6]);
        const double bjetPhi = xmin[7];
        const double bjetEta = xmin[8];

        const double bjetPx = std::cos(bjetPhi)*bjetPt;
        const double bjetPy = std::sin(bjetPhi)*bjetPt;
        const double bjetPz = std::sinh(bjetEta)*bjetPt;
        const double bjetE = std::sqrt(bjetPx*bjetPx+bjetPy*bjetPy+bjetPz*bjetPz);
        pxl::LorentzVector vec(bjetPx,bjetPy,bjetPz,bjetE);
        return vec;
    }
};

class Chi2TopSolver:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        pxl::Source* _outputVetoSource;
        
        std::string _inputEventViewName;
        std::string _leptonName;
        std::string _metName;
        std::unordered_set<std::string> _bJetNames;
        std::unordered_set<std::string> _lightJetNames;
        
    public:
        Chi2TopSolver():
            Module(),
            _inputEventViewName("Reconstructed")
        {
            gErrorIgnoreLevel = kError | kPrint | kInfo | kWarning;
            addSink("input", "input");
            _outputSource = addSource("selected","selected");
            
            addOption("event view","name of the event view",_inputEventViewName);
            addOption("lepton","name of the lepton",_leptonName);
            addOption("met","name of the met",_metName);
            addOption("b-jet","names of the b-jets",std::vector<std::string>{"blub"});
            addOption("light jet","names of the light jets",std::vector<std::string>{"blub"});

        }

        ~Chi2TopSolver()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("Chi2TopSolver");
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
            getOption("lepton",_leptonName);
            getOption("met",_metName);
            
            std::vector<std::string> bJetNames;
            getOption("b-jet",bJetNames);
            for (const std::string& s: bJetNames)
            {
                _bJetNames.insert(s);
            }
            
            std::vector<std::string> lightJetNames;
            getOption("light jet",lightJetNames);
            for (const std::string& s: lightJetNames)
            {
                _lightJetNames.insert(s);
            }
        }
        




        FitResult minimize(
                std::function<double(const double*)> chi2,
                const std::array<double,9> startPoint,
                const char * minName = "Minuit2",
                const char *algoName = "Simplex"
        )
        {
            // create minimizer giving a name and a name (optionally) for the specific
            // algorithm
            // possible choices are:
            //     minName                  algoName
            // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
            //  Minuit2                     Fumili2
            //  Fumili
            //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
            //                              BFGS2, SteepestDescent
            //  GSLMultiFit
            //   GSLSimAn
            //   Genetic
            ROOT::Math::Minimizer* min =
              ROOT::Math::Factory::CreateMinimizer(minName, algoName);

            // set tolerance , etc...
            min->SetMaxFunctionCalls(100000); // for Minuit/Minuit2
            min->SetMaxIterations(10000);  // for GSL
            min->SetTolerance(0.0001);
            //min->SetPrintLevel(-1000);

            ROOT::Math::Functor f(chi2,9);
            double step[9] = {
                startPoint[0]*0.05, //neutrino Pt
                0.05, //neutrino Phi
                0.1, //neutrino Eta

                0.005, //lepton pt
                0.005, //lepton phi
                0.005, //lepton eta

                1.0, //bjet pt
                0.5, //bjet phi
                0.1 //bjet eta
            };
            // starting point


            min->SetFunction(f);

            // Set the free variables to be minimized!

            min->SetVariable(0,"neutrinoPt",startPoint[0], step[0]);
            min->SetVariable(1,"neutrinoPhi",startPoint[1], step[1]);
            min->SetVariable(2,"neutrinoEta",startPoint[2], step[2]);
            min->SetVariable(3,"leptonPt",startPoint[3], step[3]);
            min->SetVariable(4,"leptonPhi",startPoint[4], step[4]);
            min->SetVariable(5,"leptonEta",startPoint[5], step[5]);

            min->SetVariable(6,"bjetPt",startPoint[6], step[6]);
            min->SetVariable(7,"bjetPhi",startPoint[7], step[7]);
            min->SetVariable(8,"bjetEta",startPoint[8], step[8]);

            /*
            min->FixVariable(3);
            min->FixVariable(4);
            min->FixVariable(5);
            min->FixVariable(6);
            min->FixVariable(7);
            min->FixVariable(8);
*/

            // do the minimization
            bool success = min->Minimize();
            FitResult result;
            result.success=success;
            if (!success)
            {
                return result;
            }

            const double *xs = min->X();
            const double *err = min->Errors();
            for (unsigned int i = 0; i<9; ++i)
            {
                result.xmin[i]=xs[i];
                result.xerr[i]=err[i];
            }
            result.chi2=min->MinValue();
            /*
            printf("Minimum: %i\n",success);
            printf("neutrinoPt  = %7.4f +- %6.4f\n",xs[0],err[0]);
            printf("neutrinoPhi = %7.4f +- %6.4f\n",xs[1],err[1]);
            printf("neutrinoEta = %7.4f +- %6.4f\n",xs[2],err[2]);
            printf("leptonPt    = %7.4f +- %6.4f\n",xs[3],err[3]);
            printf("leptonPhi   = %7.4f +- %6.4f\n",xs[4],err[4]);
            printf("leptonEta   = %7.4f +- %6.4f\n",xs[5],err[5]);
            printf("bjetPt      = %7.4f +- %6.4f\n",xs[6],err[6]);
            printf("bjetPhi     = %7.4f +- %6.4f\n",xs[7],err[7]);
            printf("bjetEta     = %7.4f +- %6.4f\n",xs[8],err[8]);
             */
            /*
            std::cout
            << "Minimum: "<<success<< std::endl
            << "neutrinoPt "<<xs[0] << std::endl
            << "neutrinoPhi "<<xs[1] << std::endl
            << "neutrinoEta "<<xs[2] << std::endl
            << "leptonPt "<<xs[3] << std::endl
            << "leptonPhi "<<xs[4] << std::endl
            << "leptonEta "<<xs[5] << std::endl
            << "bjetPt "<<xs[6] << std::endl
            << "bjetPhi "<<xs[7] << std::endl
            << "bjetEta "<<xs[8] << std::endl;
*/
           return result;
        }
        
        pxl::LorentzVector makeVector(double pt, double phi, double eta, double mass=0.0)
        {
            double px = std::cos(phi)*pt;
            double py = std::sin(phi)*pt;
            double pz = std::sinh(eta)*pt;
            double E = std::sqrt(px*px+py*py+pz*pz+mass*mass);
            
            pxl::LorentzVector vec(px,py,pz,E);
            return vec;
        }

        FitResult getBestFitResult(const std::vector<FitResult>& results)
        {
            double chi2 = results[0].chi2;
            FitResult best = results[0];
            for (const FitResult& result: results)
            {
                if (chi2> result.chi2)
                {
                    chi2=result.chi2;
                    best=result;
                }
            }
            return best;
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                pxl::Particle* genTop = nullptr;
                pxl::Particle* genLepton = nullptr;
                pxl::Particle* genBJet = nullptr;
                pxl::Particle* genW = nullptr;
                pxl::Particle* genNeutrino = nullptr;
                
                if (event)
                {
                    std::vector<pxl::EventView*> eventViews;
                    event->getObjectsOfType(eventViews);
                    
                    for (unsigned ieventView=0; ieventView<eventViews.size();++ieventView)
                    {
                        pxl::EventView* eventView = eventViews[ieventView];
                        if (eventView->getName()=="Generated")
                        {
                            std::vector<pxl::Particle*> particles;
                            eventView->getObjectsOfType(particles);
                            for (unsigned int iparticle = 0; iparticle<particles.size(); ++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];
                                if (fabs(particle->getPdgNumber())==6)
                                {
                                    genTop=particle;
                                }
                                if (fabs(particle->getPdgNumber())==5 and !particle->getMothers().empty())
                                {
                                    genBJet=particle;
                                }
                                if (fabs(particle->getPdgNumber())==24)
                                {
                                    genW=particle;
                                }
                                if (fabs(particle->getPdgNumber())==14 and particle->getDaughterRelations().size()==0)
                                {
                                    genNeutrino=particle;
                                }
                                if (fabs(particle->getPdgNumber())==13 and particle->getDaughterRelations().size()==0)
                                {
                                    genLepton=particle;
                                }
                            }
                        }
                        
                    }
                    
                } 
                if (not (genTop && genBJet && genNeutrino && genLepton))
                {
                    _outputSource->setTargets(event);
                    return _outputSource->processTargets();
                }
                
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

                            pxl::Particle* lepton = nullptr;
                            pxl::Particle* met = nullptr;
                            std::vector<pxl::Particle*> bjets;
                            std::vector<pxl::Particle*> lightjets;

                            for (unsigned int iparticle = 0; iparticle<particles.size(); ++iparticle)
                            {
                                pxl::Particle* particle = particles[iparticle];
                                if (!lepton and particle->getName()==_leptonName)
                                {
                                    lepton=particle;
                                }
                                if (!met and particle->getName()==_metName)
                                {
                                    met=particle;
                                }
                                if (_bJetNames.find(particle->getName())!=_bJetNames.end())
                                {
                                    bjets.push_back(particle);
                                }
                                if (_lightJetNames.find(particle->getName())!=_lightJetNames.end())
                                {
                                    lightjets.push_back(particle);
                                }
                            }

                            if (!lepton || !met || bjets.size()==0)
                            {
                                throw std::runtime_error("obj not found");
                            }

                            //double topMass = 173.0;

                            std::function<double(const double*)> prop = [=](const double *xx )
                            {

                                double p = 1.0;
                                double negLogP = 0.0;

                                const double neutrinoPt = xx[0];
                                const double neutrinoPhi = xx[1];
                                const double neutrinoEta = xx[2];

                                const double neutrinoPx = std::cos(neutrinoPhi)*neutrinoPt;
                                const double neutrinoPy = std::sin(neutrinoPhi)*neutrinoPt;
                                const double neutrinoPz = std::sinh(neutrinoEta)*neutrinoPt;
                                const double neutrinoE = std::sqrt(neutrinoPx*neutrinoPx+neutrinoPy*neutrinoPy+neutrinoPz*neutrinoPz);
                                
                                //p*=TMath::LogNormal(neutrinoPt,0.9,0,20);
                                
                                p*=logNormal(neutrinoPt);
                                //negLogP +=std::pow((neutrinoPt-met->getPt())/30,2);
                                negLogP +=std::pow((neutrinoPhi-met->getPhi())/0.4,2); //~22.5 deg

                                const double leptonPt =fabs(xx[3]);
                                const double leptonPhi = xx[4];
                                const double leptonEta = xx[5];

                                const double leptonPx = std::cos(leptonPhi)*leptonPt;
                                const double leptonPy = std::sin(leptonPhi)*leptonPt;
                                const double leptonPz = std::sinh(leptonEta)*leptonPt;
                                const double leptonE = std::sqrt(leptonPx*leptonPx+leptonPy*leptonPy+leptonPz*leptonPz);

                                negLogP +=std::pow((leptonPt-lepton->getPt())/0.05,2);
                                negLogP +=std::pow((leptonPhi-lepton->getPhi())/0.001,2);
                                negLogP +=std::pow((leptonEta-lepton->getEta())/0.001,2);
                                

                                const double wPx = neutrinoPx + leptonPx;
                                const double wPy = neutrinoPy + leptonPy;
                                const double wPz = neutrinoPz + leptonPz;

                                const double wP2 = wPx*wPx+wPy*wPy+wPz*wPz;
                                const double wE2 = (neutrinoE+leptonE)*(neutrinoE+leptonE);
                                const double wMass2 = wE2-wP2;

                                negLogP +=pow((wMass2-80.3)/4.0,2);

                                const double bjetPt = fabs(xx[6]);
                                const double bjetPhi = xx[7];
                                const double bjetEta = xx[8];
                                
                                negLogP +=pow((bjetPt-bjets[0]->getPt())/0.5,2);
                                negLogP +=pow((bjetPt-bjets[0]->getPhi())/0.05,2);
                                negLogP +=pow((bjetPt-bjets[0]->getEta())/0.05,2);

                                const double bjetPx = std::cos(bjetPhi)*bjetPt;
                                const double bjetPy = std::sin(bjetPhi)*bjetPt;
                                const double bjetPz = std::sinh(bjetEta)*bjetPt;
                                const double bjetE = std::sqrt(bjetPx*bjetPx+bjetPy*bjetPy+bjetPz*bjetPz);

                                const double topPx = bjetPx + wPx;
                                const double topPy = bjetPy + wPy;
                                const double topPz = bjetPz + wPz;

                                const double topP2 = topPx*topPx+topPy*topPy+topPz*topPz;
                                const double topE2 = (neutrinoE+leptonE+bjetE)*(neutrinoE+leptonE+bjetE);
                                const double topMass2 = topE2-topP2;

                                negLogP += pow((topMass2-173.0)/2.0,2);

                                //std::cout<<"wmass "<<std::sqrt(wMass2)<<", topmass "<<std::sqrt(topMass2)<<", nEta "<<neutrinoEta<<std::endl;
                                //std::cout<<negLogP<<std::endl;
                                //std::cout<<"wmass "<<std::sqrt(wMass2)<<", nEta "<<neutrinoEta<<std::endl;
                                
                                p*=vdt::fast_exp(-negLogP*0.5);
                                return p;
                            };
                            
                            HMC<9>::Function fctWrap = [&prop](const HMC<9>::Vector& vec) -> double 
                            {
                                double array[9] ={vec[0],vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7],vec[8]};
                                
                                return prop(array);
                            };
                            
                            
                            HMC<9> hmc;
                            HMC<9>::Vector startPosition;
                            startPosition[0]=met->getPt();
                            startPosition[1]=met->getPhi();
                            startPosition[2]=0.0;

                            startPosition[3]=lepton->getPt();
                            startPosition[4]=lepton->getPhi();
                            startPosition[5]=lepton->getEta();

                            startPosition[6]=bjets[0]->getPt();
                            startPosition[7]=bjets[0]->getPhi();
                            startPosition[8]=bjets[0]->getEta();
                            
                            double pMax = -10000.0;
                            HMC<9>::Vector maxPosition;
                            
                            std::vector<TH2F*> hists;
                            hists.push_back(new TH2F("neutrino_pt",";neutrino p_T;dR",100,0,200,100,0,4));
                            hists.push_back(new TH2F("neutrino_phi",";neutrino #phi;dR",100,-9,9,100,0,4));
                            hists.push_back(new TH2F("neutrino_eta",";neutrino #eta;dR",100,-8,8,100,0,4));
                            
                            hists.push_back(new TH2F("lepton_pt",";lepton p_T;dR",100,-200,200,100,0,4));
                            hists.push_back(new TH2F("lepton_phi",";lepton #phi;dR",100,-9,9,100,0,4));
                            hists.push_back(new TH2F("lepton_eta",";lepton #eta;dR",100,-8,8,100,0,4));
                            
                            hists.push_back(new TH2F("bjet_pt",";bjet p_T;dR",100,-200,200,100,0,4));
                            hists.push_back(new TH2F("bjet_phi",";bjet #phi;dR",100,-9,9,100,0,4));
                            hists.push_back(new TH2F("bjet_eta",";bjet #eta;dR",100,-8,8,100,0,4));
                            
                            double genVec[9] = {
                                genNeutrino->getPt(),genNeutrino->getPhi(),genNeutrino->getEta(),
                                genLepton->getPt(),genLepton->getPhi(),genLepton->getEta(),
                                genBJet->getPt(),genBJet->getPhi(),genBJet->getEta(),
                            };
                            
                            for (unsigned int i = 0; i < 10000; ++i)
                            {
                                hmc.sample(fctWrap,startPosition,20,{
                                    0.5,
                                    0.001,
                                    0.02,
                                    
                                    0.0001,
                                    0.0001,
                                    0.0001,
                                    
                                    0.01,
                                    0.001,
                                    0.01
                                });
                               
                                if (i>1000)
                                {
                                    double p = fctWrap(startPosition);
                                    if (genTop and genBJet and genW and genNeutrino and genLepton)
                                    {
                                        pxl::LorentzVector neutrinoVec = makeVector(startPosition[0],startPosition[1],startPosition[2]);
                                        pxl::LorentzVector leptonVec = makeVector(startPosition[3],startPosition[4],startPosition[5]);
                                        pxl::LorentzVector bjetVec = makeVector(startPosition[6],startPosition[7],startPosition[8]);
                                        pxl::LorentzVector wVec = neutrinoVec + leptonVec;
                                        pxl::LorentzVector topVec = wVec + bjetVec;
                                        
                                        double neutrinoDR = neutrinoVec.deltaR(genNeutrino->getVector());
                                        double leptonDR = leptonVec.deltaR(genLepton->getVector());
                                        double bjetDR = bjetVec.deltaR(genBJet->getVector());
                                        
                                        //double dRVec[9] = {neutrinoDR,neutrinoDR,neutrinoDR,leptonDR,leptonDR,leptonDR,bjetDR,bjetDR,bjetDR};
                                        
                                        for (int i = 0; i<9; ++i)
                                        {
                                            hists[i]->Fill(startPosition[i],fabs(startPosition[i]-genVec[i]));
                                        }
                                    }
                                
                                    if (p>pMax)
                                    {
                                        pMax=p;
                                        maxPosition=startPosition;
                                    }
                                }
                            }
                            std::cout<<"eff = "<<hmc.getEfficiency()<<std::endl;
                            gROOT->SetStyle("Plain");
                            //gStyle->SetOptStat(0);
                            
                            for (int i = 0; i < 9; ++i)
                            {
                                TCanvas cv("cv","",800,600);
                                TH1* projected = hists[i]->ProjectionX();
                                projected->Draw();
                                
                                double max = projected->GetMaximum();
 
                                TLine reco = TLine(maxPosition[i],0,maxPosition[i],max);
                                reco.SetLineWidth(2);
                                reco.SetLineStyle(1);
                                reco.Draw("Same");
                                
                                TLine gen = TLine(genVec[i],0,genVec[i],max);
                                gen.SetLineWidth(2);
                                gen.SetLineStyle(2);
                                gen.Draw("Same");
                                
                                char buf[100];
                                sprintf(buf,"event_%i_%s.pdf",event->getUserRecord("Event number").toUInt64(),hists[i]->GetName());
                                
                                cv.Print(buf);
                                
                            }
                            
                            pxl::EventView* fittedEventView = event->create<pxl::EventView>();
                            fittedEventView->setName("Chi2Solved");
                            pxl::Particle* fittedNeutrino = fittedEventView->create<pxl::Particle>();
                            fittedNeutrino->setName("Neutrino");

                            
                            fittedNeutrino->setP4(makeVector(maxPosition[0],maxPosition[1],maxPosition[2]));
                            pxl::Particle* fittedLepton = fittedEventView->create<pxl::Particle>();
                            fittedLepton->setP4(makeVector(maxPosition[3],maxPosition[4],maxPosition[5]));
                            fittedLepton->setName("Lepton");
                            pxl::Particle* fittedBjet = fittedEventView->create<pxl::Particle>();
                            fittedBjet->setName("BJet");
                            fittedBjet->setP4(makeVector(maxPosition[6],maxPosition[7],maxPosition[8]));
                           
                            /*
                            std::vector<FitResult> results;
                            for (unsigned int iter = 0; iter<5 and results.empty(); ++iter)
                            {

                                unsigned int N=6+iter*6;
                                double etaRange = 10.0+iter*2.0;
                                for (unsigned int i=0; i<N; ++i)
                                {
                                    //for (unsigned int itop = 0; itop<10; ++itop)
                                    //{
                                        //topMass=30+itop*50;
                                        FitResult result = minimize(chi2,
                                            {
                                                met->getPt(),
                                                met->getPhi(),
                                                -etaRange*0.5+etaRange*i/(N-1),

                                                lepton->getPt(),
                                                lepton->getPhi(),
                                                lepton->getEta(),

                                                bjets[0]->getPt(),
                                                bjets[0]->getPhi(),
                                                bjets[0]->getEta()
                                            }
                                        );
                                        if (result.success)
                                        {
                                            results.push_back(result);
                                            //break;
                                        }
                                    //}
                                }
                            }
                            std::cout<<"solutions "<<results.size()<<std::endl;
                            if (results.size()>0)
                            {
                                FitResult bestResult = getBestFitResult(results);
                                pxl::EventView* fittedEventView = event->create<pxl::EventView>();
                                fittedEventView->setName("Chi2Solved");
                                pxl::Particle* fittedNeutrino = fittedEventView->create<pxl::Particle>();
                                fittedNeutrino->setName("Neutrino");
                                fittedNeutrino->setP4(bestResult.neutrino());
                                pxl::Particle* fittedLepton = fittedEventView->create<pxl::Particle>();
                                fittedLepton->setP4(bestResult.lepton());
                                fittedLepton->setName("Lepton");
                                pxl::Particle* fittedBjet = fittedEventView->create<pxl::Particle>();
                                fittedBjet->setName("BJet");
                                fittedBjet->setP4(bestResult.bjet());
                            }*/
                        } 
                    }
                    _outputSource->setTargets(event);
                    return _outputSource->processTargets();
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

PXL_MODULE_INIT(Chi2TopSolver)
PXL_PLUGIN_INIT
