#!/bin/bash 

gcc funciones.c chinchon-servidor.c -o binarios/chinchon-servidor.bin 
gcc funciones.c interfaz.c chinchon-cliente.c -o binarios/chinchon-cliente.bin -lncurses
