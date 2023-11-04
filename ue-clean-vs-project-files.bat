@echo off
echo *********************************************************************************************************************
echo ******************************************** Clean Unreal Engine Project ********************************************
echo *********************************************************************************************************************
echo.
:PROMPT
echo Cleaning your project, please wait...
FOR /d /r %%d IN ("Binaries","Intermediate","Saved","DerivedDataCache") DO @IF EXIST "%%d" rd /s /q "%%d"
del *.sln
echo.
echo Your project cleaned perfectly, you can click Generate Visual Studio project files now.
pause
:END