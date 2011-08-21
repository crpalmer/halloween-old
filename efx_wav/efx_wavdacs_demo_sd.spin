'' =================================================================================================
''
''   File....... efx_wavdacs_demo_sd.spin
''   Purpose.... 
''   Author..... Jon "JonnyMac" McPhalen (aka Jon Williams)
''               Copyright (c) 2010 Jon McPhalen
''               -- see below for terms of use
''   E-mail..... jon@jonmcphalen.com
''   Started.... 
''   Updated.... 
''
'' =================================================================================================


con

  _clkmode = xtal1 + pll16x
' _xinfreq = 5_000_000                                          ' for demo board
  _xinfreq = 6_250_000                                          ' for AP-16+

  CLK_FREQ = ((_clkmode - xtal1) >> 6) * _xinfreq
  MS_001   = CLK_FREQ / 1_000


con

  SD_DO = 16


con 

  #1, HOME, #8, BKSP, TAB, LF, CLREOL, CLRDN, CR, #16, CLS      ' PST formmatting control


var

  long  lrsamples                                               ' WAV sample to be output
  
  long  killplayer
  long  playcog
  long  pcstack[64]
  


dat

TestFile                byte    "test.wav", 0

Msg1                    byte    "-- format........ ", 0
Msg2                    byte    "-- channels...... ", 0
Msg3                    byte    "-- bits/sample... ", 0
Msg4                    byte    "-- sample rate... ", 0
Msg5                    byte    "-- samples....... ", 0
Msg6                    byte    "-- duration...... ", 0

   
obj

  duty : "stereoduty"                                           ' dithering DAC output
  wav  : "efx_wavdacs_sd"                                       ' uses StereoDuty
  sd   : "fsrw"  
  term : "fullduplexserial"
   
  
pub main | check, d 

  duty.start(@lrsamples, 11, 10)                                ' launch duty into cog 1  
  wav.init(@lrsamples)
  term.start(31, 30, %0000, 115_200)

  pause(5)
  term.tx(CLS)
  term.str(string("WAV player demo", CR, CR))
    

  check := \sd.mount(SD_DO)
  if (check <> 0)
    term.str(string("Error: SD card failed to mount... "))
    term.dec(check)
    repeat ' until reset  
  else
    term.str(string("SD Card Mounted", CR)) 

  term.str(string("Opening: "))
  term.str(@TestFile)
  term.tx(CR)
  check := sd.popen(@TestFile, "r")
  if (check <> 0)
    term.str(string("Error: File failed to open"))
    repeat ' until reset

  ' read/display file attributes

  check := sd.pread(wav.headerbuf, 44)
  sd.pclose
  if (check <> 44)
    term.str(string("Error: Invalid file/header"))
    repeat ' until reset

  term.str(@Msg1)
  term.dec(wav.format)
  term.tx(CR)

  term.str(@Msg2)
  term.dec(wav.channels)
  term.tx(CR)

  term.str(@Msg3)
  term.dec(wav.bitspersample)
  term.tx(CR)

  term.str(@Msg4)
  term.dec(wav.samplerate)
  term.tx(CR)    

  term.str(@Msg5)
  term.dec(wav.samples)
  term.tx(CR)

  d := wav.duration
  term.str(@Msg6)
  term.dec(d / 10)
  term.tx(".")
  term.dec(d // 10)
  term.tx(CR)
  term.tx(CR)

  term.str(string("Play (Y/N)?", CR))
  term.rxflush
  check := term.rx
  if (check == "y") or (check == "Y")
    check := checkfile(@TestFile)                               ' use validation method
    if (check <> 0)
      term.str(string("-- invalid format"))
    else
      term.str(string("-- playing (X to stop)", CR))
      
      wav.setvolume(100, 100)                                   ' okay (no change to samples)
      
      playcog := cognew(playwav(@TestFile, 2), @pcstack) + 1    ' start filling buffers
      term.rxflush
      repeat
        check := term.rxtime(25)
        if (check == "x") or (check == "X")
          term.str(string("-- playback terminated", CR))
          killplayer~~  
          quit
      while (playcog > 0)

  term.str(string(CR, "Done"))

  waitpeq(0, 0, 0)                                              ' low power mode 

 
pub pause(ms) | t

  t := cnt - 1088                                               ' sync with system counter
  repeat (ms #> 0)
    waitcnt(t += MS_001)

                        
pub checkfile(strpntr) | check

'' Ensures file is present and correct type (PCM, stereo, 16-bit)

  result := 0                                                   ' assume okay

  killplayer~~                                                  ' kill if running now
  repeat until (playcog == 0)                                   ' let it finish

  check := sd.popen(strpntr, "r")                               ' try to open
  if (check <> 0)
    result := -1

  if (result == 0)
    check := sd.pread(wav.headerbuf, 44)                        ' attempt to read header
    if (check <> 44)
      result := -2

  if (result == 0)
    if (wav.format <> 1)
      result := -3

  if (result == 0)
    if (wav.channels <> 2)                                   
      result := -4

  if (result == 0)
    if (wav.samplerate < 2000) or (wav.samplerate > 48000)
      result := -5

  if (result == 0)
    if (wav.bitspersample <> 16)
      result := -6

  sd.pclose

  return result    


pub playwav(pntr, plays) | sr, n

'' Spools WAV data to DACs engine
'' -- requires efx_wavdacs object       
'' -- this method should be launched into its own cog

  if (plays =< 0)
    plays := posx

  killplayer~                                                   ' clear the fill flag
    
  repeat plays
    sd.popen(pntr, "r")                                         ' open the file
    sd.pread(wav.headerbuf, 44)                                 ' read the header
    {
    if (wav.format <> 1)                                        ' double-check attributes
      quit
    if (wav.channels <> 2)
      quit
    if (wav.bitspersample <> 16)
      quit
    }  
    wav.startplayer(wav.samplerate)                             ' enable the player
     
    repeat 
      if killplayer                                             ' terminate early?
        quit
    
      if (wav.getbuf0 == 0)                                     ' if buf0 is empty
        n := sd.pread(wav.buf0addr, constant(wav#BUF_SIZE * 4)) ' read from file
        if (n > 0)                                              ' have bytes?
          wav.setbuf0(n >> 2)                                   ' yes, convert to samples
        else                                                    ' at end of file 
          wav.setbuf0(0) 
          quit                                                  ' we're done
          
      if (wav.getbuf1 == 0)
        n := sd.pread(wav.buf1addr, constant(wav#BUF_SIZE * 4))
        if (n > 0) 
          wav.setbuf1(n >> 2)
        else
          wav.setbuf1(0)
          quit

    sd.pclose                                                   ' close file
    wav.stopplayer                                              ' shutdown/clear buffers 
    
    if killplayer                                               ' early abort?
      plays~                                                    '  yes, do it
      quit

  wav.stopplayer   
  playcog~                                                      ' alert foreground
  cogstop(cogid)                                                ' unload this cog
  
 