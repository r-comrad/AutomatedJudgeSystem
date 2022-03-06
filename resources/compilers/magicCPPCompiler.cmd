::SET VS=C:\VTS
::echo %VS%
::if		exist %VS% echo 0 Folder already exists
::if	not exist %VS% echo 1 Folder does not exist
::pause

::set arg1=%1
::set arg2=%2
::SET VS=C:\Program Files (x86)\Microsoft Visual Studio\2019
::CALL "%VS%\Community\Common7\Tools\VsDevCmd.bat"
::cl "%arg1%" /Fe%arg2%


set arg1=%1
set arg2=%2

SET VS=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat
::if exist %VS% goto VisualStudio2019
if exist %VS% goto Compile

SET VS=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat
::if exist %VS% goto VisualStudio2019
if exist %VS% goto Compile

goto NoCompiler

::VisualStudio2019
::echo Take compiler from %VS%
::goto Compile

::VisualStudio2022
::echo Take compiler from %VS%
::goto Compile

:NoCompiler
echo ERROR: Compiler not found!
::goto CommonExit
SET VS=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat

:Compile
echo Take compiler from %VS%
CALL "%VS%"
cl "%arg1%" /Fe%arg2%

:CommonExit