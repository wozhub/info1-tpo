
/* Trabajo Practico Obligatorio
 * Informática I - 2013 - Prof. Martín Marino
 * Integrantes: Ranú, Iannuzzi, Vinazza */ 


void iniciarNcurses(); /* inicia el modo de terminal adecuado para ncurses */
void notificar(char *notificacion); /* imprime un mensaje en la primera linea de la pantalla */
void dibujarSeleccionada(WINDOW *ventana);
void dibujarNoSeleccionada(WINDOW *ventana);
void dibujarMano( WINDOW** ventanas_c, t_carta** cartas, unsigned char selec, unsigned char levan );
void dibujarManoSeleccion( WINDOW** ventanas_c, t_carta** cartas, unsigned char selec, char seleccion[7] );
void dibujarCarta(WINDOW* ventana, t_carta *carta);
void dibujarCartaTapada(WINDOW* ventana);
WINDOW *nuevaVentana(int alto, int ancho, int y0, int x0);
void borrarVentana(WINDOW *local_win);
