#ifndef _JetCorrectionLevel_h
#define _JetCorrectionLevel_h

#include <functional>
#include <memory>

#include "DataFormats/PatCandidates/interface/Jet.h"

class JetCorrectionLevel
{
    private:
        std::shared_ptr<std::function<float(const pat::Jet&)>> _fct;
    public:
        JetCorrectionLevel()
        {
        }

        void setFunction(std::shared_ptr<std::function<float(const pat::Jet&)>> fct)
        {
            _fct=fct;
        }

        float getCorrection(const pat::Jet& jet) const
        {
            return (*_fct)(jet);
        }
};


#endif
