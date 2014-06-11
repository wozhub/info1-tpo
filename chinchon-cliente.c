/* Trabajo Practico Obligatorio
 * Informática I - 2013 - Prof. Martín Marino
 * Integrantes: Ranú, Iannuzzi, Vinazza */

#include <stdio.h> /* sprintf */
#include <ncurses.h> /* getch */ 
#include <stdlib.h> /* atoi */
#include <string.h> /* memset */

#include "estructuras.h"
#include "funciones.h"
#include "interfaz.h"

static char* palos[] = { "Comodín","Espada","Oro","Basto","Copa" };

/* funcion para debuguear en paz
void _pausa() {  while( getch() != KEY_DOWN ) {  } } */ 

char elegirCierre() {
    char i=0; int entrada;
    WINDOW *v= (WINDOW *) nuevaVentana(5,15,12,70);

    mvprintw(14,72,"Continuar.");
    while( ( entrada=getch() ) != '\n') {
        switch(entrada) {
            case KEY_UP: if (i>0) { i--; } break; 
            case KEY_DOWN: if (i<1) { i++; } break; }
        if (i==0) { mvprintw(14,72,"Continuar."); }
        else { mvprintw(14,72,"Cortar!   "); }
    }
    mvprintw(14,72,"         ");
    borrarVentana(v);
    return i;
}

