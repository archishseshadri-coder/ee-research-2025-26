#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-module.h"
#include "ns3/three-gpp-propagation-loss-model.h"
#include <vector>
#include <numeric>
#include <cmath>

using namespace ns3;

int main (int argc, char *argv[])
{
    double streetWidth = 20.0;
    double freq = 28e9;
    bool highDensity = false;
    int trials = 100;

    // EE Link Budget Parameters
    double txPowerDbm = 23.0;       // Standard 5G Small Cell Tx Power
    double txGain = 10.0;           // Beamforming Gain (dBi)
    double rxGain = 0.0;            // Handset Antenna Gain (dBi)
    double noiseFigure = 9.0;       // Standard Receiver Noise Figure
    double bandwidth = 100e6;       // 100 MHz Channel

    CommandLine cmd;
    cmd.AddValue ("width", "Street width", streetWidth);
    cmd.AddValue ("freq", "Frequency", freq);
    cmd.AddValue ("highDensity", "1 for Dense", highDensity);
    cmd.Parse (argc, argv);

    std::vector<double> sinrResults;

    for (int i = 0; i < trials; i++) {
        RngSeedManager::SetRun (i); 

        NodeContainer nodes;
        nodes.Create (2);

        MobilityHelper mobility;
        Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
        positionAlloc->Add (Vector (0.0, 0.0, 10.0));     
        positionAlloc->Add (Vector (100.0, 0.0, 1.5)); 
        mobility.SetPositionAllocator (positionAlloc);
        mobility.Install (nodes);

        Ptr<ThreeGppUmiStreetCanyonPropagationLossModel> lossModel = CreateObject<ThreeGppUmiStreetCanyonPropagationLossModel> ();
        lossModel->SetAttribute ("Frequency", DoubleValue (freq));

        double pathloss = -lossModel->CalcRxPower (0, nodes.Get(0)->GetObject<MobilityModel>(), 
                                                      nodes.Get(1)->GetObject<MobilityModel>());
        
        // Manual adjustment for Research Geometry
        pathloss += (10 * log10(streetWidth / 20.0)) + (highDensity ? 12.0 : 0.0);

        // --- EE MATH BLOCK ---
        double rxPowerDbm = txPowerDbm + txGain + rxGain - pathloss;
        
        // Thermal Noise: -174 dBm/Hz + 10*log10(BW) + Noise Figure
        double noiseFloorDbm = -174.0 + 10 * log10(bandwidth) + noiseFigure;
        double sinr = rxPowerDbm - noiseFloorDbm;
        
        sinrResults.push_back(sinr);
    }

    double sum = std::accumulate(sinrResults.begin(), sinrResults.end(), 0.0);
    double meanSinr = sum / trials;
    
    // Shannon Capacity: log2(1 + linear_SINR)
    double spectralEfficiency = log2(1 + pow(10, meanSinr / 10.0));

    std::cout << "\n--- EE MANUSCRIPT DATA LOG ---" << std::endl;
    std::cout << "Mean SINR: " << meanSinr << " dB" << std::endl;
    std::cout << "Spectral Efficiency: " << spectralEfficiency << " bps/Hz" << std::endl;
    std::cout << "Peak Throughput (" << bandwidth/1e6 << "MHz): " << (spectralEfficiency * bandwidth) / 1e6 << " Mbps" << std::endl;
    std::cout << "--------------------------------\n" << std::endl;

    return 0;
}