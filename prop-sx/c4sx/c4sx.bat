@echo	off
rem
rem	c4sx  ( based CC1B  compile batch file, many thanks to Bruce Ray)
rem	c4sx.bat version 2 (09/03/2007)
rem
rem	now uses CC1B return codes to determine if compile errors occured.
rem	no longer supports tthe -f 'force mode"
rem
rem	usage:
rem	to run with CC1B2SX converter (default/preferred mode):
rem		C44SX <source_name>
rem
rem
rem note that the C4SX_PATH directory contains both the CC1B2SX executable and the C4SX include files.
rem this unusual configuration will be changed in later versions of C4SX
rem
setlocal

title "C4SX Compile Window"

set CC1B_PATH=C:\Program Files\bknd\CC1B
set C4SX_PATH=C:\C4SX

if exist tmp  del tmp

rem call CC1B compiler
call "%CC1B_PATH%\CC1B" -I"%CC1B_PATH%" -I"%C4SX_PATH%" -V -atmp  %1.c

echo .
echo .  CC1B finished
if %ERRORLEVEL% NEQ 0 goto :BAD_COMPILE
if not exist tmp  goto :BAD_COMPILE
echo .  compile successful

rem call CC1B2SX utility
rem call CC1B2SX -s < tmp  > %1.src
call %C4SX_PATH%\CC1B2SX   < tmp  > %1.src 

echo .
echo .  CC1B2SX finished

:CLEANUP
del tmp
goto :DONE

:BAD_COMPILE
echo .  compile errors exist
goto :DONE

:DONE
endlocal
rem	end of 'c2sx.bat'
