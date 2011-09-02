VAR
  long pin
  long addr
  long stack[10]

PUB start(_pin, _addr)
  pin := _pin
  addr := _addr
  cognew(main, @stack)

PUB main
   