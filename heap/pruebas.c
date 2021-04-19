#include "testing.h"
#include <stdlib.h>
#include <stdio.h>

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_heap_alumno(void);

int main(int argc, char *argv[])
{
    if (argc > 1) {
        // Asumimos que nos están pidiendo pruebas de volumen.
        return 0;
    }

    printf("~~~ PRUEBAS ~~~\n");
    pruebas_heap_alumno();

    return failure_count() > 0;
}
