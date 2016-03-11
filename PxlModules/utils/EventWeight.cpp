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

const std::unordered_map<std::string,FileInfo> eventWeightsPHYS14 = {
    //PHYS14
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
    }
};

const std::unordered_map<std::string,FileInfo> eventWeightsEA = {
    
    //DR74 50ns
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
    }
};


const std::unordered_map<std::string,FileInfo> eventWeights74X = {
    
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
    }
};

const std::unordered_map<std::string,FileInfo> eventWeights76X = {
    //DR76 25ns
    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=19938230 eff=12118808 - 7819422 = 4299386
            4299386,
            216.97 * 0.324
        }
    },
    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext",
        {
            //total=28918604 eff=17574978 - 11343626 = 6231352
            6231352,
            216.97 * 0.324
        }
    },
    /*
    {"ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=3299800 eff=3299800-0=3299800
            
            136.02 * 0.324
        }
    },
    {"ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=1680200 eff=1680200-0=1680200
            
            80.95 * 0.324
        }
    },
    */
    
    {"ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=2995200 eff=2062775 - 932425 = 1130350
            1130350,
            216.97 * 0.324
        }
    },
    
    {"ST_t-channel_4f_mtop1695_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=2971601 eff=1806371 - 1165230 = 641141
            641141,
            216.97 * 0.324
        }
    
    },
    
    {"ST_t-channel_4f_mtop1755_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=2999200 eff=2062775 - 932425 = 1130350
            1130350,
            216.97 * 0.324
        }
    },
    
    
    {"ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=1000000 eff=1000000-0=1000000
            1000000,
            35.6
        }
    },
    {"ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=999400 eff=999400-0=999400
            999400,
            35.6
        }
    },
    
    {"ST_tW_top_5f_mtop1695_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=492500 eff=492500-0=492500
            492500,
            35.6
        }
    },
    {"ST_tW_top_5f_mtop1755_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=484800 eff=484800-0=484800
            484800,
            35.6
        }
    },
    
    {"ST_tW_antitop_5f_mtop1695_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=499200 eff=499200-0=499200
            499200,
            35.6
        }
    },
    {"ST_tW_antitop_5f_mtop1755_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=482200 eff=482200-0=482200
            482200,
            35.6
        }
    },
    
    

    {"TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext",
        {
            //total=97994442 eff=97994442-0=97994442
            97994442,
            831.76
        }
    },
    {"TT_TuneCUETP8M1_mtop1755_13TeV-powheg-pythia8",
        {
            //total=9659200 eff=9659200-0=9659200
            9659200,
            831.76
        }
    },
    
    {"TT_TuneCUETP8M1_mtop1695_13TeV-powheg-pythia8",
        {
            //total=9900195 eff=9900195-0=9900195
            9900195,
            831.76
        }
    },
    
    {"TT_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
        {
            //total=18430200 eff=13742435 - 4687765 = 9054670
            9054670,
            831.76
        }
    },

    
    {"WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=24156124 eff=20338580 - 3817544 = 16521036
            16521036,
            20508.9*3
        }
    },
    
    {"WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
        {
            //total=47161328 eff=47161328-0=47161328
            47161328,
            20508.9*3
        }
    },
    
    {"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=28751199 eff=24005462 - 4745737 = 19259725
            19259725,
            2008.4*3
        }
    },
    
    //DO NOT USE THIS SAMPLE -> BUGGY
    {"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext",
        {
            //total=1719818 eff=1433110 - 286708 = 1146402
            1146402,
            2008.4*3
        }
    },
    
    {"QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8",
        {
            //total=21846364 eff=21846364-0=21846364
            21846364,
            866600000 * 0.00044
        }
    }
};






