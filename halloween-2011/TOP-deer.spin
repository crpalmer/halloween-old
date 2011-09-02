{{
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐               
│ Talking Deer Skull                                                                                                          |                                                                                                                                                                                                                    
│                                                                                                                             │
│ Author: Christopher R. Palmer                                                                                               |
|
| This program is a basic talking skull on a propeller board.  It has an SD card to read audio from, an unamplified speaker
| jack connected to amplified speakers, a server moving the jaw and a pair of LEDs for eyes.
|
| 
│                                                                                                                             │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
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
 
PUB Deer | check, P, seed, last, last2, ms

  play_file.start(SD_P0, AUDIO_P0, VOLUME)

  skull.start(play_file.sampleAddress, SKULL_SERVO, SKULL_LEDS)
  
  last := -1
  last2 := -1
  seed := 1234567
  ms := clkfreq / 1000
  
  repeat
    ?seed
    P := ||seed
    P := P // 100
    P := P / 23
    if P <> last and P <> last2
      if P == 0
        skull.start_moving(12_000)
        check := \play_file.synchronously(STRING("laugh1.wav"))
      elseif P == 1
        skull.start_moving(20_000)
        check := \play_file.synchronously(STRING("laugh2.wav"))
      elseif P == 2
        skull.start_moving(12_000)
        check := \play_file.synchronously(STRING("hope.wav"))
      elseif P == 3
        skull.start_moving(24_000)
        check := \play_file.synchronously(STRING("forest.wav"))
      else
        skull.start_moving(11_000)
        check := \play_file.synchronously(STRING("steelers.wav"))
      skull.stop_moving
      last2 := last
      last := P
      ?seed
      P := ||seed
      P := P // 10000
      P := P + 2000
      waitcnt(cnt + ms * P)
                        