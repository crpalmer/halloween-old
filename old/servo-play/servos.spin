CON

VAR
    long stack[10]
    long us
    byte pins[4]
    long pos_us[4]
    byte n
    long n_pulse
    
PUB Start
    us := clkfreq / 1_000_000
    cognew(work, @stack)

PUB alloc(pin)
    if n >= 4
      abort -1
    pins[n] := pin
    pos_us[n] := 1500 * us
    n++
    return n-1

PUB get_n_pulse
    return n_pulse
    
PUB set(pinid, pos)
    if pinid < 0 or pinid >= n
      abort -2
    pos_us[pinid] := pos * us

PUB set_percent(pinid, pct)
    if pinid < 0 or pinid >= n
      abort -3
    pct := 0 #> pct <# 100
    pos_us[pinid] := (500 + pct * 20) * us
    
PUB work | i, t, tf
  repeat
    tf := cnt + 20_000 * us
    repeat i from 0 to n-1
      dira[pins[i]] := 1
      outa[pins[i]] := 1
      waitcnt(cnt + pos_us[i])
      outa[pins[i]] := 0
      n_pulse ++
    waitcnt(tf)