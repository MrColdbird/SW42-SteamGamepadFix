@ECHO OFF
:begin
SET /p backupConfirmation="Do you want to backup LINKDATA_1ST_ENG.BIN(y/n): "
IF /I %backupConfirmation% == n GOTO patch
ECHO %backupConfirmation%
IF /I NOT %backupConfirmation% == y GOTO begin
ECHO Creating backup
copy /b "%~dp0LINKDATA_1ST_ENG.BIN" "%~dp0LINKDATA_1ST_ENG.BIN.bak"
:patch
ECHO Applying patch[1/2]
"%~dp0LinkdataPatcher.exe" "%~dp0LINKDATA_1ST_ENG.BIN" "%~dp04.dat" 4
ECHO Applying patch[2/2]
"%~dp0LinkdataPatcher.exe" "%~dp0LINKDATA_1ST_ENG.BIN" "%~dp07.dat" 7
ECHO Patching complete
PAUSE