@echo off


set startpath=%cd%
set startdrive=%CD:~0,2%

C:
cd C:\ultra
call setup.bat >NUL

%startdrive%
cd %startpath%
exew32 objdump %OBJDUMPCOMMAND%