const std::unordered_map<std::string,FileInfo> eventWeights76Xv3 = {
    {"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=28751199 eff=24005460 - 4745737 = 19259723
            19259723,
            2008.4*3
        }
    },

    {"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext",
        {
            //total=121212419 eff=101224568 - 19987852 = 81236716
            81236716,
            2008.4*3
        }
    },

    {"QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8",
        {
            //total=21948126 eff=21948126 - 0 = 21948126
            21948126,
            866600000 * 0.00044
        }
    },

    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-herwigpp_TuneEE5C",
        {
            //total=19390065 eff=12580082 - 6809983 = 5770099
            5770099,
            216.97 * 0.324
        }
    },

    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=19938230 eff=12118808 - 7819422 = 4299386
            4299386,
            216.97 * 0.324
        }
    },

    {"ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext",
        {
            //total=27296269 eff=16588897 - 10707372 = 5881525
            5881525,
            216.97 * 0.324
        }
    },

    {"ST_t-channel_4f_mtop1695_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=2971601 eff=1806371 - 1165230 = 641141
            641141,
            216.97 * 0.324
        }
    },

    {"ST_t-channel_4f_mtop1755_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=2999200 eff=1823285 - 1175915 = 647370
            647370,
            216.97 * 0.324
        }
    },

    {"ST_t-channel_4f_scaledown_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext",
        {
            //total=29775108 eff=18096220 - 11678886 = 6417334
            6417334,
            216.97 * 0.324
        }
    },

    {"ST_t-channel_4f_scaleup_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1_ext",
        {
            //total=28635525 eff=17403546 - 11231978 = 6171568
            6171568,
            216.97 * 0.324
        }
    },

    {"ST_t-channel_5f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
        {
            //total=2995200 eff=2062775 - 932425 = 1130350
            1130350,
            216.97 * 0.324
        }
    },

    {"ST_t-channel_antitop_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=1630900 eff=1630900 - 0 = 1630900
            1630900,
            80.95 * 0.324
        }
    },

    {"ST_t-channel_top_4f_leptonDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=3299200 eff=3299200 - 0 = 3299200
            3299200,
            136.02  * 0.324
        }
    },

    {"ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=999400 eff=999400 - 0 = 999400
            999400,
            35.6
        }
    },

    {"ST_tW_antitop_5f_mtop1695_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=499200 eff=499200 - 0 = 499200
            499200,
            35.6
        }
    },

    {"ST_tW_antitop_5f_mtop1755_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=482200 eff=482200 - 0 = 482200
            482200,
            35.6
        }
    },

    {"ST_tW_antitop_5f_scaledown_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=495200 eff=495200 - 0 = 495200
            495200,
            35.6
        }
    },

    {"ST_tW_antitop_5f_scaleup_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=499600 eff=499600 - 0 = 499600
            499600,
            35.6
        }
    },

    {"ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=1000000 eff=1000000 - 0 = 1000000
            1000000,
            35.6
        }
    },

    {"ST_tW_top_5f_mtop1695_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=492500 eff=492500 - 0 = 492500
            492500,
            35.6
        }
    },

    {"ST_tW_top_5f_mtop1755_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=484800 eff=484800 - 0 = 484800
            484800,
            35.6
        }
    },

    {"ST_tW_top_5f_scaledown_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=500000 eff=500000 - 0 = 500000
            500000,
            35.6
        }
    },

    {"ST_tW_top_5f_scaleup_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
        {
            //total=499200 eff=499200 - 0 = 499200
            499200,
            35.6
        }
    },

    {"TT_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
        {
            //total=19090400 eff=14234931 - 4855469 = 9379462
            9379462,
            831.76
        }
    },

    {"TT_TuneCUETP8M1_13TeV-powheg-pythia8_ext",
        {
            //total=97994442 eff=97994440 - 0 = 97994440
            97994440,
            831.76
        }
    },

    {"TT_TuneCUETP8M1_13TeV-powheg-scaledown-pythia8_ext",
        {
            //total=38790990 eff=38790996 - 0 = 38790996
            38790996,
            831.76
        }
    },

    {"TT_TuneCUETP8M1_13TeV-powheg-scaleup-pythia8_ext",
        {
            //total=38507969 eff=38507960 - 0 = 38507960
            38507960,
            831.76
        }
    },

    {"TT_TuneCUETP8M1_mtop1695_13TeV-powheg-pythia8",
        {
            //total=699495 eff=699495 - 0 = 699495
            699495,
            831.76
        }
    },

    {"TT_TuneCUETP8M1_mtop1755_13TeV-powheg-pythia8",
        {
            //total=9659200 eff=9659200 - 0 = 9659200
            9659200,
            831.76
        }
    },

    {"WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
        {
            //total=24156124 eff=20338582 - 3817544 = 16521038
            16521038,
            20508.9*3
        }
    },

    {"WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ext",
        {
            //total=205212120 eff=172796864 - 32415296 = 140381568
            140381568,
            20508.9*3
        }
    },

    {"WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
        {
            //total=47161328 eff=47161340 - 0 = 47161340
            47161340,
            20508.9*3
        }
    }
};


