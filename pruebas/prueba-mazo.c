#include <malloc.h> 


#include "estructuras.h"

void main() {
    /* el cast es por el warning: initialization makes pointer from integer without a cast [enabled by default] */
    t_mazo* m = (t_mazo *) nuevoMazo();
    llenarMazo(m);

    t_carta* c;

    while ( m->cantidad > 0 ) {
        c = (t_carta *) sacarCartaAzar(m);
        printf("Quedan %d cartas...\n\n",m->cantidad);
    }
}
