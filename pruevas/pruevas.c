#include "testing.h"
#include "strutil.h"
#include <stddef.h>
#include <stdio.h>

/* ******************************************************************
 *                        PRUEBAS UNITARIAS 
 * *****************************************************************/

/* Prueba de la funcion seleccion. */
void alpha() {
	const size_t n = 3;
	const char espacio = ' ';
	char* cadena1 = "Hola mundo wii";
	char** cadenas1 = split(cadena1,espacio);
	for (size_t i = 0; i < n; i++){
		printf("%s",cadenas1[i]);
	}
	print_test("Prueba split", cadenas1[n] == NULL);
	free_strv(cadenas1);
}

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

/* Programa principal. */
int main(void) {
	/* Ejecutar todas las pruebas unitarias. */
	alpha();
	return failure_count() > 0;
}
