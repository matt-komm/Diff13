#include "pxl/hep.hh"
#include "pxl/core.hh"
#include "pxl/core/macros.hh"
#include "pxl/core/PluginManager.hh"
#include "pxl/modules/Module.hh"
#include "pxl/modules/ModuleFactory.hh"

#include <string>
#include <unordered_map>

static pxl::Logger logger("EventWeight");

struct FileInfo
{
    double nEvents;
    double crossSection;
};

const std::unordered_map<std::string,FileInfo> eventWeights = {
    //PHYS14
    /*
    {"TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola",
        {
            1037660,
            136.02*0.324
        }
    },
    {"TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola",
        {
            519948,
            80.95*0.324
        }
    },

    {"T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola",
        {
            986100,
            35.6
        }
    },
    {"Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola",
        {
            971800,
            35.6
        }
    },

    {"TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola",
        {
            500000,
            7.16 * 0.324
        }
    },
    {"TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola",
        {
            250000,
            4.20 * 0.324
        }
    },

    {"TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola",
        {
            25446993,
            831.76
        }
    },
    {"WJetsToLNu_13TeV-madgraph-pythia8-tauola",
        {
            10017462,
            20508.9*3 //(= n leptons)
        }
    },
    {"DYJetsToLL_M-50_13TeV-madgraph-pythia8",
        {
            2829164,
            6025.2
        }
    },

    {"QCD_Pt-20toInf_MuEnrichedPt15_PionKaonDecay_Tune4C_13TeV_pythia8",
        {
            4777926,
            866600000 * 0.00044
        }
    },

    {"QCD_Pt_20to30_bcToE_Tune4C_13TeV_pythia8",
        {
            999926,
            675900000 * 0.00075
        }
    },
    {"QCD_Pt_30to80_bcToE_Tune4C_13TeV_pythia8",
        {
            1852166,
            185900000 * 0.00272
        }
    },
    {"QCD_Pt_80to170_bcToE_Tune4C_13TeV_pythia8",
        {
            1000671,
            3495000 * 0.01225
        }
    },
    {"QCD_Pt_170toInf_bcToE_Tune4C_13TeV_pythia8",
        {
            1000221,
            128500 * 0.0406
        }
    },

    {"QCD_Pt-20to30_EMEnriched_Tune4C_13TeV_pythia8",
        {
            1987127,
            677300000 * 0.007
        }
    },
    {"QCD_Pt-30to80_EMEnriched_Tune4C_13TeV_pythia8",
        {
            2000838,
            185900000 * 0.056
        }
    },
    {"QCD_Pt-80to170_EMEnriched_Tune4C_13TeV_pythia8",
        {
            1959507,
            3529000 * 0.158
        }
    },

    {"WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola",
        {
            5262265,
            1817
        }
    },
    {"WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola",
        {
            4936077,
            471.6
        }
    },
    {"WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola",
        {
            4640594,
            55.61
        }
    },
    {"WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola",
        {
            4581841,
            18.81
        }
    },
    */
    
    //DR74 50ns
    /*
    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            860070,
            216.97 * 0.324
        }
    },
    {"ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            1273800,
            136.02 * 0.324
        }
    },
    {"ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            681900,
            80.95 * 0.324
        }
    },
    {"ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            998400,
            35.6
        }
    },
    {"ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            1000000,
            35.6
        }
    },
    
    {"ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            613384,
            11.36 * 0.324
        }
    },
    {"TT_TuneCUETP8M1_13TeV-powheg-pythia8",
        {
            19665194,
            831.76
        }
    },
    {"WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            16476437,
            20508.9*3
        }
    },
    {"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            13351018,
            2008.4*3
        }
    },
    {"QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8",
        {
            13227148,
            866600000 * 0.00044
        }
    },
    */
    
    //DR74 25ns
    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=19788182 eff=12027285-7760897=4266388
            4266388,
            216.97 * 0.324
        }
    },
    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext",
        {
            //total=29511826 eff=17936536-11575290=6361246
            6361246,
            216.97 * 0.324
        }
    },
    {"ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=3299800 eff=3299800-0=3299800
            3299800,
            136.02 * 0.324
        }
    },
    {"ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=1472500 eff=1472500-0=1472500
            1472500,
            80.95 * 0.324
        }
    },
    
    {"ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=773800 eff=773800-0=773800
            773800,
            35.6
        }
    },
    {"ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=988500 eff=988500-0=988500
            988500,
            35.6
        }
    },
    
    {"TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext",
        {
            //total=96134394 eff=96134394-0=96134394
            96134394,
            831.76
        }
    },
    {"TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=42496191 eff=28294599-14201592=14093007
            14093007,
            831.76
        }
    },
    
    {"TT_TuneZ2star_13TeV-powheg-pythia6-tauola",
        {
            //total=18666925 eff=18666925-0=18666925
            18666925,
            831.76
        }
    },
    
    
    {"WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=23742033 eff=19990467-3751566=16238901
            16238901,
            20508.9*3
        }
    },
    {"WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
        {
            //total=71533362 eff=71533362-0=71533362
            71533362,
            20508.9*3
        }
    },
    
    {"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=28255991 eff=23592814-4663177=18929637
            18929637,
            2008.4*3
        }
    },
    
    {"QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8",
        {
            //total=13101802 eff=13101802-0=13101802
            13101802,
            866600000 * 0.00044
        }
    },
    
};

class EventWeight:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        std::string _processNameField;

        std::vector<std::string> _allowedPostfixes;

    public:
    
        EventWeight():
            Module(),
            _processNameField("ProcessName"),
            _allowedPostfixes({"","_iso","_antiiso"})
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            
            addOption("name of process field","",_processNameField);
            addOption("allowed postfixes","",_allowedPostfixes);
        }

        ~EventWeight()
        {
        }

        // every Module needs a unique type
        static const std::string &getStaticType()
        {
            static std::string type ("EventWeight");
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
            getOption("name of process field",_processNameField);
            getOption("allowed postfixes",_allowedPostfixes);
        }

        bool analyse(pxl::Sink *sink) throw (std::runtime_error)
        {
            try
            {
                pxl::Event *event  = dynamic_cast<pxl::Event*>(sink->get());
                if (event)
                {
                    std::string processName = event->getUserRecord(_processNameField);
                    bool found = false;
                    for (unsigned int ipostfix = 0; ipostfix<_allowedPostfixes.size(); ++ipostfix)
                    {
                        const std::string& postfix = _allowedPostfixes[ipostfix];
                        //check if postfix matches
                        if (0 == processName.compare (processName.length() - postfix.length(), postfix.length(), postfix))
                        {
		                    auto it = eventWeights.find(processName.substr(0,processName.length() - postfix.length()));
                            if (it!=eventWeights.end())
                            {
		                        event->setUserRecord("mc_weight",1.0*it->second.crossSection/it->second.nEvents);
                                found = true;
                                break;
                            }
                        }
                    }
                    if (!found)
                    {
                        throw std::runtime_error("no event weight information available for process name '"+processName+"'");
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

PXL_MODULE_INIT(EventWeight)
PXL_PLUGIN_INIT
