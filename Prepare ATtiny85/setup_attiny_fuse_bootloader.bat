set AVRDUDE_PATH=C:\Users\jojo\Documents\ArduinoData\packages\arduino\tools\avrdude\6.3.0-arduino18\bin
set config-file=C:\Users\jojo\Documents\ArduinoData\packages\ATTinyCore\hardware\avr\1.5.2
set firmware_PATH=Micronucleus\firmware\releases
set port=COM4
set baudrate=19200
set partno=t85
set programmer=stk500v1 
set bitclock=20

REM UNCOMMENT THE BOOTLOADER YOU WANT
set BOOTLOADER=t85_default.hex
REM set BOOTLOADER=micronucleus-1.11-ledpb1.hex
REM set BOOTLOADER=micronucleus-1.02.hex

@echo off

:MENU
set CHOICE=0
CLS
echo ** INSTALL OF MICRONUCLEUS BOOTLOADER ON ATtiny85 **
echo.
ECHO ...............................................
ECHO PRESS 1, 2 to select your task, or 3 to EXIT.
ECHO ...............................................
echo.
echo 1. Burn the fuses configuration
echo 2. Burn the bootloader itself
echo 3. EXIT
echo.
set /P CHOICE="Your choice : "

if %CHOICE%==1 goto BURNFUSE
if %CHOICE%==2 goto FLASH

if %CHOICE%==3 (
    goto:eof
) else (GOTO OTHER)

:BURNFUSE
%AVRDUDE_PATH%\avrdude.exe -C %config-file%\avrdude.conf -v -p %partno% -c %programmer% -P %port% -b %baudrate% -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m -B %bitclock%
pause
GOTO MENU

:FLASH
%AVRDUDE_PATH%\avrdude.exe -C %config-file%\avrdude.conf -v -p %partno% -c %programmer% -P %port% -b %baudrate% -U flash:w:%firmware_PATH%\%BOOTLOADER%:i -B %bitclock%
pause
GOTO MENU

:OTHER
echo unknown command, please retry
pause
GOTO MENU