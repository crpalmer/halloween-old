CON

VAR
    long              moving
    long              position
    long              stack[32], stack2[32], stack3[32]

CON
    MIN_POS    =     1150
    RANGE      =      600

OBJ
    debug : "Parallax Serial Terminal"
    
PUB start(addr, servo, eyes)
    cognew(background_worker(addr, @position), @stack)
    cognew(background_servo(@position, servo, eyes), @stack2)
    ' cognew(background_printer(addr), @stack3)
    
PUB start_moving
    moving := 1

PUB stop_moving
    moving := 0

PUB background_worker(sample_addr, position_addr) | x,y
  repeat
    if moving
      x := LONG[sample_addr][2]
      position := ((x * RANGE) / 32768) + MIN_POS

PUB background_servo(addr, servo, eyes) | us, x
  dira[servo] := 1
  us := clkfreq / 1_000_000
  repeat
    x := cnt + 20_000 * us
    outa[servo] := 1
    waitcnt(cnt + positi`on * us)
    outa[servo] := 0
    waitcnt(x)

PUB background_printer(sample_addr) | x
    debug.start(115_200)
    repeat
      x := LONG[sample_addr][2]
      ||x
      if moving
      
        debug.dec(position)
        debug.str(string(" "))
      