#include <malloc.h> 

#include "estructuras.h"

void main() {

    t_carta* m[4];
    char i;

    //juego de escalera
    m[0]=(t_carta*) nuevaCarta(1,3); m[1]=(t_carta*) nuevaCarta(1,5); m[2]=(t_carta*) nuevaCarta(1,4); m[3]= (t_carta*) nuevaCarta(1,6);

    printf("%d %d %d %d\n",m[0]->valor, m[1]->valor, m[2]->valor, m[3]->valor);
    i=ordenarJuego(m,4);
    printf("%d %d %d %d (%d comodines)\n",m[0]->valor, m[1]->valor, m[2]->valor, m[3]->valor,i);
    for (i=0;i<4; i++) { free(m[i]); }

    //juego de escalera
    m[0]=(t_carta*) nuevaCarta(1,3); m[1]=(t_carta*) nuevaCarta(1,5); m[2]=(t_carta*) nuevaCarta(0,13); m[3]= (t_carta*) nuevaCarta(1,6);

    printf("%d %d %d %d\n",m[0]->valor, m[1]->valor, m[2]->valor, m[3]->valor);
    i=ordenarJuego(m,4);
    printf("%d %d %d %d (%d comodines)\n",m[0]->valor, m[1]->valor, m[2]->valor, m[3]->valor,i);
    for (i=0;i<4; i++) { free(m[i]); }

    //juego de todos igual
    m[0]=(t_carta*) nuevaCarta(4,3); m[1]=(t_carta*) nuevaCarta(2,3); m[2]=(t_carta*) nuevaCarta(0,13); m[3]= (t_carta*) nuevaCarta(1,3);

    printf("%d %d %d %d\n",m[0]->valor, m[1]->valor, m[2]->valor, m[3]->valor);
    i=ordenarJuego(m,4);
    printf("%d %d %d %d (%d comodines)\n",m[0]->valor, m[1]->valor, m[2]->valor, m[3]->valor,i);
    for (i=0;i<4; i++) { free(m[i]); }

}
