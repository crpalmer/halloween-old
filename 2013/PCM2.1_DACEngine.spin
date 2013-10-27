{{
+-----------------------------------------------------------------------------------------------------------------------------+               
� PCM2.1 Digital To Analog Converter Engine                                                                                   �
�                                                                                                                             �
� Author: Kwabena W. Agyeman                                                                                                  �                              
� Updated: 6/2/2010                                                                                                           �
� Designed For: P8X32A @ 80Mhz                                                                                                �
� Version: 1.1                                                                                                                �
�                                                                                                                             �
� Copyright (c) 2010 Kwabena W. Agyeman                                                                                       �              
� See end of file for terms of use.                                                                                           �
�                                                                                                                             �
� Update History:                                                                                                             �
�                                                                                                                             �
� v1.0 - Original release - 1/10/2010.                                                                                        �
� v1.1 - Added variable pin support - 6/2/2010.                                                                               �
�                                                                                                                             �
� For each included copy of this object only one spin interpreter should access it at a time.                                 �
�                                                                                                                             �
� Nyamekye,                                                                                                                   �
|
|
| Modified by crpalmer:
|  * added ability to get the raw left channel
|  * added the ability to have up to 4 channels with channels 3 and 4 being data (e.g. servo) channels
|  * removed support for 8-bit sound files.
+-----------------------------------------------------------------------------------------------------------------------------+
+-----------------------------------------------------------------------------------------------------------------------------+
� Converter Circuit:                                                                                                          �
�                                                                                                                             �
�                        100ohm   1microF                                                                                           �
� DAC Left Pin Number  -res--+--cap-> Left Out                                                                                  �                                    
�                            �                                                                                                �
�                            cap 10nF                                                                                           �
�                            �                                                                                                �
�                            gnd                                                                                                �
�                                                                                                                             �
�                        100ohm   1microF                                                                                           �
� DAC Right Pin Number -res--+--cap->Right Out                                                                                 �
�                            �                                                                                                �
�                            cap  10nF                                                                                           �
�                            �                                                                                                �
�                            gnd                                                                                                �
+-----------------------------------------------------------------------------------------------------------------------------+
}}
                         
VAR

  long cogNumber, sampleRate
  byte callerToggle, calleeToggle
  long currentSample[4]
  word dataBlock[512]                          
  byte stopedOrStarted, numberOfChannels , numberOfTracksToPlay

PUB isPlaying
  return stopedOrStarted

PUB sampleAddress
  return @currentSample[0]
  
PUB startPlayer '' 3 Stack Longs

'' +--------------------------------------------------------------------------------------------------------------------------+
'' � Starts/Unpauses the stereo DAC player. The player will continue at whatever point it stopped at.                         �
'' +--------------------------------------------------------------------------------------------------------------------------+
 
  stopedOrStarted := true

PUB stopPlayer '' 3 Stack Longs

'' +--------------------------------------------------------------------------------------------------------------------------+
'' � Stops/pauses the stereo DAC. The player will remember at whatever point it stopped at.                                   �                               
'' +--------------------------------------------------------------------------------------------------------------------------+

  stopedOrStarted := false

PUB transferData '' 3 Stack Longs

'' +--------------------------------------------------------------------------------------------------------------------------+
'' � Helps reliably transfers data to the stereo DAC to play.                                                                 �
'' �                                                                                                                          �
'' � Returns the address of the next data buffer to transfer data to.                                                         �
'' �                                                                                                                          �
'' � At 8 bits per sample using 1 channel each data buffer is composed of 512 bytes with 512 samples per transfer.            �
'' �                                                                                                                          �
'' � At 8 bits per sample using 2 channels each data buffer is composed of 512 bytes with 256 samples per transfer.           �
'' �                                                                                                                          �
'' � At 16 bits per sample using 1 channel each data buffer is composed of 256 words with 256 samples per transfer.           �
'' �                                                                                                                          �
'' � At 16 bits per sample using 2 channels each data buffer is composed of 256 words with 128 samples per transfer.          �
'' �                                                                                                                          �
'' � This function waits indefiantly to sync up with the DAC driver.                                                          �    
'' �                                                                                                                          �
'' � Use this function with the longmove/wordmove/bytemove commands and/or a file system to play data.                        �
'' +--------------------------------------------------------------------------------------------------------------------------+

  repeat until callerToggle == calleeToggle
  if calleeToggle
    callerToggle := 0
    result := @dataBlock[0]
  else
    callerToggle := 1
    result := @dataBlock[256]

PUB clearData '' 3 Stack Longs

