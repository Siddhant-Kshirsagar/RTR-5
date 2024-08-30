

clang++ -Wno-deprecated-declarations -c OGL.o OGL.mm Sphere.cpp -std=c++17

mkdir -p OGL.app/Contents/MacOS

clang++ -o OGL.app/Contents/MacOS/OGL OGL.o Sphere.o -framework Cocoa -framework QuartzCore -framework OpenGL



