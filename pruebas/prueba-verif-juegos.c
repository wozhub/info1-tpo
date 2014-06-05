#include <stdio.h>

#include "estructuras.h"
#include "funciones.h"

void main() {

    t_carta* m[7];
    char i;

    //juego de escalera
    m[0]=(t_carta*) nuevaCarta(1,1); m[1]=(t_carta*) nuevaCarta(1,3); m[2]=nuevaCarta(1,6); m[3]=nuevaCarta(1,5);
    m[4]=nuevaCarta(1,4); m[5]=nuevaCarta(1,2); m[6]=nuevaCarta(1,7);

    printf("Juego: %d %d %d %d %d %d %d\n",m[0]->valor, m[1]->valor, m[2]->valor, m[3]->valor, m[4]->valor, m[5]->valor, m[6]->valor);

    printf("%d\n",verificaChinchon(m,7));

    printf("Juego: %d %d %d %d %d %d %d\n",m[0]->valor, m[1]->valor, m[2]->valor, m[3]->valor, m[4]->valor, m[5]->valor, m[6]->valor);
}
