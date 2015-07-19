#ifndef __muonisolation_H__
#define __muonisolation_H__

#include "pxl/hep.hh"
#include "pxl/core.hh"
#include <limits>

double pfRelIsoCorrectedDeltaBetaR04(const pxl::Particle* particle, double beta=0.5)
{
    float R04PFsumChargedHadronPt = particle->getUserRecord("R04PFsumChargedHadronPt").toFloat();
    float R04sumNeutralHadronEt = particle->getUserRecord("R04PFsumNeutralHadronEt").toFloat(); //Correct it!
    float R04PFsumPhotonEt = particle->getUserRecord("R04PFsumPhotonEt").toFloat(); //Correct it!
    float R04PFsumPUPt = particle->getUserRecord("R04PFsumPUPt").toFloat();
    float pT =  particle->getPt();
    if( pT < std::numeric_limits<float>::epsilon())
    {
        throw "Division by zero pT!";
    }
    return  (R04PFsumChargedHadronPt + std::max(R04sumNeutralHadronEt+ R04PFsumPhotonEt - beta*R04PFsumPUPt, 0.0)) / pT;
}

double pfRelIsoPUPPI (pxl::Particle* particle)
{
    float pT =  particle->getPt();
    if( pT < std::numeric_limits<float>::epsilon())
    {
        throw "Division by zero pT!";
    }
    float relIso = particle->getUserRecord("puppiIsoMuonR40").toFloat() / pT;

    return relIso;
}

#endif

