export SCRAM_ARCH=slc6_amd64_gcc491
cmsrel CMSSW_7_3_4
cd CMSSW_7_3_4/src

cmsenv
git cms-init

hg clone -u pxl-3.5.1 https://forge.physik.rwth-aachen.de/hg/cmssw-modules/Pxl
git clone --branch plugin-based --depth 1 https://github.com/matt-komm/EDM2PXLIO.git

scram b -j10