struct Era
{
    enum TYPE
    {
        MCPHYS14=0, MCEA=1, MC74X=2, MC76X=3, MC76Xv3=4
    };
};
        
const std::unordered_map<int,const std::unordered_map<std::string,FileInfo>> _eventWeightsPerEra =
{
    {Era::MCPHYS14,eventWeightsPHYS14},
    {Era::MCEA,eventWeightsEA},
    {Era::MC74X,eventWeights74X},
    {Era::MC76X,eventWeights76X},
    {Era::MC76Xv3,eventWeights76Xv3}
};

class EventWeight:
    public pxl::Module
{
    private:
        pxl::Source* _outputSource;
        std::string _processNameField;

        std::vector<std::string> _allowedPostfixes;
        
        std::vector<std::string> _processNameList;
        
        Era::TYPE _eraWeightType;

    public:
    
        EventWeight():
            Module(),
            _processNameField("ProcessName"),
            _allowedPostfixes({"","_iso","_antiiso"})
        {
            addSink("input", "input");
            _outputSource = addSource("output","output");
            
            addOption("name of process field","",_processNameField);
            
            addOption("use PHYS14","",false);
            addOption("use EA","",false);
            addOption("use 74X","",false);
            addOption("use 76X","",false);
            addOption("use 76Xv3","",true);
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
            
            bool usePHYS14 = false;
            bool useEA = false;
            bool use74X = false;
            bool use76X = false;
            bool use76Xv3 = false;
            
            getOption("use PHYS14",usePHYS14);
            getOption("use EA",useEA);
            getOption("use 74X",use74X);
            getOption("use 76X",use76X);
            getOption("use 76Xv3",use76Xv3);
            std::string eraName = "";
            if (usePHYS14)
            {
                _eraWeightType = Era::MCPHYS14;
                eraName="PHYS14";
            }
            else if (useEA)
            {
                _eraWeightType = Era::MCEA;
                eraName="EA";
            }
            else if (use74X)
            {
                _eraWeightType = Era::MC74X;
                eraName="74X";
            }
            else if (use76X)
            {
                _eraWeightType = Era::MC76X;
                eraName="76X";
            }
            else if (use76Xv3)
            {
                _eraWeightType = Era::MC76Xv3;
                eraName="76Xv3";
            }
            else
            {
                throw std::runtime_error("Event weight era not set");
            }
            logger(pxl::LOG_LEVEL_INFO , "Event weights taken from '"+eraName+"'");
            
            for (auto it = _eventWeightsPerEra.at(_eraWeightType).cbegin(); it != _eventWeightsPerEra.at(_eraWeightType).cend(); ++it)
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
                    
                    std::string processName = event->getUserRecord(_processNameField).toString();
                    std::string strippedProcessName = "";
                    for (std::string& possibleName: _processNameList)
                    {
                        //need to find the largest match here for the 'ext' samples
                        if ((possibleName.size()>strippedProcessName.size()) and std::equal(possibleName.begin(),possibleName.end(),processName.begin()))
                        {
                            strippedProcessName = possibleName;
                        }
                    }

                    if (strippedProcessName.size()>0)
                    {
	                    auto it = _eventWeightsPerEra.at(_eraWeightType).find(strippedProcessName);
                        if (it!=_eventWeightsPerEra.at(_eraWeightType).end())
                        {
	                        event->setUserRecord("mc_weight",1.0*it->second.crossSection/it->second.nEvents);
                        }
                    }
                    if (strippedProcessName.size()==0)
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
