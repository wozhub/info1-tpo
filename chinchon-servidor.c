/* Trabajo Practico Obligatorio
 * Informática I - 2013 - Prof. Martín Marino
 * Integrantes: Ranú, Iannuzzi, Vinazza */ 

#include <stdio.h>
#include <stdlib.h> // exit

#include "estructuras.h"
#include "funciones.h"

#define J_MAXJUGADORES 2 /* tambien hay que ponerlo en funciones.c */
#define J_MAXPUNTAJE 100

static char* palos[] = { "Comodín","Espada","Oro","Basto","Copa" };

void main() {
    char i,j,signalAux, puntajeMayor=-100, cortaron=0;
    int puntos;
    unsigned char turno, ronda, cartas_recibidas, hubo_chinchon=0;
    t_jugador* jugadores[J_MAXJUGADORES];

    t_carta *cartaAux, *pozoAux;

    printf("Creando mazo y pozo...\n");
    /* El mazo lo creo y una sola vez. Después vamos a ir manejando las cartas
     * como para no perderlas. El pozo es un mazo vacío. */
    t_mazo *mazo=nuevoMazo(); llenarMazo(mazo);
    t_mazo *pozo=nuevoMazo(); t_mazo *mazoAux;

    printf("Iniciando servidor.\n");
    int sockServidor = iniciarServidor();

    for (i=0; i<J_MAXJUGADORES; i++) {
        printf("Esperando jugador %d/%d...\n",i,J_MAXJUGADORES);
        jugadores[i] = recibirJugador(sockServidor);
        printf("Se conecto %s.\n",jugadores[i]->nombre);
    }

    printf("Iniciando el juego\n.");

    while ( puntajeMayor <= J_MAXPUNTAJE && hubo_chinchon == 0 ) {

        printf("El puntaje mayor hasta ahora es %d\n",puntajeMayor);
        
        printf("Reparto las cartas que le corresponden a cada jugador.\n");
        repartirCartas(jugadores,mazo); //reparto 7 cartas a cada jugador
        
        printf("Saco la primera carta del mazo y la paso al pozo.\n");
        pozoAux=sacarCartaAzar(mazo); //saco una carta del mazo al pozo 
        agregarCarta(pozo, pozoAux);

        cortaron=0;
        while ( cortaron==0 ) { //empiezan los turnos
            i++;
            if (i>=J_MAXJUGADORES) { i=0; } //loop circular

            printf("Es el turno de %s [%d]\n",jugadores[i]->nombre,i);

            /* Si no hay cartas en el mazo, intercambio los punteros con el pozo.
             * Asi, una vez jugadas todas las cartas del mazo, se puede continuar.
             * No hace falta mezclar las cartas porque siempre las sacamos al azar! */
            if (mazo->cantidad == 0) {
                printf("No hay mas cartas en el mazo. Hay %d cartas en el pozo. Intercambiando.\n",pozo->cantidad);
                pozoAux= (t_carta *) sacarCarta(pozo);
                mazoAux=pozo; pozo=mazo; mazo=mazoAux;
                agregarCarta(pozo,pozoAux); }

            enviarSenial(jugadores[i]->socket, 1);              //aviso al cliente que carta está en el pozo.
            enviarSenial(jugadores[i]->socket, pozoAux->palo);  //lo hacemos mediante una señal para evitar borrar la carta al enviarla
            enviarSenial(jugadores[i]->socket, pozoAux->valor);
    
            printf("Esperando opcion.\n");
            signalAux=recibirSenial(jugadores[i]->socket);
            switch ( signalAux ) {
                case 0: 
                    printf("%s Eligio mazo.\n",jugadores[i]->nombre);
                    cartaAux=sacarCartaAzar(mazo);
                    enviarCarta(jugadores[i]->socket, cartaAux);
                    break;
                case 1: //podriamos usar default y ademas no poner break en el ultimo
                    printf("%s Eligio pozo.\n",jugadores[i]->nombre);
                    cartaAux=(t_carta*) sacarCarta(pozo);
                    enviarCarta(jugadores[i]->socket, cartaAux);
                    break;
            }

            signalAux=recibirSenial(jugadores[i]->socket);
            cartaAux=recibirCarta(jugadores[i]->socket);
            printf("%s tiro un %d de %s.\n",jugadores[i]->nombre,cartaAux->valor,palos[cartaAux->palo]);

            switch ( signalAux ) {
                case 0:
                    printf("%s sigue jugando.\n",jugadores[i]->nombre);
                    agregarCarta(pozo,cartaAux);
                    pozoAux=cartaAux;
                    break;
                case 1:
                    printf("%s decidio cortar.\n",jugadores[i]->nombre);
                    agregarCarta(mazo,cartaAux);

                    for (i=0; i<J_MAXJUGADORES; i++) {
                        printf("Esperando juegos de %s.\n",jugadores[i]->nombre);
                        puntos=recibirJuegos(jugadores[i]->socket,mazo);

                        if (puntos==-20) { 
                            printf("%s hizo chinchon!\n",jugadores[i]->nombre);
                            hubo_chinchon=1; puntos=-10;
                        }

                        jugadores[i]->puntaje+=puntos;

                        printf("%s hizo %d puntos y ahora tiene %d puntos.\n",jugadores[i]->nombre, puntos, jugadores[i]->puntaje); }

                    cortaron=1; //para salir del while

                } //ciera el switch
            } //cierra while cuando cortan 

        //paso todas las cartas del pozo al mazo.
        while ( pozo->cantidad > 0 ) {
            cartaAux=(t_carta*) sacarCarta(pozo);
            agregarCarta(mazo,cartaAux);
        }

        /* Verificamos que no hayamos perdido cartas en el intercambio.
         * En el mundo real, contar las cartas sería tedioso y de desconfiado...
         * pero acá no nos cuesta nada y no se entera nadie. */
        if ( mazo->cantidad != 50 ) {
            printf("en mazo=%d.\n",mazo->cantidad);
            perror("Se te perdieron cartas en el camino.\n"); 
            exit(1); }


        for (i=0; i<J_MAXJUGADORES; i++) {
            if ( jugadores[i]->puntaje > puntajeMayor ) {
                puntajeMayor=jugadores[i]->puntaje;
            }
        }
            
    }
    /* El while cierra cuando alguien pierde por puntos o alguien gana por chinchón.
     * Queda pendiente escribir un estado más en el cual el servidor envia al cliente 
     * los datos de los jugadores para informar quien gano/perdio. */

    printf("Notificando a los clientes que el juego termino.\n");
    for (i=0; i<J_MAXJUGADORES; i++) {
        printf("Chau, %s!",jugadores[i]->nombre);
        enviarSenial(jugadores[i]->socket,3);
        if ( recibirSenial(jugadores[i]->socket) == 1 ) {
            printf("Adios.\n");
        }
        close(jugadores[i]->socket);
    }

}
