{{
}}

CON

  _clkfreq = 80_000_000
  _clkmode = xtal1 + pll16x

  ANNIE_MIN_POS = 1100
  ANNIE_RANGE = 850
  
  VOLUME = 100
  SD_P0 = 0
  AUDIO_P0 = 22
  SKULL_SERVO = 7
  SKULL_LEDS = 6

  PIR = 16
  BUTTON = 17
  MANUAL = 18

OBJ

  skull : "talking_skull"
  play_file : "play_file"
  'debug : "Parallax Serial Terminal"
 
PUB Deer | P, seed, ms, buttons, pirs, pir_blocked

  'debug.start(250000)
  play_file.start(SD_P0, AUDIO_P0)
  skull.start(ANNIE_MIN_POS, ANNIE_RANGE, play_file.sampleAddress, SKULL_SERVO, SKULL_LEDS, 1)
  
  seed := 1234567
  ms := clkfreq / 1000
  
  repeat
    buttons := 0
    pirs := 0
    pir_blocked := 5

    repeat while ina[MANUAL] and buttons < 100
      buttons := buttons * ina[BUTTON] + ina[BUTTON]
      pirs := pirs * ina[PIR] + ina[PIR]
      ?seed
      'debug.dec(ina[MANUAL])
      'debug.dec(ina[BUTTON])
      'debug.dec(ina[PIR])
      'debug.dec(buttons)
      'debug.str(STRING(","))
      'debug.dec(pirs)
      'debug.str(STRING(","))
      'debug.dec(pir_blocked)
      'debug.str(STRING(" "))
    
      if pirs > 100 and pir_blocked == 0
        pir_blocked := 30000
        play_file.synchronously(STRING("push.wav"), 1)
        
      waitcnt(cnt + ms)
      if pir_blocked > 0
        pir_blocked := pir_blocked - 1
        pirs := 0

    play_file.synchronously(STRING("wheels.wav"), 1)
    ?seed
    if not ina[MANUAL]
      P := ||seed
      P := P // 20000 + 5000
      waitcnt(cnt + ms * P)
                        