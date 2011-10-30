{{
}}

CON

  _clkfreq = 80_000_000
  _clkmode = xtal1 + pll16x

  VOLUME = 100
  SD_P0 = 0
  AUDIO_P0 = 22
  SKULL_SERVO = 7
  SKULL_LEDS = 6

OBJ

  skull : "talking_skull"
  play_file : "play_file"
  'debug : "Parallax Serial Terminal"
 
PUB Deer | check, P, seed, last, last2, ms

  'debug.start(250000)
  play_file.start(SD_P0, AUDIO_P0)
  skull.start(1100, 850, play_file.sampleAddress, SKULL_SERVO, SKULL_LEDS, 1)
  
  seed := 1234567
  ms := clkfreq / 1000
  
  repeat
    play_file.synchronously(STRING("annie.wav"), 1)
    ?seed
    P := ||seed
    P := P // 20000 + 5000
    waitcnt(cnt + ms * P)
                        