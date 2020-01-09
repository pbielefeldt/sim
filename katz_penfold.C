/* ROOT macro to investigate the calculations in AliceModel.cc to receive an 
 * energy from the range. Range here is the cut value after which Geant4 will
 * treat an electron as an independent delta track.
 *
 * See L. Katz and A.S. Penfold, https://doi.org/10.1103/RevModPhys.24.28 for
 * reference
 *
 * (C) 2020, P. Bielefeldt, Uni Bonn
 */

#include <iostream>
#include <fstream>
#include <cmath>
// ROOT
#include "TCanvas.h"
#include "TGraph.h"

// these values are taken directly from the paper ...
const double a = 0.412;
const double b = 1.265;
const double c = 0.0954;

// for checking: the formula given in the paper (eq. 8)
double RangeFromEnergy(double e, double d)
{
    return a/d*std::pow(e, b-c*std::log(e));    
}

double ElectronEnergyFromRange(double r/*in cm*/, double d/*in g/cm³*/)
{   
    const double a_ = a/d;  // [r]=cm -> [r]=g/cm^2
    const double b_ = b/2.; // we only need b/2
    const double c_ = c;
    const double sq = std::sqrt((b_/c_)*(b_/c_) - (std::log(r/a_))/c_);
    
    return std::exp((b_/c_)-sq);
}

// for comparison with
// https://physics.nist.gov/PhysRefData/Star/Text/ESTAR.html
void KatzPenfold(double in_energy/*in MeV*/)
{
    // argon, see https://physics.nist.gov/cgi-bin/Star/compos.pl?matno=018
    const double d = 1.78e-3; // g/cm³
    
    double r = RangeFromEnergy(in_energy, d);
    double e = ElectronEnergyFromRange(r, d);

    std::cout << "E " << e << " MeV,\t R " << r*d << " g/cm^2" << std::endl;
}

void CompareKatzPenfold(std::string in_file="NIST-electron-data_argon.dat")
{
    double nist_e, nist_r, calc_e;
    std::fstream in_stream(in_file, std::ios_base::in);
    const double density = 1.78e-3;

    TGraph* calc_graph = new TGraph();
    TGraph* nist_graph = new TGraph();
    unsigned int graph_i = 0;

    while (in_stream >> nist_e >> nist_r)
    {
        calc_e = ElectronEnergyFromRange(nist_r/density, density);
        if (std::isnan(calc_e)) continue;

        nist_graph->SetPoint(graph_i, nist_r, nist_e);
        calc_graph->SetPoint(graph_i, nist_r, calc_e);
        graph_i++;
    }

    TCanvas *c1 = new TCanvas("c1","comparison", 300, 150, 960, 720);
    c1->cd();

    calc_graph->SetNameTitle("grC", "comparison of Katz-Penfold with NIST");
    //calc_graph->GetXaxis()->SetTitle("electron range / cm");
    //calc_graph->GetYaxis()->SetTitle("energy / MeV");
    calc_graph->Draw("AC");
    //nist_graph->SetNameTitle("grN", "");
    nist_graph->Draw("same");

    in_stream.close();
}
