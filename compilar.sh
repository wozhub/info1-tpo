#!/bin/bash 

#compila el servidor
gcc funciones.c chinchon-servidor.c -o binarios/chinchon-servidor.bin 

#compila el cliente
gcc funciones.c interfaz.c chinchon-cliente.c -o binarios/chinchon-cliente.bin -lncurses
