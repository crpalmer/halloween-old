
Rogue Robotics uMP3 Firmware Update Procedure
---------------------------------------------


If there is a new firmware update for the uMP3, it may be downloaded from 
http://www.roguerobotics.com/.  Once downloaded, it can be sent to the uMP3 
using the "update" program.  They will be packaged together in a Zip compressed 
file.

You will need to connect the uMP3 to a PC serial port through a TTL Level 
converter, such as a MAX232.  THE uMP3 WILL BE DAMAGED IF CONNECTED DIRECTLY TO 
A PC SERIAL PORT!  More than likely you'll be using a USB to Serial port, but
the same still applies.  You can also find a USB to TTL Serial device such as
the Pololu USB-2-Serial adapter.

To put the uMP3 into bootloader mode and download the firmware:

*	Disconnect power to the uMP3, and remove any MMC/SD card from the uMP3.

*	Use a small flat-blade screwdriver to bridge the "UPD" jumper, and continue to 
  hold the screwdriver in place.

*	Connect the power to uMP3.  The Activity LED will stay illuminated (longer 
  than 2 seconds).  You can remove the screwdriver.

*	Start the update program from the command line (Start -> Run -> "cmd.exe" or 
  "command.exe"):

  *	"update ump3-111-00.rfw -COM1" (if your serial port is something other than 
    COM1, use "update ump3-111-00.rfw -COMn", where COMn is your com port, e.g. -COM2)

The update program will show the progress and you will see the uMP3 Activity LED 
blink as the firmware is updated.  Once complete, the uMP3 will reset and start 
normally.

When the uMP3 is put into bootloader mode, all settings for the Settings command 
are reset to default values (this is so that you can reset the uMP3 if an 
unknown value is put in any of the settings).  You do not have to download 
anything to reset the values.  Just simply put the uMP3 into bootloader mode 
(explained above), then remove the power to the uMP3.  The values will be reset.
