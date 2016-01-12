#ifndef __PROJECTOR2D_H__
#define __PROJECTOR2D_H__

#include <iostream>
#include <functional>
#include <vector>
#include <string>

#include "TObject.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TH2.h"
#include "TThread.h"
#include "TFile.h"
#include "TMemFile.h"



class Projector2D
{
    public:
        
    private:
        TH2* _hist;
        std::vector<std::string> _filenames;
        std::vector<std::string> _treenames;
        std::string _varExpX;
        std::string _varExpY;
        std::string _cutExp;
        
    public:
    
        Projector2D(TH2* hist, const char* filename, const char* treename, const char* varExpX, const char* varExpY, const char* cutExp);
        
        void addFriend(const char* filename, const char* treename);
        void Project(long maxentries=0);
        
        inline const TH2* getHist() const
        {
            return _hist;
        }
        
        virtual ~Projector2D()
        {
        }


    ClassDef(Projector2D, 1)
};

#endif

