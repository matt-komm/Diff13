#!/bin/bash
for pu in 60000 65000 70000 75000 80000 85000 90000
    do
    echo $pu for $1
    pileupCalc.py -i $1 \
    --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/PileUp/pileup_latest.txt \
    --calcMode true --minBiasXsec $pu --maxPileupBin 52 --numPileupBins 52 \
    "PU"$pu".root"
    done
