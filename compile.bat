@echo off

echo.
echo This batch file compiles a given source file in C and outputs the executable in the terminal
echo.

REM Set the directory to the location of the batch file
cd /D "%~dp0"

:run_file
REM set /p fileName="File Name: "
set beginCompTime=%time%

set srcFile=armyarraystructv1.c
set exeFile=%fileName%.exe

:compile_file
gcc -o %exeFile% %srcFile% -O3

set endCompTime=%time%


echo ------------------------------------------------------------------------------
echo.

%exeFile%
echo.
echo ------------------------------------------------------------------------------

REM echo Began Compiling:    %beginCompTime%
REM echo Finished Compiling: %endCompTime%
REM echo Exe finished at:    %time%

echo Re-compile?
echo.

set /p YorN="Y/N: "

if %YorN%==y goto:compile_file

echo Run a different file?
echo.

set /p YorN="Y/N: "

if %YorN%==y goto:run_file