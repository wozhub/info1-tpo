#include <ncurses.h>    /* todo el manejo de ventanas */
#include <malloc.h>     /* malloc */
#include <stdlib.h>     /* srand, rand */

#include "estructuras.h"  /* t_carta */

/* Para probar la interfaz gráfica armamos un arreglo de cartas cualquiera. */

int main(int argc, char *argv[]) {
	int entrada,seleccionada;
    
    srand (time(NULL));

    iniciarNcurses();

    WINDOW *ventanas_c[7]; //las ventanas donde dibujamos las cartas (cajitas)
    t_carta *manoJugador[8],*cartaAux;

    unsigned char i=0;
    for (;i<8;i++) {
            manoJugador[i]=malloc(sizeof(t_carta));
            manoJugador[i]->palo=rand() %5;
            if ( manoJugador[i]->palo > 0 ) {
                manoJugador[i]->valor=( rand() % 12 ) +1;
            } else { manoJugador[i]->valor=25; }

            /* warning: assignment makes pointer from integer without a cast [enabled by default] */
            ventanas_c[i] = (WINDOW * ) nuevaVentana(12, 15, 30, (i+1)*16);
            dibujarCarta(ventanas_c[i],manoJugador[i]);
    }

    WINDOW *mazo,*pozo;
    mazo= (WINDOW *) nuevaVentana(12,14,10,20);
    dibujarCartaTapada(mazo);
    pozo= (WINDOW *) nuevaVentana(12,14,10,36);

    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    
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
                    } break; 
        }
        dibujarMano(ventanas_c,manoJugador,i,seleccionada); 
    }

	endwin();			/* End curses mode		  */
	return 0;
}
