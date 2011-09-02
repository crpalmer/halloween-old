CON

VAR
    long              moving
    long              max_value
    long              position, sample, accel
    long              stack[32], stack2[32], stack3[32]

CON
    MIN_POS    =      1100
    RANGE      =       700
    SAMPLE_SHIFT =      10

OBJ
    debug : "Parallax Serial Terminal"
    
PUB start(addr, servo, eyes)
    cognew(background_worker(addr, @position), @stack)
    cognew(background_servo(@position, servo, eyes), @stack2)
    cognew(background_printer, @stack3)
    
PUB start_moving(_max)
    moving := 1
    max_value := _max

PUB stop_moving
    moving := 0

PUB background_worker(sample_addr, position_addr) | x,y
  repeat
    position := MIN_POS
    accel := 0

    repeat until moving

    accel := 0
    repeat while moving
      y := 0
      repeat (1<<SAMPLE_SHIFT)
        x := LONG [sample_addr] & $ffff
        ~~x
        ||x
        y := y + x
      sample := y >> (SAMPLE_SHIFT)
      if sample > accel
        accel := sample
      position := (max_value <# sample) * RANGE / max_value + MIN_POS

PUB background_servo(addr, servo, eyes) | us, x
  dira[servo] := 1
  us := clkfreq / 1_000_000
  repeat
    x := cnt + 20_000 * us
    outa[servo] := 1
    waitcnt(cnt + position * us)
    outa[servo] := 0
    waitcnt(x)

PUB background_printer
    debug.start(115_200)
    repeat
      if moving
        'debug.str(STRING("("))
        debug.dec(position)
        debug.str(STRING(", "))
        debug.dec(sample)
        'debug.str(STRING(" <--> "))
        'debug.dec(threshold)
        debug.str(STRING(", "))
        debug.dec(accel)
        'debug.str(STRING(") "))
        debug.str(STRING(" "))
      waitcnt(cnt + clkfreq / 10)
      