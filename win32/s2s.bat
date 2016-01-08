echo off
if exist %1~.bmp del %1~.bmp
if exist %1~.wav del %1~.wav
asperes -m anal -f %1.wav -o %1~.bmp
asperes -m sine -f %1~.bmp -o %1~.wav
pause
