#include "PyUtils.hpp"


TMatrixD PyUtils::convert2DHistToMatrix(const TH2* hist)
{
    TMatrixD matrix(hist->GetNbinsX(),hist->GetNbinsY());
    for (int row=0; row<hist->GetNbinsX();++row)
    {
        for (int col=0; col<hist->GetNbinsY(); ++col)
        {
            (matrix)[row][col]=hist->GetBinContent(row+1,col+1);
        }
    }
    return std::move(matrix);
}

std::vector<double> PyUtils::getBinByBinCorrelations(const TH2* hist)
{
    int NBINS = hist->GetNbinsX();
    TMatrixD cov_matrix = convert2DHistToMatrix(hist);
    TMatrixD inv_cov_matrix=TMatrixD(TMatrixD::kInverted,cov_matrix);
    TMatrixD diag_cov_halfs(NBINS,NBINS);
    for (int i=0; i<NBINS; ++i) {
        for (int j=0; j<NBINS; ++j) {
            if (i==j)
            {
                diag_cov_halfs[i][j]=1.0/TMath::Sqrt((cov_matrix)[i][j]);
            }
            else
            {
                diag_cov_halfs[i][j]=0.0;
            }
        }
    }
    //correlations of the unfolded dist
    TMatrixD rho = diag_cov_halfs*cov_matrix*diag_cov_halfs;
    
    
    double rhoMax=0; //will store the max global correlation over bins
    for (int i=0; i<NBINS; ++i) 
    {
        //calculate the global correlations
        rhoMax=std::max(sqrt(1.0-1.0/(inv_cov_matrix[i][i]*(cov_matrix)[i][i])),rhoMax);
    }
    
    std::vector<double> rhoPerBins;
    rhoPerBins.push_back(rhoMax);
    //calculate the average per bin correlation; for the "subway" plot
    for (int offrow = 1; offrow<NBINS; ++offrow)
    {
        double sum=0.0;
        for (int entry = 0; entry < NBINS-offrow; ++entry)
        {
            sum+=rho[offrow+entry][entry];
        }
        rhoPerBins.push_back(sum/(NBINS-offrow));
    }
    return rhoPerBins;
}


ClassImp(PyUtils)


