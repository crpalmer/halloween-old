''DoReMi.spin
''Play C6, D6, E6, F6, G6, A6, B6, C7 as quarter notes quarter stops between.
CON
  _clkmode = xtal1 + pll16x ' System clock → 80 MHz
  _xinfreq = 5_000_000

VAR
  audio_value long 0
   
OBJ
  audio : "StereoDuty"
   
PUB TestFrequency | index
  audio.start(@audio_value, 0, 0)
  repeat
    audio_value := 1_000
    repeat until audio_value >= 15_000
      waitcnt(clkfreq / 4 + cnt)
      audio_value := audio_value + 1_000
