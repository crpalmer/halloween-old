{{
}}

CON

  _clkfreq = 80_000_000
  _clkmode = xtal1 + pll16x

  VOLUME = 100
  SD_P0 = 16
  AUDIO_P0 = 24
  SKULL_SERVO = 7
  SKULL_LEDS = 6

OBJ

  skull : "talking_skull"
  play_file : "play_file"
 
PUB Deer | check, P, seed, last, last2, ms

  play_file.start(SD_P0, AUDIO_P0, VOLUME)

  skull.start(play_file.sampleAddress, SKULL_SERVO, SKULL_LEDS)
  
  seed := 1234567
  ms := clkfreq / 1000
  
  repeat
    ?seed
    P := ||seed
    P := P // 20000 + 5000
    waitcnt(cnt + ms * P)
    skull.start_moving
    play_file.synchronously(STRING("annie.wav"))
                        