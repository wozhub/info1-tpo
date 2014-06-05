#!/bin/bash 

mkdir binarios
gcc funciones.c interfaz.c prueba-interfaz.c -o binarios/prueba-interfaz.bin -lncurses
gcc funciones.c prueba-mazo.c -o binarios/prueba-mazo.bin
gcc funciones.c prueba-ordenar-juego.c -o binarios/prueba-ordenar-juego.bin
gcc funciones.c prueba-verif-juegos.c  -o binarios/prueba-verif-juegos.c
gcc funciones.c prueba-red-mazo-cliente.c -o binarios/prueba-red-mazo-cliente.bin
gcc funciones.c prueba-red-mazo-servidor.c -o binarios/prueba-red-mazo-servidor.bin 
