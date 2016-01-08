echo off
if exist 0err0 del 0err0
if exist image~.bmp del image~.bmp
if exist image~.wav del image~.wav
if exist sound~.wav del sound~.wav
if exist sound~.bmp del sound~.bmp
if exist arss.log del arss.log
del .\obj\*.o
del *.exe
pause
