#!/bin/bash
for pu in {60..80}
    do
    echo $pu"000" "for" $1
    pileupCalc.py -i $1 \
    --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/PileUp/pileup_latest.txt \
    --calcMode true --minBiasXsec $pu"000" --maxPileupBin 52 --numPileupBins 52 \
    "PU"$pu"000.root"
    done
