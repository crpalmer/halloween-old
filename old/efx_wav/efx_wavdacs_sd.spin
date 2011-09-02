'' =================================================================================================
''
''   File....... efx_wavdacs_sd.spin
''   Purpose.... WAV player that outputs to HUB for StereoDuty object
''   Author..... Jon "JonnyMac" McPhalen (aka Jon Williams)
''               Copyright (c) 2010 EFX-TEK
''               -- see below for terms of use
''   E-mail..... jon@jonmcphalen.com
''   Started.... 
''   Updated.... 20 JUN 2010
''
'' =================================================================================================

{{

   https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
   
}}



con

  BUF_SIZE = 128

  
var

  long  cog
  
  long  playflag
  long  srate
  long  buf0count 
  long  buf1count
  long  buf0[BUF_SIZE]
  long  buf1[BUF_SIZE]
  
  long  lvolume
  long  rvolume
  long  speed

  long  mintix

  byte  header[44]
  

pub main

  repeat
  
  
pub init(pntr) | ok

  finalize

  ' set cog parameters

  sdpntr    := pntr           
  pfpntr    := @playflag    
  srpntr    := @srate   
  b0cntpntr := @buf0count
  b1cntpntr := @buf1count 
  b0pntr    := @buf0
  b1pntr    := @buf1 
  lvolpntr  := @lvolume
  rvolpntr  := @rvolume

  setvolume(100, 100)
  setspeed(100)
  
  ok := cog := cognew(@wavdacs, 0) + 1                          ' start the cog

  return ok


pub finalize

'' Stops cog (if running)

  if cog
    stopplayer  
    cogstop(cog~ - 1)      


pub headerbuf

'' Returns address of WAV header buffer

  return @header
  

pub format | fmt

'' Returns format of WAV file

  fmt~                           
  wordmove(@fmt, @header[20], 1)

  return fmt
  

pub channels | ch

'' Returns channels in WAV file

  ch~                            
  wordmove(@ch, @header[22], 1)

  return ch


pub samplerate | sr

'' Returns sample rate (Hz) of WAV file

  sr~
  longmove(@sr, @header[24], 1)

  return sr


pub bitspersample  | bits

'' Returns bits per sample of WAV file

  bits~
  wordmove(@bits, @header[34], 1)

  return bits


pub samples | balign, chunk2

'' Returns samples in WAV file (chunk2)

  balign~
  wordmove(@balign, @header[32], 1)                             ' block align (bytes per sample)
  chunk2~
  longmove(@chunk2, @header[40], 1)                             ' bytes in chunk2 (audio)  

  return chunk2 / balign
  

pub duration

'' Returns duration of audio chunk in tenths of seconds

  return samples / (samplerate / 10)  


pub startplayer(rate) | tmp

'' Sets sample rate for driver and enables play flag

  if (speed == 100)
    srate := clkfreq / rate
  else
    srate := ((clkfreq / rate) * 100 / speed) #> (clkfreq / 44_100)
    
  playflag := true


pub playing

'' Returns status of play flag
'' -- driver will clear when finished playing

  return playflag


pub stopplayer

'' Stops player
'' -- clears play flag, rate, and play buffers

  playflag := false
  srate~
  clearbufs


pub setbuf0(c)

'' Sets # of samples in buf0
'' -- call AFTER loading buffer

  buf0count := c


pub getbuf0

'' Returns samples in buf0
'' -- driver will clear when buffer is finished

  return buf0count


pub buf0addr

'' Returns pointer to buf0

  return @buf0
  

pub setbuf1(c)

'' Sets # of samples in buf1
'' -- call AFTER loading buffer

  buf1count := c


pub getbuf1

'' Returns samples in buf1
'' -- driver will clear when buffer is finished

  return buf1count


pub buf1addr

'' Returns pointer to buf1

  return @buf1


pub setvolume(l, r)

