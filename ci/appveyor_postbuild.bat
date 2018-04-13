setlocal

for %%* in (.) do set CurrDirName=%%~nx*
cd build-%PLATFORM%
nmake INSTALL_ROOT=\projects\%CurrDirName%\install qtifw || exit /B 1
