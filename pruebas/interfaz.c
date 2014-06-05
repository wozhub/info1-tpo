
/* Trabajo Practico Obligatorio
 * Informática I - 2013 - Prof. Martín Marino
 * Integrantes: Ranú, Iannuzzi, Vinazza */ 

#include <ncurses.h>    /* todo el manejo de ventanas */
#include <malloc.h>     /* malloc */
#include <stdlib.h>     /* srand, rand */

#include "estructuras.h"  /* t_carta t_mazo */
#include "interfaz.h" 

void iniciarNcurses() {
    //inicializo ncurses
    initscr(); // inicio pantalla
    start_color(); /* con colores */
    curs_set(0); /* apago el cursor */
	cbreak(); /* sin line buffer */
    //timeout(-1); /* getch bloqueante */ 
	keypad(stdscr, TRUE);
    refresh();
}

void notificar(char *notificacion) {
    move(0, 0);          
    clrtoeol();
    mvprintw(0,0,notificacion);
}
    
void dibujarSeleccionada(WINDOW *ventana) {
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    wbkgd(ventana,COLOR_PAIR(3)); /* fondo de la carta */   
    wattron(ventana,COLOR_PAIR(1)); 
    box(ventana, 0 , 0); 
    wattroff(ventana,COLOR_PAIR(1)); 
    wrefresh(ventana);
}

void dibujarNoSeleccionada(WINDOW *ventana) {
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    wbkgd(ventana,COLOR_PAIR(3)); /* fondo de la carta */   
    wattron(ventana,COLOR_PAIR(2));
    box(ventana, 0 , 0); 
    wattroff(ventana,COLOR_PAIR(2)); 
    wrefresh(ventana);
}

void dibujarMano( WINDOW** ventanas_c, t_carta** cartas, unsigned char selec, unsigned char levan ) {
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    int i=0;
    for (;i<8;i++) {
        borrarVentana(ventanas_c[i]);
        
        if ( i==levan ) { ventanas_c[i]=nuevaVentana(12,14,25,(i+1)*16); } //La cartas que se dibuja levantadas
        else { ventanas_c[i]=nuevaVentana(12,14,30,(i+1)*16); }
        
        if ( i==selec ) { dibujarSeleccionada(ventanas_c[i]); } //La carta que se dibuja resaltada
        else { dibujarNoSeleccionada(ventanas_c[i]); }

        dibujarCarta(ventanas_c[i],cartas[i]);
    }
}

void dibujarManoSeleccion( WINDOW** ventanas_c, t_carta** cartas, unsigned char selec, char seleccion[7] ) {
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);


    int i;
    for (i=0;i<7;i++) {
        if ( seleccion[i] != -1 ) {
            borrarVentana(ventanas_c[i]);

            if ( seleccion[i] == 1 ) { ventanas_c[i]=nuevaVentana(12,14,25,(i+1)*16); } //La cartas que se dibuja levantadas
            else if ( seleccion[i] == 0 ) { ventanas_c[i]=nuevaVentana(12,14,30,(i+1)*16); }

            if ( i==selec ) { dibujarSeleccionada(ventanas_c[i]); } //La carta que se dibuja resaltada
            else { dibujarNoSeleccionada(ventanas_c[i]); }
                
            dibujarCarta(ventanas_c[i],cartas[i]);
        }
    }
}

void dibujarCartaTapada(WINDOW* ventana) {
    int row,col;
    getmaxyx(ventana,row,col);

    int x,y;
    for ( y=2; y+2<row;y++ ) { for ( x=2; x+2<col;x++ ) {
            mvwprintw(ventana,y,x,".");
    } }

	wrefresh(ventana);
}

void dibujarCartaBorrada(WINDOW* ventana) {
    int row,col;
    getmaxyx(ventana,row,col);

    int x,y;
    for ( y=1; y+1<row;y++ ) { for ( x=1; x+1<col;x++ ) {
            mvwprintw(ventana,y,x," ");
    } }
	wrefresh(ventana);
}
    
