#!/bin/bash
mkdir ./bin
gcc -o ./bin/write src/write.c src/ipc.c
gcc -o ./bin/read src/read.c src/ipc.c
gcc -o ./bin/bc.c src/bc src/ipc.c -I ./ws/usr/local/include/wsserver -L ./ws/usr/local/lib64/ -lws
