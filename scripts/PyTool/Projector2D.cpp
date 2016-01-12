#include "Projector2D.hpp"
#include "TMath.h"
#include "TDirectory.h"
#include "TFile.h"
#include <math.h>

ClassImp(Projector2D)

Projector2D::Projector2D(TH2* hist, const char* filename, const char* treename, const char* varExpX, const char* varExpY, const char* cutExp):
    _hist(hist),
    _varExpX(varExpX),
    _varExpY(varExpY),
    _cutExp(cutExp)
{
    _filenames.push_back(filename);
    _treenames.push_back(treename);
}

void Projector2D::addFriend(const char* filename, const char* treename)
{
    _filenames.push_back(filename);
    _treenames.push_back(treename);
}

void Projector2D::Project(long maxentries)
{   
    TFile* mainFile = new TFile(_filenames[0].c_str());
    TTree* tree = (TTree*)(mainFile->Get(_treenames[0].c_str()));
    
    if (!tree)
    {
        std::cout<<"Tree '"<<_treenames[0].c_str()<<"' not found in file '"<<_filenames[0].c_str()<<std::endl;
        return;
    }
    for (unsigned int i = 1; i < _filenames.size(); ++i)
    {
        tree->AddFriend(_treenames[i].c_str(),_filenames[i].c_str());
    }
    
    tree->GetEntry(0);
    char varFctNameX[150];
    char varFctNameY[150];
    char cutFctName[150];
    sprintf(varFctNameX,"varFctX_%s_%i",_treenames[0].c_str(),rand());
    sprintf(varFctNameY,"varFctY_%s_%i",_treenames[0].c_str(),rand());
    sprintf(cutFctName,"cutFct_%s_%i",_treenames[0].c_str(),rand());
    TTreeFormula varFctX(varFctNameX,_varExpX.c_str(),tree);
    TTreeFormula varFctY(varFctNameY,_varExpY.c_str(),tree);
    TTreeFormula cutFct(cutFctName,_cutExp.c_str(),tree);
    if (maxentries==0)
    {
        maxentries=tree->GetEntries();
    }
    for (long ientry = 0; ientry < std::min((Long64_t)maxentries,tree->GetEntries()); ++ientry)
    {
        tree->GetEntry(ientry);
        
        double varX = varFctX.EvalInstance();
        double varY = varFctY.EvalInstance();
        double weight = cutFct.EvalInstance();
        if (!std::isnan(varX) && !std::isnan(varY) && !std::isnan(weight) && !std::isinf(varX) && !std::isinf(varY) && !std::isinf(weight))
        {
            _hist->Fill(varX,varY,weight);
        }
    }
    mainFile->Close();  
}   
