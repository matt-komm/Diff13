#!/bin/bash
for pu in {650..750}
    do
    echo $pu"00" "for" $1
    pileupCalc.py -i $1 \
    --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/PileUp/pileup_latest.txt \
    --calcMode true --minBiasXsec $pu"00" --maxPileupBin 52 --numPileupBins 52 \
    "PU"$pu"00.root"
    done
