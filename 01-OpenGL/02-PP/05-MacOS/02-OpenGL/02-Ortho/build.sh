clear

clang -Wno-deprecated-declarations -c -o OGL.o OGL.m

mkdir -p OGL.app/Contents/MacOS

clang -o OGL.app/Contents/MacOS/OGL OGL.o -framework Cocoa -framework QuartzCore -framework OpenGL
