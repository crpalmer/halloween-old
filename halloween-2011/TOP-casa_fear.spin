{{
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐               
│ Casa Fear Zombie                                                                                                            |                                                                                                                                                                                                                    
│                                                                                                                             │
│ Author: Christopher R. Palmer                                                                                               |
|                                                                                                                             |
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
}}

CON

  _clkfreq = 80_000_000
  _clkmode = xtal1 + pll16x

  VOLUME = 100
  SD_P0 = 16
  AUDIO_P0 = 22
  PIR = 0
  CYLINDER_P0 = 8

VAR
  long       ms
  
OBJ

  play_file : "play_file"
 
PUB CasaFearZombie | check, P, seed, last, last2

  play_file.start(SD_P0, AUDIO_P0, VOLUME)

  last := -1
  last2 := -1
  seed := 1234567
  ms := clkfreq / 1000

  repeat
    seed := wait_pir_active(seed)
    ?seed
    track := seed // N_TRACKS

  
PRI wait_pir_active(seed) | n
  n := 0
  repeat
    ?seed
    if ina[PIR]
      n++
    else
      n := 0
    waitcnt(cnt + 10*ms)
  until n > 5
   
  return seed
