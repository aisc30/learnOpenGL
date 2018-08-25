#!/bin/sh
g++ src/glad.c src/task2_gl.cpp -o test_gl -lglfw3 -lX11 -lXxf86vm  -lXrandr -lpthread -lXi -lXcursor -lXinerama -ldl -lm -lGL -Iinclude  
./test_gl
