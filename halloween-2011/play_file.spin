CON
  NO_WORKER = 255
  
VAR
  byte                worker
  
OBJ

  fat: "SD2.0_FATEngine.spin"
  dac: "PCM2.1_DACEngine.spin"

PUB start(sd_p0, audio_p0, volume) | check

  ifnot(fat.FATEngineStart(sd_p0, sd_p0+1, sd_p0+2, sd_p0+3))
    reboot

  ifnot(dac.DACEngineStart(audio_p0, audio_p0+1, 0))
    reboot

  check := \fat.mountPartition(0, 0)
  if check < 0
    reboot

  dac.changeLeftChannelVolume(volume)
  dac.changeRightChannelVolume(volume)  

PUB sampleAddress
  return dac.sampleAddress
  
PUB synchronously(stringPointer) | numberOfChannels, sampleRate

    fat.openFile(stringPointer, "R")

    if(fat.readLong <> $46_46_49_52)
      return string("Not RIFF file")

    if((fat.readlong + 8) <> fat.listSize)
      return string("WAV file corrupt")

    if(fat.readLong <> $45_56_41_57)
      return string("Not WAVE file")       

    if(fat.readLong <> $20_74_6D_66)
      return string("Wav file corrupt")

    if(fat.readLong <> 16)
      return string("Wav file corrupt")
      
    if(fat.readShort <> 1)
      return string("Not LPCM file")

    numberOfChannels := fat.readShort  
    dac.changeNumberOfChannels(numberOfChannels)
    sampleRate := fat.readLong
    dac.changeSampleRate(sampleRate)
    
    fat.changeFilePosition(34)
    result := fat.readShort

    dac.changeBitsPerSample(result)    
    dac.changeSampleSign(result == 16)

    fat.changeFilePosition(28)

    if(fat.readLong <> (sampleRate * numberOfChannels * result / 8))
      return string("Wav file corrupt")

    if(fat.readShort <> (numberOfChannels * result / 8))
      return string("Wav file corrupt")    

    fat.changeFilePosition(36)
      
    if(fat.readLong <> $61_74_61_64)
      return string("No DATA") 

'   com.str(string("Now playing "))
'   com.str(stringPointer)

    dac.clearData 
    dac.startPlayer
  
    repeat (fat.readLong / 512)
      fat.readData(dac.transferData, 512)

    dac.stopPlayer
 '  com.str(string("... End of File."))
    fat.closeFile

    return -1   