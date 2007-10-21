@echo off
gcc -D__USE_FIXED_PROTOTYPES__ -O2 -fstrength-reduce comptest.c -o comptest 
if errorlevel == 1 goto error
del trash_me.err >nul
goto ok
:error
echo ERROR > trash_me.err
:ok
