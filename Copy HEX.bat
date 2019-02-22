@echo off

for /f "skip=14 delims=" %%a in (USER\main.c) do (
set si=%%~a
goto :site
)
:site
set si=%si:~-5%
set si=%si:~0,4%
echo %si%

for /f "skip=112 delims=" %%a in (USER\include.h) do (
set id=%%~a
goto :project
)
:project
set id=%id:~-38%
set id=%id:~0,17%
echo %id%

copy ".\Project\Output\*.hex" ".\%id%%si%.hex"

pause
