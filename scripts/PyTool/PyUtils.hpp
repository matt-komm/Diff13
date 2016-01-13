#ifndef __PYUTILS_H__
#define __PYUTILS_H__

#include "TObject.h"
#include "TH2.h"
#include "TMatrixD.h"
#include "TMath.h"

#include <vector>
#include <string>
#include <stdexcept>

class PyUtils
{
    private:
        PyUtils() {}
        virtual ~PyUtils() {}
    public:
        static TMatrixD convert2DHistToMatrix(const TH2* hist);
        static std::vector<double> getBinByBinCorrelations(const TH2* hist);

    ClassDef(PyUtils, 1)
};

#endif

