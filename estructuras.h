
/* Trabajo Practico Obligatorio
 * Informática I - 2013 - Prof. Martín Marino
 * Integrantes: Ranú, Iannuzzi, Vinazza */ 

/* Representamos a las cartas con una estructura con dos campos char que representan
 * su valor y palo. Ademas tiene un puntero a carta para encadenarlas en un mazo. */
typedef struct t_carta {  
    char valor;
    char palo;
    struct t_carta *siguiente;
} t_carta;

/* Para ayudarnos a manejar la lista de cartas sin variables sueltas,  armamos una 
 * estructura mazo. Tener siempre la cantidad de cartas a mano nos va a simplificar
 * sacar una al azar eligiendo la posición de la carta. */
typedef struct t_mazo {
    struct t_carta *primera;
    struct t_carta *ultima;
    unsigned int cantidad;
} t_mazo;

/* A cada jugador de asignamos una estructura con nombre, puntaje y el socket 
 * */
typedef struct t_jugador {
    char* nombre;
    int puntaje;
    int socket;
} t_jugador;

//al final no lo usamos
//typedef enum t_palos { COMODIN, ESPADA, ORO, BASTO, COPA } t_palos;
