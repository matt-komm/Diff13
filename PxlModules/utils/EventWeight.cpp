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
            //total=19904330 eff=12098029-7806301=4291728
            4291728,
            216.97 * 0.324
        }
    },
    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext",
        {
            //total=29954054 eff=18205053-11749001=6456052
            6456052,
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
            //total=1680200 eff=1680200-0=1680200
            1680200,
            80.95 * 0.324
        }
    },
    {"ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=2966200 eff=2042819-923381=1119438
            1119438,
            216.97 * 0.324
        }
    },
    
    {"ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=995600 eff=995600-0=995600
            995600,
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
    
    {"TT_TuneCUETP8M1_13TeV-powheg-pythia8",
        {
            //total=19757190 eff=19757190-0=19757190
            19757190,
            831.76
        }
    },
    {"TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext",
        {
            //total=96834559 eff=96834559-0=96834559
            96834559,
            831.76
        }
    },
    {"TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=42784971 eff=28486758-14298213=14188545
            14188545,
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
            //total=24184766 eff=20363007-3821759=16541248
            16541248,
            20508.9*3
        }
    },
    {"WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
        {
            //total=72207128 eff=72207128-0=72207128
            72207128,
            20508.9*3
        }
    },
    
    {"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=28747969 eff=24003538-4744431=19259107
            19259107,
            2008.4*3
        }
    },
    
    {"QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8",
        {
            //total=13247363 eff=13247363-0=13247363
            13247363,
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
        
        std::vector<std::string> _processNameList;

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
            
            for (auto it = eventWeights.begin(); it!=eventWeights.end(); ++it)
            {
                _processNameList.push_back(it->first);
            }
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
                    for (std::string& possibleName: _processNameList)
                    {
                        if (std::equal(possibleName.begin(),possibleName.end(),processName.begin()))
                        {
                            processName = possibleName;
                            found=true;
                        }
                    }

                    if (found)
                    {
	                    auto it = eventWeights.find(processName);
                        if (it!=eventWeights.end())
                        {
	                        event->setUserRecord("mc_weight",1.0*it->second.crossSection/it->second.nEvents);
                            found = true;
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
