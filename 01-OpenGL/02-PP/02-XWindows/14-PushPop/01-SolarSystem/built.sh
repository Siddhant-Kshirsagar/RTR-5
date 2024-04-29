clear
g++ -c -o OGL.o -I /usr/include OGL.cpp 
g++ -o OGL OGL.o -L . -lX11 -lGL -lGLEW -lSphere


