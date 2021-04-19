#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>


/* ******************************************************************
 *			PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
// Alumno: Belletti Gabriel Ignacio Padron: 100053
void pruevas_pila_vacia(){
	pila_t* pila = pila_crear();
	print_test("la pila 0 fue creada", pila != NULL);
	print_test("Crear pila no vacia", pila != NULL);
	print_test("Ver tope vacio", pila_ver_tope(pila) == NULL);
	print_test("Desapilar elemento en pila vacia", pila_desapilar(pila) == NULL);
	pila_destruir(pila);
	print_test("la pila fue destruida", true);
}
void pruevas_pila_con_elementos(int* a){
	pila_t* pila = pila_crear();
	print_test("la pila 1 fue creada", pila != NULL);
	print_test("Apilar elemento pila", pila_apilar(pila,a));
	print_test("Ver tope no vacio", pila_ver_tope(pila) == a);
	print_test("Desapilar elemento", pila_desapilar(pila) == a);
	print_test("Pila vacia", pila_esta_vacia(pila));
	print_test("Desapilar elemento en pila vacia", pila_desapilar(pila) == NULL);
	for(int i = 0;i < *a;i++){
		char* cad = malloc(sizeof(char));
		pila_apilar(pila,cad);
	}
	print_test("Pila no vacia", !pila_esta_vacia(pila));
	int elementos = 0;
	while (!pila_esta_vacia(pila)){
		char* a_borrar = pila_desapilar(pila);
		elementos += 1;
		free(a_borrar);
	}
	print_test("Pila vacia", pila_esta_vacia(pila));
	print_test("La cantidad de elementos desapilados coincide con los apilados", elementos == *a);
	pila_destruir(pila);
	print_test("la pila fue destruida", true);
}

void pruevas_pila_con_elemento_null(){
	pila_t* pila = pila_crear();
	print_test("la pila 2 fue creada", pila != NULL);
	print_test("Apilar elemento NULL", pila_apilar(pila,NULL));
	print_test("ver tope elemento NULL", pila_ver_tope(pila) == NULL);
	print_test("Desapilar elemento NULL", pila_desapilar(pila) == NULL);	
	pila_destruir(pila);
	print_test("la pila fue destruida", true);
}

void pruebas_pila_inexistente() {
	/* variables a utilizar*/
	pila_t* nulo = NULL;
	/* Inicio de pruebas */
	print_test("Puntero inicializado a NULL", nulo == NULL);
}

void pruebas_pila_alumno() {
	/* variables a utilizar*/
	int a = 1000;
	/* Inicio de pruebas */
	pruebas_pila_inexistente();
	pruevas_pila_vacia();
	pruevas_pila_con_elementos(&a);
	/* Pruevas con pila con elementos NULL */
	pruevas_pila_con_elemento_null();
}
