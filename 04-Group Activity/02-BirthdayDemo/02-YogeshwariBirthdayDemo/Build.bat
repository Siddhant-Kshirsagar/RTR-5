CLS

cl.exe /c /EHsc OGL.cpp SukrutScene.cpp
rc.exe OGL.rc
link.exe OGL.obj SukrutScene.obj OGL.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
OGL.exe