'' +--------------------------------------------------------------------------------------------------------------------------+
'' � Clears all data in all player data buffers.                                                                              �
'' �                                                                                                                          �
'' � Helps prevent old data from being played when changing to a new data transfer source.                                    �                                                                
'' +--------------------------------------------------------------------------------------------------------------------------+

  bytefill(@dataBlock, 0, 1024)
  
PUB changeNumberOfChannels(newNumber) '' 4 Stack Longs

'' +--------------------------------------------------------------------------------------------------------------------------+
'' � Changes the number of channels, 1 channel or 2 channels.                                                                 �
'' �                                                                                                                          �
'' � When set to 1 channel mode the driver will output samples to both channels at once.                                      �
'' �                                                                                                                          �
'' � When set to 2 channel mode the driver will output samples to both channels at simultaneously.                            �
'' �                                                                                                                          �
'' � NewNumber - New number of channels to output samples at. Samples are grouped by this value. (1 or 2)                     �
'' +--------------------------------------------------------------------------------------------------------------------------+

  numberOfChannels := newNumber

PUB changeNumberOfTracksToPlay(newNumber)
  numberOfTracksToPlay := newNumber
  
PUB changeSampleRate(newRate) '' 4 Stack Longs

'' +--------------------------------------------------------------------------------------------------------------------------+
'' � Changes the sample rate. The stereo DAC supportes samples rates from 1HZ to 80KHZ @ 80Mhz.                               �
'' �                                                                                                                          �
'' � NewRate - New sample rate to output samples at. Samples are outputted at the frequency specified by this value.          �
'' +--------------------------------------------------------------------------------------------------------------------------+

  sampleRate := (clkfreq / ((newRate <# (clkfreq / constant(80_000_000 / 80_000))) #> 1))

PUB DACEngineStart(leftPinNumber, rightPinNumber, newRate) '' 10 Stack Longs 

'' +--------------------------------------------------------------------------------------------------------------------------+
'' � Starts up the DAC driver running on a cog.                                                                               �
'' �                                                                                                                          �
'' � Returns true on success and false on failure.                                                                            �
'' �                                                                                                                          �
'' � LeftPinNumber - Pin to use to play the left channel audio. -1 to disable.                                                �
'' � RightPinNumber - Pin to use to play the right channel audio. -1 to disable.                                              �
'' � NewRate - New sample rate to output samples at. Samples are outputted at the frequency specified by this value.          � 
'' +--------------------------------------------------------------------------------------------------------------------------+

  DACEngineStop
  changeSampleRate(newRate)  

  if((chipver == 1) and (clkmode > 1))
  
    result := ((leftPinNumber <# 31) #> 0)
    newRate := ((rightPinNumber <# 31) #> 0)

    outputMask := (((leftPinNumber <> -1) & (|<result)) | ((rightPinNumber <> -1) & (|<newRate)))  
  
    leftCounterSetup := (result + constant(%00110 << 26))
    rightCounterSetup := (newRate + constant(%00110 << 26))

    dataBlockAddress := @dataBlock  
    calleeToggleAddress := @calleeToggle
    stopedOrStartedAddress := @stopedOrStarted
    numberOfChannelsAddress := @NumberOfChannels
    numberOfTracksToPlayAddress := @NumberOfTracksToPlay
    currentSampleAddress1 := @currentSample
    currentSampleAddress2 := currentSampleAddress1 + 4
    currentSampleAddress3 := currentSampleAddress1 + 8

    cogNumber := cognew(@initialization, @sampleRate)  
    return (not(not(++cogNumber)))
  
PUB DACEngineStop '' 3 Stack Longs

'' +--------------------------------------------------------------------------------------------------------------------------+
'' � Shuts down the DAC driver running on a cog.                                                                              �                
'' +--------------------------------------------------------------------------------------------------------------------------+

  if(cogNumber)
    cogstop(-1 + cogNumber~) 

DAT

' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
'                       DAC Driver
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  

                        org     0

' //////////////////////Initialization/////////////////////////////////////////////////////////////////////////////////////////

initialization          mov     ctra,            leftCounterSetup              ' Setup counter modes to duty cycle mode.
                        mov     ctrb,            rightCounterSetup             '
                        mov     frqa,            longAdjust                    ' 
                        mov     frqb,            longAdjust                    '                         
                        
                        mov     dira,            outputMask                    ' Setup I/O pin directions.

                        rdlong  timeCounter,     par
                        add     timeCounter,     cnt                           '    

waiting                 mov     frqa,            longAdjust                                            ' 
                        mov     frqb,            longAdjust                    '
                        mov     buffer,          #0
                        wrbyte  buffer,          calleeToggleAddress
                        wrlong  buffer,          currentSampleAddress1
                        wrlong  buffer,          currentSampleAddress2
                        wrlong  buffer,          currentSampleAddress3
                        
stillWaiting            rdbyte  buffer,          stopedOrStartedAddress wz
if_z                    rdlong  buffer,          par
if_z                    waitcnt timeCounter,     buffer
if_z                    jmp     #stillWaiting

                        rdbyte  currentChannels, numberOfChannelsAddress wz    ' Setup player mode.
                        rdbyte  currentTracks,   numberOfTracksToPlayAddress
                        mov     playerPointer,   dataBlockAddress
'                       rdword  counter,         W256
                        mov     counter,         #255

                        
' //////////////////////Inner Loop/////////////////////////////////////////////////////////////////////////////////////////////                               

loop
                        call    #decode                                        '
                        wrlong  raw,             currentSampleAddress1
                        mov     frqa,            sampleBuffer                  '
                        cmp     currentTracks,  #2 wc
if_nc                   mov     frqb,            sampleBuffer


                        cmp     currentChannels, #2 wc
if_nc                   call    #decode
                        wrlong  raw,             currentSampleAddress2
                        cmp     currentTracks,  #2 wc
if_nc                   mov     frqb,            sampleBuffer

                        cmp     currentChannels, #3 wc
if_nc                   call    #decode
                        wrlong  raw,             currentSampleAddress3

                        rdlong  buffer,          par                           ' Wait until next sample output period.
                        waitcnt timeCounter,     buffer                        '

                        rdbyte  buffer,          stopedOrStartedAddress wz     ' If stopped loop continously.
if_z                    jmp     #waiting                                                                                
                        jmp     #loop                                          ' Loop.

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Decode Value
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

decode                  rdword  multiplyBuffer,  playerPointer                 '
                        call    #incrementPlayerPointer    

                        shl     multiplyBuffer,  #16                           ' Sign extend.
                        sar     multiplyBuffer,  #16                           '

                        mov     raw,             multiplyBuffer
                        shl     multiplyBuffer,  #16
                        mov     sampleBuffer,    multiplyBuffer
                        add     sampleBuffer,    longAdjust
                        
                    
decode_ret              ret                                                    ' Return.

' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
'         incrementPlayerPointer
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

incrementPlayerPointer  add     playerPointer, #2
                        sub     counter, #1 wc
if_c                    call    #loadCurrentBuffer
incrementPlayerPointer_ret     ret

loadCurrentBuffer       rdbyte  currentChannels, numberOfChannelsAddress wz    ' Setup player mode.
                        rdbyte  currentTracks,   numberOfTracksToPlayAddress
                        
                        mov     counter,        #255

                        rdbyte  buffer,          calleeToggleAddress wz        ' Switch data block pointer
if_z                    mov     buffer,          #1
if_nz                   mov     buffer,          #0
                        wrbyte  buffer,          calleeToggleAddress
if_nz                   mov     playerPointer,   dataBlockAddress
                        
loadCurrentBuffer_ret   ret
                        
' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Data
' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

wordAdjust              long    $8000                                          ' Edits word signed value.
longAdjust              long    $80000000                                      ' Edits long unsigend value.

' //////////////////////Configuration Settings/////////////////////////////////////////////////////////////////////////////////

leftCounterSetup        long    0
rightCounterSetup       long    0

outputMask              long    0  

' //////////////////////Addresses//////////////////////////////////////////////////////////////////////////////////////////////

dataBlockAddress        long    0
calleeToggleAddress     long    0
stopedOrStartedAddress  long    0
numberOfChannelsAddress long    0
currentSampleAddress1    long    0
currentSampleAddress2   long    0
currentSampleAddress3   long    0
numberOfTracksToPlayAddress long 0
raw                     word    0

' //////////////////////Run Time Variables/////////////////////////////////////////////////////////////////////////////////////

buffer                  res     1
counter                 res     1

playerPointer           res     1
currentChannels         res     1
currentTracks           res     1

sampleBuffer            res     1
timeCounter             res     1

multiplyBuffer          res     1
multiplyCounter         res     1

' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                        fit     496
 
{{

+-----------------------------------------------------------------------------------------------------------------------------+
�                                                   TERMS OF USE: MIT License                                                 �                                                            
+-----------------------------------------------------------------------------------------------------------------------------�
�Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation   � 
�files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,   �
�modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the        �
�Software is furnished to do so, subject to the following conditions:                                                         �         
�                                                                                                                             �
�The above copyright notice and this permission notice shall be included in all copies or substantial portions of the         �
�Software.                                                                                                                    �
�                                                                                                                             �
�THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE         �
�WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR        �
�COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,  �
�ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                        �
+-----------------------------------------------------------------------------------------------------------------------------+
}}                        