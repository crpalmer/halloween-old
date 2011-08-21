CON

  _clkfreq = 80_000_000
  _clkmode = xtal1 + pll16x

  _speakerPin = 4
  _speakerVolume = 50

  _speakerMaxFrequency = 1000
  _speakerMinFrequency = 10
  _speakerFrequencyStep = 10
  _speakerFrequencyMsPauseTime = 10
  
OBJ

  spk: "SPKEngine.spin"
       
PUB demo

  spk.SPKEngineStart(_speakerPin)
  
  spk.changeSpeakerVolume(_speakerVolume) 

  repeat
   
    repeat result from _speakerMinFrequency to _speakerMaxFrequency step _speakerFrequencyStep
      spk.changeSpeakerFrequency(result)
      waitcnt(((clkfreq / 1000) * _speakerFrequencyMsPauseTime) + cnt)

    repeat result from _speakerMaxFrequency to _speakerMinFrequency step _speakerFrequencyStep
      spk.changeSpeakerFrequency(result)
      waitcnt(((clkfreq / 1000) * _speakerFrequencyMsPauseTime) + cnt)

  spk.SPKEngineStop
  
{{

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                                   TERMS OF USE: MIT License                                                 │                                                            
├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation   │ 
│files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,   │
│modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the        │
│Software is furnished to do so, subject to the following conditions:                                                         │         
│                                                                                                                             │
│The above copyright notice and this permission notice shall be included in all copies or substantial portions of the         │
│Software.                                                                                                                    │
│                                                                                                                             │
│THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE         │
│WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR        │
│COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,  │
│ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                        │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
}}                        