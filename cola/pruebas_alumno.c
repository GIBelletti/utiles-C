#include "cola.h"
#include "testing.h"
#include <stddef.h>


/* ******************************************************************
 *			PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
// Alumno: Belletti Gabriel Ignacio Padron: 100053

void wrapper(void* dato){
	cola_destruir(dato,NULL);
}

void pruevas_cola_vacia_y_un_elemento(cola_t* cola,int* a){
	print_test("cola inicialmente vacia", cola_esta_vacia(cola));
	print_test("encolar elemento en cola vacia", cola_encolar(cola, a));
	print_test("no esta vacia", !cola_esta_vacia(cola));
	print_test("ver primero", cola_ver_primero(cola) == a);
	print_test("desencolar elemento", cola_desencolar(cola) == a);
	print_test("esta vacia", cola_esta_vacia(cola));
	print_test("desencolar elemento en cola vacia", cola_desencolar(cola) == NULL);
}

void pruevas_con_cola_con_elementos_que_usan_el_heap(cola_t* cola0,cola_t* cola1){
	print_test("encolar elemento que usa heap", cola_encolar(cola0, cola1));
	print_test("desencolar elemento que usa heap", cola_desencolar(cola0) == cola1);
	print_test("encolar elemento que usa heap", cola_encolar(cola1, cola0));
}

void Pruevas_con_cola_con_elementos_NULL(cola_t* cola){
	print_test("encolar elemento NULL en cola", cola_encolar(cola, NULL));
	print_test("no esta vacia", !cola_esta_vacia(cola));
	print_test("ver primero NULL", cola_ver_primero(cola) == NULL);
	print_test("desencolar elemento", cola_desencolar(cola) == NULL);
	print_test("esta vacia", cola_esta_vacia(cola));
}

void prueva_de_orden_de_encolamiento_y_desencolamiento(cola_t* cola,int* a){
	print_test("encolar elemento A", cola_encolar(cola, &a));
	print_test("ver elemento A en cola", cola_ver_primero(cola) == &a);
	print_test("encolar elemento NULL", cola_encolar(cola, NULL));
	print_test("ver elemento A en cola", cola_ver_primero(cola) == &a);
	print_test("desencolar elemento A", cola_desencolar(cola) == &a);
	print_test("desencolar elemento NULL", cola_desencolar(cola) == NULL);
}

void pruebas_cola_alumno() {
	/* variables a utilizar */
	cola_t* cola0 = cola_crear();
	cola_t* cola1 = cola_crear();
	int a = 15;
	/* Inicio de pruebas */
	print_test("cola existe", cola0 != NULL);
	print_test("cola existe", cola1 != NULL);
	pruevas_cola_vacia_y_un_elemento(cola0,&a);
	/* pruevas con cola con elementos que usan el heap */
	pruevas_con_cola_con_elementos_que_usan_el_heap(cola0, cola1);
	/* Pruevas con cola con elementos NULL */
	Pruevas_con_cola_con_elementos_NULL(cola0);
	/* prueva de oreden de encolamiento y desencolamiento */
	prueva_de_orden_de_encolamiento_y_desencolamiento(cola0,&a);
	/* Prueva de destruccion de pilas */
	/* devido a que la cola1 tiene la cola0 esta tambien sera destruida por ser dato de esta */
	cola_destruir(cola1,(*wrapper));
	print_test("la cola con sus elementos los cuales usan el heap fue destruida", true);
}
