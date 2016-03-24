#include "BTagWeightCalculator.hpp"
#include "BTagCalibrationStandalone.hpp"

#include <iostream>

int main()
{
    using namespace BWGHT;
    BTagCalibration _btagCalib;
    BTagCalibrationReader _readerNominal;
    BTagCalibrationReader _readerUp;
    BTagCalibrationReader _readerDown;

    BWGHT::BTagWeightCalculator _btagWeightCalc;

    constexpr static float MaxBJetPt = 670;
    constexpr static float MaxLJetPt = 1000;

    
    _btagCalib=BTagCalibration("CSVv2", "CSVv2_76X.csv");
    _readerNominal = BTagCalibrationReader(
        &_btagCalib,               // calibration instance
        BTagEntry::OP_TIGHT,  // operating point
        "mujets",               // measurement type
        "central"             // systematics type
    );         
      
    _readerUp = BTagCalibrationReader(&_btagCalib, BTagEntry::OP_TIGHT, "mujets", "up");  // sys up
    _readerDown = BTagCalibrationReader(&_btagCalib, BTagEntry::OP_TIGHT, "mujets", "down");  // sys down
                
    BWGHT::WorkingPoint tightWP(0.97);
    
    tightWP.setEfficiencyFunction(new BWGHT::ConstEfficiencyFunction(0.5));
    tightWP.setScaleFactorFunction(new BWGHT::LambdaScaleFactorFunction([&](const BWGHT::Jet& jet, BWGHT::SYS::TYPE sys) -> double
    {
        
        //return 1.1;
        
        float pt = jet.pt; 
        float eta = jet.eta;
        bool doubleUncertainty = false;
        
        
        BTagEntry::JetFlavor flavor = BTagEntry::FLAV_B;
        if (jet.flavor==5)
        {
            flavor = BTagEntry::FLAV_B;
            if (pt>MaxBJetPt)  
            {
                pt = MaxBJetPt; 
                doubleUncertainty = true;
            }  
        } 
        else if (jet.flavor==4)
        {
            flavor = BTagEntry::FLAV_C;
            if (pt>MaxBJetPt)  
            {
                pt = MaxBJetPt; 
                doubleUncertainty = true;
            }  
        } 
        else
        {
            flavor = BTagEntry::FLAV_UDSG;
            if (pt>MaxLJetPt)  
            {
                pt = MaxLJetPt; 
                doubleUncertainty = true;
            }  
        }
        std::cout<<jet.flavor<<std::endl;
        double jet_scalefactor =  _readerNominal.eval(flavor, eta, pt); 
        double jet_scalefactor_up =  _readerUp.eval(flavor, eta, pt); 
        double jet_scalefactor_down =  _readerDown.eval(flavor, eta, pt); 

        if (doubleUncertainty)
        {
            jet_scalefactor_up = 2*(jet_scalefactor_up - jet_scalefactor) + jet_scalefactor; 
            jet_scalefactor_down = 2*(jet_scalefactor_down - jet_scalefactor) + jet_scalefactor; 
        }

        else if (sys== BWGHT::SYS::BC_UP and (flavor==BTagEntry::FLAV_B or flavor==BTagEntry::FLAV_C))
        {
            return jet_scalefactor_up;
        }
        else if (sys== BWGHT::SYS::BC_DOWN and (flavor==BTagEntry::FLAV_B or flavor==BTagEntry::FLAV_C))
        {
            return jet_scalefactor_down;
        }
        else if (sys== BWGHT::SYS::L_UP and flavor==BTagEntry::FLAV_UDSG)
        {
            return jet_scalefactor_up;
        }
        else if (sys== BWGHT::SYS::L_DOWN and flavor==BTagEntry::FLAV_UDSG)
        {
            return jet_scalefactor_down;
        }
        return jet_scalefactor;
    
    }));
    _btagWeightCalc.addWorkingPoint(tightWP);
    
    std::cout<<_btagWeightCalc.getEventWeight({Jet(0.1,5,50,0)})<<std::endl;
}
