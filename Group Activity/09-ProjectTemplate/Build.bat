CLS
cl.exe /c /EHsc OGLMain.cpp
rc.exe OGLMain.rc
link.exe OGLMain.obj OGLMain.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
OGLMain.exe

