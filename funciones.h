
/* Trabajo Practico Obligatorio
 * Informática I - 2013 - Prof. Martín Marino
 * Integrantes: Ranú, Iannuzzi, Vinazza */ 

/* mazo y cartas */
t_mazo* nuevoMazo(); // el pozo es un mazo vacio
void llenarMazo(t_mazo *mazo); //carga todas las cartas
t_carta* nuevaCarta(char palo, char valor); /* Devuelve un puntero a una carta nueva, con el palo y valor deseados */
void agregarCarta(t_mazo* lista, t_carta* elemento); // Agrega una carta al mazo
t_carta* sacarCartaAzar(t_mazo *mazo);

/* juegos */
char ordenarJuego(t_carta **arreglo, char dimension); // ordena un arreglo de cartas por valor
void intercambiar(t_carta *pos1, t_carta *pos2); //intercambia dos punteros a carta
char verificaJuego(t_carta **arreglo,char dimension); //verifica que un arreglo de *cartas sea un juego 
char verificaChinchon(t_carta **arreglo,char dimension); //verifica que un arreglo de *cartas sea chinchon

/* red */
int iniciarServidor(); //inicia un socket, hace el bind y lo pone en listen
int iniciarCliente(char* ipServidor, char* nombreJ);
t_jugador* recibirJugador(int socket);

void enviarSenial( int fileDescriptor, char signal); //envia un char por el socket 
char recibirSenial(int fileDescriptor); //recibe un char por el socket
void enviarCarta( int socket, t_carta *carta); //envia una carta por el socket
t_carta* recibirCarta(int socket); //recibe una carta por el socket
int recibirJuegos(int socket,t_mazo *mazo);
void repartirCartas(t_jugador *jugadores[], t_mazo *mazo);
