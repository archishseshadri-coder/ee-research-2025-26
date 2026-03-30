# ee-research-2025-26
# 5G Urban Canyon Propagation Analysis (28 GHz vs 3.5 GHz)

## Overview
This project utilizes the **ns-3 (v3.4x)** discrete-event simulator to evaluate the impact of urban geometry on 5G New Radio (NR) link budgets. Specifically, it characterizes the **Signal-to-Interference-plus-Noise Ratio (SINR)** and **Spectral Efficiency** across varying street widths and building densities.

## Research Methodology
I utilized a Full Factorial Design to isolate three primary variables:
1. **Carrier Frequency:** 3.5 GHz (Mid-band) vs. 28 GHz (mmWave).
2. **Street Geometry:** Widths ranging from 5m (Narrow) to 80m (Wide).
3. **Urban Density:** High-clutter (High Density) vs. Clear-path (Low Density) environments.

The simulation uses the **3GPP TR 38.901 UMi-Street Canyon** propagation model. Each scenario is run for **100 trials** with randomized seeding to ensure statistical significance.

## System Parameters
- **Transmit Power:** 23 dBm
- **Antenna Gain (Tx/Rx):** 10 dBi / 0 dBi
- **Bandwidth:** 100 MHz
- **Noise Figure:** 9 dB
- **Noise Floor:** -174 dBm/Hz (Thermal)

## How to Run
./ns3 run scratch/urban-canyon-model.cc -- --width=80 --freq=28e9 --highDensity=1
