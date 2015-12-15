#ifndef __PYFIT_H__
#define __PYFIT_H__

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class PyFit
{
    public:
        PyFit()
        {
        }
        
        virtual ~PyFit()
        {
        }
        
    ClassDef(PyFit, 1)
};

#endif
