clear
g++ -c -o OGL.o -I /usr/include OGL.cpp 
g++ -o OGL -L /usr/lib/x86_64-linux-gnu OGL.o -lGLEW -lX11 -lGL -lSOIL 


