'  Note: Remove comment from 'mov ctrb,ctrb_' for stereo operation.
'        It's commented out because the demo doesn't output right channel samples (just hash noise in the low word).
'
'  written by Chip Gracey
'  -- modified by JonnyMac to match WAV channel assignments 


CON

  ditherdown = 4      'Use 1..8, 1=most dither, 8=least dither. The further you get from cog0, the more dither you need.


VAR

  long  cog


PUB start(sample_ptr, left_pin, right_pin) : okay

'' Start driver - starts a cog
'' returns false if no cog available
''
''   sample_ptr = pointer to long which contains signed R:L 16-bit samples updated at 44,100Hz
''     left_pin = left channel output pin
''    right_pin = right channel output pin

  'Reset driver
  stop                                                    

  'Set pins
  dira_ := |< left_pin | |< right_pin
  ctra_ := $18000000 + left_pin
  ctrb_ := $18000000 + right_pin

  'Launch stereo pwm cog
  return cog := cognew(@entry, sample_ptr) + 1
  'coginit(7, @entry, sample_ptr)  'uncomment this line and comment the prior line to hear it run on cog7


PUB stop

'' Stop driver - frees a cog

  'If already running, stop cog
  if cog                                               
    cogstop(cog~ -  1)


DAT                      
                        org

entry                   mov     dira,dira_              'set dira bits

                        mov     ctra,ctra_              'set ctr's to output duty to pins
                        mov     ctrb,ctrb_

:loop                   rdlong  lsample,par             'read samples
                        xor     lsample,h80008000       'convert signed samples to unsigned
                        mov     rsample,lsample         'split them apart and msb-justify them
                        shl     lsample,#16
                        and     rsample,hFFFF0000

                        test    lfsr,taps       wc      'iterate noise generator
                        rcl     lfsr,#1

                        mov     dither,lfsr             'make dither from noise
                        sar     dither,#ditherdown

                        add     lsample,dither          'add dither to samples
                        add     rsample,dither

                        mov     frqa,lsample            'output samples
                        mov     frqb,rsample

                        jmp     #:loop                  'loop


dira_                   long    0
ctra_                   long    0
ctrb_                   long    0

h80008000               long    $80008000
hFFFF0000               long    $FFFF0000
taps                    long    $80000062
lfsr                    long    1

lsample                 res     1
rsample                 res     1
dither                  res     1

