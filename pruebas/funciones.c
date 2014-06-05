
/* Trabajo Practico Obligatorio
 * Informática I - 2013 - Prof. Martín Marino
 * Integrantes: Ranú, Iannuzzi, Vinazza */ 

#include <stdio.h> // perror 
#include <string.h>
#include <time.h> // rand 
#include <malloc.h> // malloc
#include <stdlib.h> // exit
#include <errno.h> // perror
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "estructuras.h"
#include "funciones.h"

#define RED_PUERTO 5678    // Puerto de red por donde se comunicaran los clientes y el servidor
#define RED_BUFFER 2     // Cola de conexiones en espera
#define J_MAXJUGADORES 2 /* tambien hay que ponerlo en chinchon-servidor.c */


static char* palos[] = { "Comodín","Espada","Oro","Basto","Copa" };

/*
 * Funciones relacionadas con la creación del mazo y su utilización.
 *
 */ 

void llenarMazo(t_mazo *mazo) {
    srand(time(NULL));

    if ( mazo != NULL ) { 
        unsigned char i,j; //tipos optimizados para las cartas i es el palo y j el valor

        for(i=1;i<=4;i++) { //recorro los cuatro palos
            for(j=1;j<=12;j++) { //las doce cartas
                    agregarCarta(mazo, nuevaCarta(i,j) ); 
            }
        } 

        agregarCarta(mazo, nuevaCarta(0,25) ); //pongo los comodines en el mazo. 
        agregarCarta(mazo, nuevaCarta(0,25) ); //tienen valor 25
    }
}

t_mazo* nuevoMazo() {
    t_mazo *mazo=malloc(sizeof(t_mazo));
    if (mazo != NULL ) {
        //el mazo arranca vacío
        mazo->primera=NULL; mazo->ultima=NULL; mazo->cantidad=0; }
    return mazo;
}

t_carta* nuevaCarta(char palo, char valor) {
    t_carta *carta=malloc(sizeof(t_carta));
    if (carta != NULL) { 
        //d printf("Creando el %d de %d\n.", (int) valor, (int) palo);
        carta->palo=palo;
        carta->valor=valor; 
        carta->siguiente=NULL; }
    return carta;
}

void agregarCarta(t_mazo* mazo, t_carta* carta) {
    /* En caso de que no se pueda crear la carta y reciba un puntero a null, no lo agrego al mazo */
    if ( carta != NULL ) {
        //d printf("Agregando el %d de %d al mazo.\n", (int) carta->valor, (int) carta->palo);
        if ( mazo->cantidad == 0 ) { /* El mazo esta vacio */
            mazo->primera=carta; 
            mazo->ultima=carta; }
        else {
            carta->siguiente=mazo->primera;
            mazo->primera=carta; }

        mazo->cantidad++; }
}

t_carta* sacarCarta(t_mazo* mazo) {
    t_carta *carta=NULL;
    if ( mazo->cantidad != 0 ) { /* El mazo no esta vacio */ 
        carta=mazo->primera;
        mazo->primera=carta->siguiente; 
        carta->siguiente=NULL; //safety first
        mazo->cantidad--; }
    return carta;
}

t_carta* sacarCartaAzar(t_mazo *mazo) {
    t_carta *carta=NULL;

    if ( mazo->cantidad > 0 ) {
        int carta_numero = rand() % mazo->cantidad;
        unsigned int i;
        //d printf("Voy a sacar la carta de la posicion %d.\n",carta_numero);

        if ( carta_numero == 0 ) { //caso especial: sacar el primer elemento. no tiene anterior
            carta=mazo->primera;
            mazo->primera=carta->siguiente; }
        else { // busco la pos anterior al elemento a quitar
            t_carta *aux=mazo->primera;

            for ( i=0; i != carta_numero-1; i++ ) { aux=aux->siguiente; }

            carta=aux->siguiente; //hago la magia de listas
            aux->siguiente=carta->siguiente;
            carta->siguiente=NULL; }

        mazo->cantidad--; }
    printf("Saque la carta %d de %d del mazo.\n",(int) carta->valor, (int) carta->palo);
    return carta;
}

/*
 * Funciones relacionadas con los juegos de cartas.
 *
 */ 
