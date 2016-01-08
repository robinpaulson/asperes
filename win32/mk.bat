echo off
echo Making the ARSS utility...
if exist 0err0 del 0err0
.\util\redir.exe -e 0err0 %MINGW%\bin\mingw32-make.exe CCDIR=%MINGW%
if errorlevel 1 .\util\win32pad 0err0
