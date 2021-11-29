# Aims
* Measure the ratio between the two and three photons decay of the Positronium;
* Measure the lifetime of the Positronium through the temporal distribution of the events, i.e. decays.
  

## First session: preparation and calibration of the scintillators
* Connect the anode signal of the detector 4 directly to the oscilloscope: write down the **polarity amplitude** and the signal **rising time**. Then connect the anode signal to the input of FAN-IN-FAN-OUT and check its output on the oscilloscope -> identify the amplitude corresponding to the 511 keV and 1274 keV energy peaks.
* Connect the second output of the FAN-IN-FAN-OUT to the CFTD input and send the CFTD output to the second channel of the oscilloscope. Trigger on the ch2 (CFTD) and see the two peaks -> take photo of oscilloscope! 
* **Setting of the CFTD thresholds**: fix the minimum threshold value needed to cut the electronics noise (for detectors 1 2 and 3) and to cut 511 keV events for detector 4 (it will be a higher value since we want to trigger only in case of 1275 keV photons). As an example, if the peaks are seen @ an amplitude of 100 and 250 mV, a reasonable value for the threshold could be 75/80 mV for detectors 1 2 3 and around 200 mV for D4. Moreover it's possible to act on the *delay* and *width* micro switches: the first one allows to adjust the timing between the trigger signal and the start of the logic signal; the second one can be used to adjust the duration of the logic signal. 
* Connections:
  - anode signal from the FAN-IN-FAN-OUT -> ch1 of DAQ
  - delayed output to TRG IN of the digitizer
  - CFTD prompt output to CAEN scaler 
* Acquire 22Na spectrum: verify that 511 keV peak lies around channel number 7500
* For D4 for the calibration first use the minimum threshold, so as to have both peaks, then set the thr value at the correct value to cut the lower energy peak. For D1/D2/D3 repeat all the procedure and finally acquire the spectrum. 
* Fit the two peaks with a Gaussian retrieving centroids and sigmas: 
  - NB: if we observe a variable background it's better to add a linear background to the fit function.
* Calibrate the spectrum.
* Calculate the energy resolution.
* Digitizer dead time: measure the rate of different signals with both the digitizer and the CAEN scaler module. Connect a free output of the CFTD to the scaler and check this data during the acquisitions. 