char ordenarJuego(t_carta **arreglo, char dimension) {
    char vuelta, posicion, contador_comodines=0;
    for (vuelta=0; vuelta<dimension; vuelta++) { //la ultima pos va a quedar ordenada por el tipo de sort
        for (posicion=0; posicion<dimension-1; posicion++) {
            if ( arreglo[posicion]->valor > arreglo[posicion+1]->valor ) {
                intercambiar( arreglo[posicion], arreglo[posicion+1]); }
        }
    }
    
    for (posicion=0; posicion<dimension; posicion++) {
        if ( arreglo[posicion]->palo == 0 ) {
            contador_comodines++; }
    }
    return contador_comodines;
}

void intercambiar(t_carta *pos1, t_carta *pos2) {
    //verifico que no sean la misma posicion de memoria
    if (pos1 != pos2) {
        t_carta aux=*pos1;
        *pos1=*pos2;
        *pos2=aux; }
}

char verificaJuego(t_carta **arreglo,char dimension) {
    if ( dimension < 3) { return -1; } //un juego lo forman minimo 3 cartas

    char comodines=ordenarJuego(arreglo,dimension);
    if ( comodines > 1 ) { return -1; } //no puede tener dos comodines

    int i,verifica=1; //verifico que sean del mismo palo
    for (i=1; i<dimension; i++) { 
        if ( arreglo[0]->palo == arreglo[i]->palo || arreglo[i]->palo == 0 ) { }
        else { verifica=0; }
    }

    if ( verifica==1 ) { //son todos del mismo palo, verifico escalera
        int tope = (dimension-1) - comodines;
        for (i=0; i<tope; i++) {

            if ( arreglo[i]->valor == arreglo[i+1]->valor-1 ) { } //caso 1: consecutivo inmediato

            else if ( arreglo[i]->valor == ((arreglo[i+1]->valor)-2) && comodines==1 )
            { comodines--; }  //caso 2: salto, el lugar donde va el comodin

            else { verifica=0; } //caso 3: no verifica
        }
	} else { //no son del mismo palo, deberian ser del mismo valor o comodin
        verifica=1;
        for(i=1; i<dimension; i++) { 
            if ( arreglo[0]->valor == arreglo[i]->valor || arreglo[i]->palo==0) {  }
            else { verifica=0; }
        }
    }
    return verifica;
}

char verificaChinchon(t_carta **arreglo,char dimension) {
    if ( dimension != 7) { return 0; }
    if ( ordenarJuego(arreglo,dimension) != 0 ) { return 0; }
    return verificaJuego(arreglo,dimension);
}

/*
 * Funciones relacionadas con la comunicación por red.
 *
 */ 

int iniciarServidor() {
    int sockfd;     /* El file descriptor del socket para escuchar */
    struct sockaddr_in my_addr;     /* contendra la direccion IP y el numero de puerto local */
    int sin_size;   /* Contendra el tamanio de la escructura sockaddr_in */
    

    if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) == -1 ) { /* creamos el socket */
        perror("Error en la creación del socket.\n"); exit(1); }

    /* Asignamos valores a la estructura my_addr para luego poder llamar a la funcion bind() */ 
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(RED_PUERTO); /*debe convertirse a network byte order porque es enviado por la red*/
    my_addr.sin_addr.s_addr = INADDR_ANY;   /* automaticamente usa CUALQUIER direccion IP local */
    memset( &(my_addr.sin_zero), 8, 0);  /* rellena con ceros el resto de la estructura */
    
    if ( bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr) ) == -1) {
        perror("Error en el bind.\n"); exit(1); }

    /* Habilitamos el socket para recibir conexiones, con una cola de 5 conexiones en espera como maximo */
    if ( listen(sockfd, RED_BUFFER) == -1 ) { 
        perror("Error en el listen.\n"); exit(1); }

    return sockfd;
}

int iniciarCliente(char* ipServidor, char* nombreJ) {
    int sockfd;
    struct sockaddr_in direccionServidor;

    if ( ( sockfd=socket(AF_INET, SOCK_STREAM,0) ) == -1 ) {
        perror("Error creando el socket.\n"); exit(1); }

    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(RED_PUERTO);
    direccionServidor.sin_addr.s_addr = inet_addr(ipServidor);
    memset( &(direccionServidor.sin_zero), 8, 0); //cambie bzero por memset que es ansi c

    if ( connect(sockfd, (struct sockaddr*) &direccionServidor, sizeof(struct sockaddr) ) == -1 ) {
        perror("Error conectandose al servidor.\n"); exit(1); }

    if ( send(sockfd, nombreJ, sizeof(char)*8, 0) == -1 ) {
        perror("Error enviando nombre del jugador.\n"); close(sockfd); exit(1); }

    return sockfd;
}

