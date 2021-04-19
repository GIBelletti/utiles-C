#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>


/* ******************************************************************
 *			PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
// Alumno: Belletti Gabriel Ignacio Padron: 100053

void wrapper(void* dato){
	cola_destruir(dato,NULL);
	return;
}

void wrapper_char(void* dato){
	free(dato);
	return;
}

void pruevas_cola_vacia_y_un_elemento(int* a){
	cola_t* cola = cola_crear();
	print_test("cola existe", cola != NULL);
	print_test("cola inicialmente vacia", cola_esta_vacia(cola));
	print_test("encolar elemento en cola vacia", cola_encolar(cola, a));
	print_test("no esta vacia", !cola_esta_vacia(cola));
	print_test("ver primero", cola_ver_primero(cola) == a);
	print_test("desencolar elemento", cola_desencolar(cola) == a);
	print_test("esta vacia", cola_esta_vacia(cola));
	print_test("desencolar elemento en cola vacia", cola_desencolar(cola) == NULL);
	cola_destruir(cola,NULL);
}

void pruevas_con_cola_con_elementos_que_usan_el_heap(cola_t* cola0,cola_t* cola1){
	print_test("encolar elemento que usa heap", cola_encolar(cola0, cola1));
	print_test("desencolar elemento que usa heap", cola_desencolar(cola0) == cola1);
	print_test("encolar elemento que usa heap", cola_encolar(cola1, cola0));
}

void Pruevas_con_cola_con_elementos_NULL(){
	cola_t* cola = cola_crear();
	print_test("cola existe", cola != NULL);
	print_test("encolar elemento NULL en cola", cola_encolar(cola, NULL));
	print_test("no esta vacia", !cola_esta_vacia(cola));
	print_test("ver primero NULL", cola_ver_primero(cola) == NULL);
	print_test("desencolar elemento", cola_desencolar(cola) == NULL);
	print_test("esta vacia", cola_esta_vacia(cola));
	cola_destruir(cola,NULL);
}

void prueva_de_orden_de_encolamiento_y_desencolamiento(int* a){
	cola_t* cola = cola_crear();
	print_test("cola existe", cola != NULL);
	print_test("encolar elemento A", cola_encolar(cola, &a));
	print_test("ver elemento A en cola", cola_ver_primero(cola) == &a);
	print_test("encolar elemento NULL", cola_encolar(cola, NULL));
	print_test("ver elemento A en cola", cola_ver_primero(cola) == &a);
	print_test("desencolar elemento A", cola_desencolar(cola) == &a);
	print_test("desencolar elemento NULL", cola_desencolar(cola) == NULL);
	for(int i = 0;i < *a;i++){
		char* cad = malloc(sizeof(char));
		cola_encolar(cola, cad);
	}
	print_test("cola no vacia", !cola_esta_vacia(cola));
	int elementos = 0;
	while (!cola_esta_vacia(cola)){
		char* a_borrar = cola_desencolar(cola);
		elementos += 1;
		free(a_borrar);
	}
	print_test("La cantidad de elementos desencolados coincide con los encolados", elementos == *a);
	cola_destruir(cola,NULL);
}

void prueva_de_destrucion_de_colas(){
	cola_t* cola_sin_elemetos = cola_crear();
	print_test("cola existe", cola_sin_elemetos != NULL);
	cola_t* cola_con_elemetos = cola_crear();
	print_test("cola existe", cola_con_elemetos != NULL);
	cola_t* cola_sin_elemetos_wrapper = cola_crear();
	print_test("cola existe", cola_sin_elemetos_wrapper != NULL);
	cola_t* cola_con_elemetos_wrapper = cola_crear();
	print_test("cola existe", cola_con_elemetos_wrapper != NULL);
	char* pru0 = "";
	cola_destruir(cola_sin_elemetos,NULL);
	print_test("la cola sin elementos fue destruida", true);
	print_test("encolar elemento pru0", cola_encolar(cola_con_elemetos, pru0));
	cola_destruir(cola_con_elemetos,NULL);
	print_test("la cola con elementos fue destruida", true);
	int* pru1 = malloc(sizeof(int));
	pru1[0] = '\0';
	print_test("creacion de elemento", pru1 != NULL);
	cola_destruir(cola_sin_elemetos_wrapper,wrapper_char);
	print_test("la cola sin elementos fue destruida con wrapper", true);
	print_test("encolar elemento que usa memoria", cola_encolar(cola_con_elemetos_wrapper, pru1));
	cola_destruir(cola_con_elemetos_wrapper,wrapper_char);
	print_test("la cola sin elementos fue destruida con wrapper", true);
}

void pruebas_cola_alumno() {
	/* variables a utilizar */
	cola_t* cola0 = cola_crear();
	cola_t* cola1 = cola_crear();
	int a = 1000;
	/* Inicio de pruebas */
	print_test("cola existe", cola0 != NULL);
	print_test("cola existe", cola1 != NULL);
	pruevas_cola_vacia_y_un_elemento(&a);
	/* Pruevas con cola con elementos NULL */
	Pruevas_con_cola_con_elementos_NULL();
	/* prueva de oreden de encolamiento y desencolamiento */
	prueva_de_orden_de_encolamiento_y_desencolamiento(&a);
	/* Prueva de destruccion de pilas */
	/* pruevas con cola con elementos que usan el heap */
	pruevas_con_cola_con_elementos_que_usan_el_heap(cola0, cola1);
	/* devido a que la cola1 tiene la cola0 esta tambien sera destruida por ser dato de esta */
	cola_destruir(cola1,(*wrapper));
	print_test("la cola con sus elementos los cuales usan el heap fue destruida", true);
	prueva_de_destrucion_de_colas();
}
