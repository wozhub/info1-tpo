#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "estructuras.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
        /* Tratamiento de la linea de comandos. */
        if (argc != 2) {
                perror("El programa toma como argumento el nombre o la dirección del servidor.\n");
                exit(1); }

        char nombre[]="Pepitote";

        int sockfd = iniciarCliente(argv[1],nombre);

        t_carta *carta;

        int cartas_recibidas=0;
        while (cartas_recibidas < 7) {
            carta=recibirCarta(sockfd);

            printf("Recibi un %d de %d!\n",carta->valor,carta->palo);
            cartas_recibidas++;
        }
        
        /* Devolvemos recursos al sistema */
        close(sockfd);
        return 0;
}