t_jugador* recibirJugador(int socketServidor) {
    int sin_size = sizeof(struct sockaddr_in);
    struct sockaddr_in dirCliente;

    t_jugador* j = malloc(sizeof(t_jugador));
    if ( j != NULL ) {
            j->nombre=malloc(9);
            if ( ( j->socket=accept(socketServidor, (struct sockaddr*) &dirCliente, &sin_size) ) == -1 ) {
                    perror("Error aceptando la conexión de un cliente.\n");
                    exit(1); } //el accept es bloqueante            

            recv(j->socket,j->nombre, 9, 0); //el cliente envia el nombre del jugador al conectarse
            j->puntaje=0;
    }
    return j;
}

void enviarSenial(int fileDescriptor, char signal ) {
    if ( send(fileDescriptor,&signal,sizeof(char),0) != sizeof(char) ) {
        perror("Error enviando senial.\n");
        close(fileDescriptor); exit(1); }
}

char recibirSenial(int fileDescriptor) {
    char signal;
    if ( recv(fileDescriptor,&signal,sizeof(char),0) != sizeof(char) ) {
        perror("Error recibiendo senial.\n");
        close(fileDescriptor); exit(1); }
    return signal;
}

void enviarCarta( int socket, t_carta *carta ) {
    if ( send(socket,carta,sizeof(t_carta),0) != sizeof(t_carta) ) {
        perror("Error recibiendo la carta.\n");
        close(socket); exit(0); }
    free(carta);
    carta=NULL;
}
        
t_carta* recibirCarta(int socket) {
    t_carta *c=malloc(sizeof(t_carta));
    if (c != NULL) {
        if ( recv(socket,c,sizeof(t_carta),0) != sizeof(t_carta) ) {
            perror("Error recibiendo carta.\n");
            close(socket); exit(0); }
    }
    return c;
}

int recibirJuegos(int socket,t_mazo *mazo) {
    //el cliente envia el numero de cartas en el juego mediante una señal 
    //si el numero es negativo, son cartas sueltas que no forman ningun juego 
    //-20 es chinchon!
    char puntos=-10, cartas_recibidas=0, j;
    t_carta *cartaAux;
    char signalAux;
    char posible_chinchon=0;

    enviarSenial(socket, 2); //aviso al cliente que cortaron y espero los juegos.

    while ( cartas_recibidas < 7 ) {

        signalAux=recibirSenial(socket); //el cliente informa el nro de cartas que compone el juego a enviar
        printf("Voy a recibir %d cartas\n",signalAux);

        if (signalAux > 0) {

            if ( signalAux == 7 ) { posible_chinchon=1; puntos=-20; }

            for (j=0; j<signalAux; j++) { //son un juego, no suman puntos
                cartaAux=recibirCarta(socket);
                printf("Recibi el %d de %s.\n",cartaAux->valor,palos[cartaAux->palo]);
                
                //con comodin no es chinchon!
                if ( cartaAux->palo == 0 && posible_chinchon == 1 ) { posible_chinchon=0; puntos=-10; }

                agregarCarta(mazo,cartaAux);
                cartas_recibidas++; }
        } else if ( signalAux < 0 ) { //no forman un juego 
            if (puntos==-10) { puntos=0; } //ya no tenes -10

            for (j=signalAux; j<0; j++) {
                cartaAux=recibirCarta(socket);
                printf("Recibi el %d de %s.\n",cartaAux->valor,palos[cartaAux->palo]);
                puntos+=cartaAux->valor;
                agregarCarta(mazo,cartaAux);
                cartas_recibidas++; }                                                
        }
    }

    return puntos;
}

void repartirCartas(t_jugador *jugadores[], t_mazo *mazo) {
    char i,j;
    t_carta *cartaAux;

    for (i=0; i<J_MAXJUGADORES; i++) {
        //d printf("Repartiendo cartas a %s\n",jugadores[i]->nombre);
        enviarSenial(jugadores[i]->socket, 0);

        for (j=0; j<7; j++) {
            cartaAux=sacarCartaAzar(mazo);
            //d printf("Enviando %d de %d a %s",cartaAux->valor,cartaAux->palo,jugadores[i]->nombre);
            enviarCarta(jugadores[i]->socket, cartaAux); 
        }
    }
}

