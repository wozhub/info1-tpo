#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "estructuras.h"
#include "funciones.h"

void main() {
        t_mazo* m = nuevoMazo();
        llenarMazo(m);

        t_carta* c;
        struct t_jugador *j;
        
        int sockfd=iniciarServidor();

        while ( m->cantidad >= 7 )
        {
                j=recibirJugador(sockfd);

                printf("Se conecto %s.\n",j->nombre);
                
                int enviados;
                int cartas_enviadas=0;
                while ( cartas_enviadas < 7 ) {
                    c = (t_carta *) sacarCartaAzar(m);
                    enviarCarta(j->socket,c);
                    cartas_enviadas++;
                }
        }
}