'' Set playback levels for left and right channels, 0 to 100%

  l := (0 #> l <# 100)                                          ' keep in legal range
  if (l == 100)
    lvolume := $1_0000                                          ' avoid rounding errors
  else
    lvolume := l * $1_0000 / 100
   
  r := (0 #> r <# 100)
  if (r == 100)
    rvolume := $1_0000
  else
    rvolume := r * $1_0000 / 100


pub setspeed(spd)

'' Set playback speed, 50% to 200% 

  speed := 50 #> spd <# 200                                             
                             
    
pri clearbufs

  longfill(@buf0count, 0, (1 + BUF_SIZE) << 1)
  bytefill(@header, 0, 44)


dat

{ Based on original WAV player code by Raymond Allen } 

                        org     0

wavdacs
quiet                   wrlong  ZERO, sdpntr                    ' write silence to StereoDuty
                        
                        rdlong  tmp1, pfpntr            wz      ' read samples
        if_z            jmp     #quiet
        
                        rdlong  ratetix, srpntr         wz      ' sample rate tix
        if_z            jmp     #quiet
                        
                        call    #check0                         ' anything in 1st buffer?
        if_z            jmp     #quiet

                        ' ready to play

                        mov     smpltimer, cnt
                        add     smpltimer, ratetix

playloop                rdlong  tmp1, pfpntr            wz      ' check play flag
        if_z            jmp     #quiet
        
                        waitcnt smpltimer, ratetix              ' let sample timer expire
                        
getsamples              rdlong  lchan, bufpntr                  ' get left & right
                        mov     rchan, lchan                    ' copy right

cleanup                 and     lchan, hx0000_FFFF              ' restore to 16-bit
                        shr     rchan, #16

adjleft                 mov     arg1, lchan
                        rdlong  arg2, lvolpntr
                        call    #adjust
                        mov     lchan, arg1

adjright                mov     arg1, rchan
                        rdlong  arg2, rvolpntr
                        call    #adjust
                        mov     rchan, arg1

                        shl     rchan, #16                      ' move to high word
                        or      lchan, rchan                    ' reconstruct 


updateduty              wrlong  lchan, sdpntr                   ' update StereoDuty

                        add     bufpntr, #4                     ' point to next sample
                        djnz    bcount, #playloop               ' done with this buffer?

                        wrlong  ZERO, bcpntr                    ' clear this buffer for re-fill
                        test    bufid, #1               wc      ' which buffer just played?
                        
        if_c            call    #check0                         ' anything in buffer 0?
        if_nc           call    #check1                         ' anything in buffer 1?
        if_nz           jmp     #playloop                       ' we're still playing

                        wrlong  ZERO, pfpntr                    ' clear play flag
        if_z            jmp     #quiet                  


' check samples in buffer
' -- resets buffer id and pointer to buffer

check0                  mov     bufid, #0                       ' set buffer id
                        mov     bcpntr, b0cntpntr               ' point to count
                        rdlong  bcount, bcpntr          wz      ' get buffer samples count
                        mov     bufpntr, b0pntr                 ' point to element 0
check0_ret              ret


check1                  mov     bufid, #1
                        mov     bcpntr, b1cntpntr      
                        rdlong  bcount, bcpntr          wz         
                        mov     bufpntr, b1pntr         
check1_ret              ret



' Multiply signed, 16-bit channel data by unsigned (16-bit) adjustment
' -- adjustment expressed as percent (0.00 to 1.00) x $1_0000
'
' arg1 = channel value in (signed, 16-bit)
' arg2 = adjustment (unsigned)
' tmp1 = holds sign bit
' tmp2 = working variabl

'
adjust                  cmp     arg2, hx0000_FFFF       wc      ' 100%?
        if_a            jmp     #adjust_ret

                        cmps    arg2, #1                wc      ' 0%
        if_b            mov     arg1, #0
        if_b            jmp     #adjust_ret

                        shl     arg1, #16                       ' extend sign from bit15
                        sar     arg1, #16
                        abs     arg1, arg1              wc      ' get sign, force positive
                        muxc    tmp1, #1                        ' save sign

                        max     arg1, POSX16                    ' limit to 16-bit pos max
                        mov     tmp2, #0                        ' clear result workspace 
{                        
noisegate               tjz     noisegate, #:loop               ' if 0, skip it
                        cmp     arg1, noisegate         wc      ' 0.5% threshold
        if_b            mov     arg1, #0
        if_b            jmp     #adjust_ret
}                    
                                             
:loop                   shr     arg2, #1                wc      ' get multiplier bit
        if_c            add     tmp2, arg1                      ' if 1, add muliplicand
                        shl     arg1, #1                wc      ' adjust for next bit
                        tjnz    arg2, #:loop                    ' again if more bits
                        
                        test    tmp1, #1                wc      ' recall and restore sign
                        negc    arg1, tmp2
                        shr     arg1, #16                       ' back to 16 bits
adjust_ret              ret

' -------------------------------------------------------------------------------------------------                        

ZERO                    long    0
MAX_VOL                 long    $1_0000
POSX16                  long    $0_7FFF

hx0000_FFFF             long    $0000_FFFF                      ' for truncating to 16 bits
hx8000_0000             long    $8000_0000                      ' zero level out at 32 bits

sdpntr                  long    0-0                             ' samples address for StereoDuty

pfpntr                  long    0-0                             ' pointer to playflag
srpntr                  long    0-0                             ' pointer to sample rate ticks
b0cntpntr               long    0-0                             
b0pntr                  long    0-0                             
b1cntpntr               long    0-0                             
b1pntr                  long    0-0
lvolpntr                long    0-0
rvolpntr                long    0-0

ratetix                 res     1                               ' ticks in sample rate
smpltimer               res     1                               ' timer for samples
bufid                   res     1                               ' buffer in use
bcpntr                  res     1                               ' buffer count pointer
bcount                  res     1                               ' samples in buffer
bufpntr                 res     1                               ' pointer to buffer in use
lchan                   res     1                               ' left ch sample
rchan                   res     1                               ' right ch sample
lvol                    res     1                               ' left volume
rvol                    res     1                               ' right volume
noisegate               res     1                               ' cut-off threshold

arg1                    res     1                               ' arguement
arg2                    res     1                               ' arguement

tmp1                    res     1
tmp2                    res     1

                        fit     492


dat

{{

  Terms of Use: MIT License

  Permission is hereby granted, free of charge, to any person obtaining a copy of this
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify,
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be included in all copies
  or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
  OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

}}    