int main( int argc, char* argv[] ) {

    //verifico los argumentos ingresados por linea de comando
    if ( argc != 3 ) { 
        printf("Requiere IP servidor y nombre del jugador para ejecutarse.\n");
        printf("Ej: %s 192.168.1.100 Chino\n\n",argv[0]);
        return 1; }

    //variables para el juego
    t_carta *cartaAux, *manoJugador[8]; //la octava carta es la que el jugador levanta
    char signalAux,pozoPalo,pozoValor; //auxiliares para comunicacion con el servidor
    char i,j,k; //auxiliares para iteracion
    char seleccionada, cartas_seleccionadas, cartas_enviadas; //contadores para seleccion
    char seleccion[7]; //flags 0 no sel, 1 sel, -1 ya descartado
    t_carta **juegosAux1, **juegosAux2;
    char jugando=1;


    iniciarNcurses();
    // variables para ncurses
    int maxX,maxY,entrada;
    WINDOW *ventanas_c[8], *mazo, *pozo; //las ventanas donde dibujamos las cartas (cajitas)
    getmaxyx(stdscr, maxX, maxY);
    char notificacion[maxX]; //arreglo donde escribir los mensajes con formato

    sprintf(notificacion,"Conectandose a %s como %s.\n",argv[1],argv[2]);
    notificar(notificacion);

    int socket=iniciarCliente(argv[1],argv[2]); //ip, nombre jugador

    pozo= (WINDOW *) nuevaVentana(12,14,10,36);
    mazo= (WINDOW *) nuevaVentana(12,14,10,20);
    dibujarCartaTapada(mazo);

    while ( jugando ) {
        sprintf(notificacion,"Esperando Inicio del turno...\n");
        notificar(notificacion);

        signalAux=recibirSenial(socket);

        /* Seria mas prolijo encerrar cada caso en una funcion, tipo
         *  0: recibirCartas()
         *  1: turnoNormal()
         *  2: turnoCierre() 
         */
        switch (signalAux) {
            case 0:
                sprintf(notificacion,"0: Voy a recibir las 7 cartas iniciales.\n");
                notificar(notificacion);

                for (i=0;i<7;i++) {
                    manoJugador[i]=recibirCarta(socket);
                    ventanas_c[i] = (WINDOW * ) nuevaVentana(12, 15, 30, (i+1)*16);
                    dibujarCarta(ventanas_c[i],manoJugador[i]); }
                break;
            case 1:
                sprintf(notificacion,"1: Me toca a mi. Estas son mis cartas:\n");
                notificar(notificacion);

                //recibo la carta que esta en el pozo mediante señales 
                pozoPalo=recibirSenial(socket); pozoValor=recibirSenial(socket);
                cartaAux=nuevaCarta(pozoPalo,pozoValor);
                dibujarCartaBorrada(pozo);
                dibujarCarta(pozo,cartaAux); free(cartaAux); //la libero porque voy a recibirla bien si la elijo
                   
                //el jugador elige de donde tomar a carta 
                sprintf(notificacion,"Elija tomar del pozo o del mazo.\n");
                notificar(notificacion);

                i=0;
                dibujarNoSeleccionada(pozo); dibujarSeleccionada(mazo);
                while( ( entrada=getch() ) != '\n') {
                    switch(entrada) {    
                        case KEY_LEFT: if ( i>0 ) {
                                           dibujarNoSeleccionada(pozo); dibujarSeleccionada(mazo);
                                           i--; } break;
                        case KEY_RIGHT: if ( i<1 ) {
                                            dibujarNoSeleccionada(mazo); dibujarSeleccionada(pozo);
                                            i++; } break; } }

                //envio la opcion y recibo la carta
                enviarSenial(socket,i);
                cartaAux=recibirCarta(socket);
                sprintf(notificacion,"Recibí el %d de %s\n",cartaAux->valor,palos[cartaAux->palo]);
                notificar(notificacion);
                ventanas_c[7] = (WINDOW * ) nuevaVentana(12, 15, 30, 128);
                manoJugador[7] = cartaAux;
                dibujarCarta(ventanas_c[7],manoJugador[7]);
                
                //el jugador elige que carta descartar
                sprintf(notificacion,"Puede intercambiar las cartas de lugar. Al terminar la ultima carta sera descartada.\n");
                notificar(notificacion);

                i=0; seleccionada=-1;
                dibujarMano(ventanas_c,manoJugador,i,seleccionada);
                while( ( entrada=getch() ) != '\n') {
                    switch(entrada) {    
                        case KEY_LEFT:  if (i>0) { i--; } break;
                        case KEY_RIGHT: if (i<7) { i++; } break;
                        case KEY_UP: if (seleccionada == -1) { seleccionada=i; }
                                     else if ( seleccionada != i ) { //ya hay OTRA seleccionda, las intercambio
                                        cartaAux=manoJugador[i];
                                        manoJugador[i]=manoJugador[seleccionada];
                                        manoJugador[seleccionada]=cartaAux;
                                        seleccionada=-1;
                                        } break; }
                    dibujarMano(ventanas_c,manoJugador,i,seleccionada); 
                }
                
                //borro la carta 8 de la mano y la dibujo en el pozo
                borrarVentana(ventanas_c[7]); dibujarCartaBorrada(pozo); dibujarCarta(pozo,manoJugador[7]);
                //pongo todas las cartas "azules" para que se note que no es el turno del jugador
                if ( i != 7 ) { dibujarNoSeleccionada(ventanas_c[i]); } 
                dibujarNoSeleccionada(mazo); dibujarNoSeleccionada(pozo);


                //el jugador elige si cortar o no 
                sprintf(notificacion,"Elija continuar o cortar.\n");
                notificar(notificacion);
                
                signalAux=elegirCierre();

                enviarSenial(socket,signalAux);
                enviarCarta(socket,manoJugador[7]);

                break;
            case 2:
                sprintf(notificacion,"Cortaron! Debe enviar los juegos al servidor.\n");
                notificar(notificacion);

                char seleccion[7]={0,0,0,0,0,0,0};
                cartas_enviadas=0;
                while (cartas_enviadas != 7) {
                    /* Debug Seleccion
                    for (i=0;i<7;i++) { printw("%d\t",seleccion[i]); } _pausa(); */

                    /* Debug Ventanas Existentes
                    for (j=0;j<7;j++) {
                        if (ventanas_c[j] == NULL) { printw("ventanas_c[%d] es nulo.\n",j); }
                        else { printw("ventanas_c[%d] no es nulo.\n",j); }
                    } _pausa(); */

                    i=0;
                    dibujarManoSeleccion(ventanas_c,manoJugador,i,seleccion);
                    while( ( entrada=getch() ) != '\n' ) { //ojo con la portabilidad
                        switch(entrada) {
                            case KEY_LEFT:  if (i>0) { i--; } break;
                            case KEY_RIGHT: if (i<6-cartas_enviadas) { i++; } break;
                            case KEY_UP:    seleccion[i]=1; break; 
                            case KEY_DOWN:  seleccion[i]=0; break; } //cierra el switch
                        dibujarManoSeleccion(ventanas_c,manoJugador,i,seleccion); 
                    } //cierra el while
                    
                    /* Cuento las cartas elegidas */
                    cartas_seleccionadas=0;
                    for (i=0;i<7;i++) { if (seleccion[i]==1) { cartas_seleccionadas++; } }
                        
                    if (cartas_seleccionadas != 0) {
                        juegosAux1=malloc( cartas_seleccionadas * sizeof(t_carta*) ); //vector de cartas a enviar
                        juegosAux2=malloc( (7-cartas_enviadas-cartas_seleccionadas) * sizeof(t_carta*) ); //vector de cartas a conservar

                        /* Distribuyo las cartas en los arreglos a enviar y a conservar */ 
                        j=0;k=0;
                        for (i=0;i<7;i++) {
                            if ( seleccion[i] == 1 ) {
                                //d printw("Agregando %d de %s al arreglo de cartas a enviar.\n",manoJugador[i]->valor,palos[manoJugador[i]->palo]);
                                juegosAux1[j]=manoJugador[i]; j++; //j es el numero de cartas a enviar
                            }
                            else if ( seleccion[i] == 0 ) {
                                //d printw("Agregando %d de %s al arreglo de cartas a conservar.\n",manoJugador[i]->valor,palos[manoJugador[i]->palo]);
                                juegosAux2[k]=manoJugador[i]; k++; //k es el numero de cartas restantes
                            }
                        }

                        /* Envio el juego seleccionado */ 
                        /* El servidor espera un numero positivo si las cartas verifican un juego 
                         * o un numero negativo si son descarte */
                        if ( verificaJuego(juegosAux1,cartas_seleccionadas) == 1 ) { enviarSenial(socket,cartas_seleccionadas); }
                        else { enviarSenial(socket,-cartas_seleccionadas); }
                        
                        /* Envio las cartas */
                        for (i=0;i<j;i++) { enviarCarta(socket,juegosAux1[i]); }

                        /* Rearmo manoJugador con las cartas resantes */ 
                        for (i=0; i<k; i++) {
                            //d printw("Agregando %d de %s a manoJugador[%d].\n",juegosAux2[i]->valor,palos[juegosAux2[i]->palo],i);
                            manoJugador[i]=juegosAux2[i]; seleccion[i]=0; }

                        for (i=k; i<k+j; i++) { /* recorro solo las cartas que no envie antes */
                            //d printw("Seteando NULL manoJugador[%d].\n",i);
                            manoJugador[i]=NULL; seleccion[i]=-1; borrarVentana(ventanas_c[i]);
                        }

                        //d printw("k=%d j=%d enviadas=%d",k,j,cartas_enviadas); _pausa();
                        
                        cartas_enviadas+=j;

                        /* Debug Auxiliares 
                        printw("Libero los auxiliares.\n"); _pausa(); */
                        free(juegosAux1); free(juegosAux2);
                    }

                }

                break;
            case 3: //termino el juego;
                signalAux=1;
                enviarSenial(socket,signalAux);
                close(socket);
                jugando=0;
            }
    }
    
    endwin(); //cierro ncurses   
    return 0; //salgo sin errores
}
