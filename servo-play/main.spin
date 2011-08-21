CON
  _clkmode = xtal1 + pll16x ' System clock → 80 MHz
  _xinfreq = 5_000_000

VAR
   long pos, newpos
   long head
   long dir
    
OBJ
  debug : "Parallax Serial Terminal"
  servos : "servos"

PUB TestServos | next_at, i

  debug.start(115200)

  servos.start
  head := servos.alloc(0)
  dir := 100
  pos := 1500

  dira[0] := 1
  
  repeat
    next_at := cnt + clkfreq / 50
    debug.str(string(" | after "))
    debug.dec(servos.get_n_pulse)
    debug.str(string(" moving head "))
    debug.dec(head)
    debug.str(string(" to "))
    debug.dec(pos)
    debug.str(string(" dir "))
    debug.dec(dir)
    servos.set(head, pos)
    'outa[0] := 1
    'waitcnt(cnt + clkfreq / 1_000_000 * pos)
    'outa[0] := 0
    newpos := pos + dir
    if newpos < 500
      dir := 100
    elseif newpos > 2300
      dir := -100
    pos := pos + dir
     waitcnt(next_at)