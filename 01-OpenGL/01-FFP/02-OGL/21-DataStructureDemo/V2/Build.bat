@echo off
CLS

@REM creating bin folder which does not exist

if not exist "bin" (
	mkdir "bin"
	echo Bin folder created successfully.
) else (
	echo Bin folder already exists.
)

echo =====================        Cleaning Up         ============================

del OGLMain.exe 
del ".\\bin\\*.obj"
del ".\\bin\\OGLMain.res"

echo =====================    Compilation started     ============================

@REM compiling all available cpp files in directory

for /r %%i in (*.cpp) do (
	echo "%%i"
	cl.exe /Fo".\\bin\\" /c /EHsc /I "C:\Program Files (x86)\OpenAL 1.1 SDK\include" "%%i"
)

echo =====================    Compilation completed   ============================

rc.exe /Fo".\\bin\\OGLMain.res" OGLMain.rc

echo =====================      Linking started       ============================

link.exe /out:OGLMain.exe ./bin/*.obj ./bin/*.res /LIBPATH:"C:\Program Files (x86)\OpenAL 1.1 SDK\libs\Win64" user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

echo =====================      Linking ended         ============================




