#!/bin/bash

if [ ! -d "$DIRECTORY" ]; then
  mkdir build
fi

g++ -std=c++11 -o build/pong pong.cpp -lSOIL -lGL -lGLU -lglut -fPIC