void dibujarCarta(WINDOW* ventana, t_carta *carta) {
    int row,col;
    getmaxyx(ventana,row,col);

    //dibujo el numero
    if ( carta->valor == 0 )
    { /* comodin */ }
    else if ( carta->valor < 9 ) { 
    /* 1 - 9 */
        mvwprintw(ventana,1,1,"%d",carta->valor);
        mvwprintw(ventana,row-2,1,"%d",carta->valor);
        mvwprintw(ventana,1,col-2,"%d",carta->valor);
        mvwprintw(ventana,row-2,col-2,"%d",carta->valor);
    } else {
    /* 10 - 12 */
        mvwprintw(ventana,1,1,"%d",carta->valor);
        mvwprintw(ventana,row-2,1,"%d",carta->valor);
        mvwprintw(ventana,1,col-3,"%d",carta->valor);
        mvwprintw(ventana,row-2,col-3,"%d",carta->valor);
    }

    //dibujo el palo
    if ( carta->palo == 1 ) { // espada
        mvwprintw(ventana,3,6, "/|");
        mvwprintw(ventana,4,6, "|]");
        mvwprintw(ventana,5,6, "|]");
        mvwprintw(ventana,6,6, "|]");
        mvwprintw(ventana,7,5,"====");
        mvwprintw(ventana,8,6, "||");
        mvwprintw(ventana,9,6, "''");
    } else if ( carta->palo == 2 ) { // oro
        mvwprintw(ventana,4,4," .-'-.");
        mvwprintw(ventana,5,4,"|     |");
        mvwprintw(ventana,6,4,"|     |");
        mvwprintw(ventana,7,4," '-.-'");     
    } else if ( carta->palo ==3 ) { // basto
        mvwprintw(ventana,3,8,    ".-=");
        mvwprintw(ventana,4,7,   "/  /");
        mvwprintw(ventana,5,6,  "y  /");
        mvwprintw(ventana,6,4,"./ //");
        mvwprintw(ventana,7,4,"/ /'");
        mvwprintw(ventana,8,4,"--");
    } else if ( carta->palo == 4 ) { // copa
        mvwprintw(ventana,3,6, "___");
        mvwprintw(ventana,4,5,"(   )");
        mvwprintw(ventana,5,6, "---");
        mvwprintw(ventana,6,6, "| |");
        mvwprintw(ventana,7,5,"'---'");
    } else { // comodin
        mvwprintw(ventana,3,3,  ".=   _=_");
        mvwprintw(ventana,4,2,"//; \\/ ._\\");
        mvwprintw(ventana,5,2,"' ;    ; '");
        mvwprintw(ventana,6,5,   "====");
        mvwprintw(ventana,8,3, "comodin!");
    }
 
	wrefresh(ventana);
}

WINDOW *nuevaVentana(int alto, int ancho, int y0, int x0) {
    /* Creo la ventana */
    WINDOW *w = newwin(alto, ancho, y0, x0);
    
    init_pair(2, COLOR_BLACK, COLOR_BLUE);

    /* Dibujo el marco */
    wattron(w,COLOR_PAIR(2));
	box(w, 0 , 0);
    wattroff(w,COLOR_PAIR(1)); 

	wrefresh(w);
	return w;
}

void borrarVentana(WINDOW *local_win)
{	
    if (local_win == NULL) { return; }
    int x,y,row,col;

    /* Borro los bordes de la ventana */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    
    /* Borro todos los caracteres dibujados dentro de la ventana */
    getmaxyx(local_win,row,col);
    for ( y=0; y<row ;y++ ) { for ( x=0; x<col;x++ ) {
            mvwprintw(local_win,y,x," "); }  }

	wrefresh(local_win);

    /* Borro la ventana */
	delwin(local_win);
    local_win=NULL;
}
