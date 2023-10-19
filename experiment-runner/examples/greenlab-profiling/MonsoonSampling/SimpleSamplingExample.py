import Monsoon.HVPM as HVPM
import Monsoon.LVPM as LVPM
import Monsoon.sampleEngine as sampleEngine
import Monsoon.Operations as op
import Monsoon.pmapi as pmapi
import numpy as np

def testHVPM(serialno=None,Protocol=pmapi.USB_protocol()):
    HVMON = HVPM.Monsoon()
    HVMON.setup_usb(serialno,Protocol)
    print("HVPM Serial Number: " + repr(HVMON.getSerialNumber()))
    HVMON.fillStatusPacket() 
    HVMON.setVout(6)
    HVengine = sampleEngine.SampleEngine(HVMON)
    #Output to CSV
    HVengine.enableCSVOutput("HV Output.csv") 
    #Turning off periodic console outputs.
    HVengine.ConsoleOutput(False)

 
    #Setting main channels enabled
    HVengine.enableChannel(sampleEngine.channels.MainCurrent)         # Main Channel
    HVengine.enableChannel(sampleEngine.channels.MainVoltage)

    # USB Channel
    HVengine.enableChannel(sampleEngine.channels.USBCurrent)            
    HVengine.enableChannel(sampleEngine.channels.USBVoltage)

    # Aux Channel
    HVengine.enableChannel(sampleEngine.channels.AuxCurrent)     
     
    # timeStamp Channel
    HVengine.enableChannel(sampleEngine.channels.timeStamp)

    # Allows power to the HVPM USB ports
    HVMON.setUSBPassthroughMode(op.USB_Passthrough.On) 

    #Setting trigger conditions
    numSamples=sampleEngine.triggers.SAMPLECOUNT_INFINITE      # numSamples = 5000 is one second of sampling 

    HVengine.setStartTrigger(sampleEngine.triggers.GREATER_THAN,0) # Determine when to begin readings
    # HVengine.setStopTrigger(sampleEngine.triggers.GREATER_THAN,5) # Determine when to stop readings

    HVengine.setTriggerChannel(sampleEngine.channels.timeStamp) 

    #Actually start collecting samples
    HVengine.startSampling(samples=numSamples, granularity=10, legacy_timestamp=True)
    #startSampling() continues until the trigger conditions have been met, and then ends automatically.
    #Measurements are automatically saved to the filename passed in enableCSVOutput()

def main():
    HVPMSerialNo = 23170    # Enter your device's serial number here (Sticker on the back of the device)
    testHVPM(HVPMSerialNo,pmapi.USB_protocol())

if __name__ == "__main__":
    main()