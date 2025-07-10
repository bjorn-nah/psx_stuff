@echo off

set PS1trDir="C:\dev\psx\ps1transfert\"
set Target="shadow25"

REM suppression du précédent build
set REP=%~dp0
del %REP%build

REM build du projet
cmake -S . -B ./build -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=C:\dev\psx\PSn00bSDK-0.24-win32\lib\libpsn00b\cmake\sdk.cmake
cmake --build ./build


echo "envoi a la console"
pause
%PS1trDir%ps1transfer -e -f %REP%build\\%Target%.exe