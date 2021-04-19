#include "lista.h"
#include "testing.h"
#include <stddef.h>


/* ******************************************************************
 *			PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
// Alumno: Belletti Gabriel Ignacio Padron: 100053
// FUNCIONES AUXILIARES
void wrapper(void* dato){
	lista_destruir(dato,NULL);
}

bool copiar_lista(void* dato, void* extra){
	lista_insertar_primero(extra, dato);
	return true;
}

void enviar_al_final(lista_iter_t* iter){
	while (lista_iter_avanzar(iter)){
	}
	return;
}

bool eliminar_elementos_del_iterador(lista_iter_t* iter){
	while (!lista_iter_al_final(iter)){
		lista_iter_borrar(iter);
	}
	return true;
}

// PRUEBAS
void pruevas_de_listas_con_lista_vacia(){
	lista_t* lista = lista_crear();
	print_test("lista vacia", lista_esta_vacia(lista));
	print_test("primer elemento de lista vacia NULL", lista_ver_primero(lista) == NULL);
	print_test("ultimo elemento de lista vacia NULL", lista_ver_ultimo(lista) == NULL);
	print_test("borrar primero en lista vacia", lista_borrar_primero(lista) == NULL);
	print_test("largo de lista vacia", lista_largo(lista) == 0);
	lista_destruir(lista,NULL);
}

void pruevas_de_listas_con_un_elemento(int* a,int* b){
	lista_t* lista = lista_crear();
	print_test("insertar primero lista vacia", lista_insertar_primero(lista, a));
	print_test("lista no vacia", !lista_esta_vacia(lista));
	print_test("largo de lista no vacia", lista_largo(lista) == 1);
	print_test("primer elemento de lista", lista_ver_primero(lista) == a);
	print_test("ultimo elemento de lista", lista_ver_ultimo(lista) == a);
	print_test("vaciar lista", lista_borrar_primero(lista) == a);
	print_test("lista vacia", lista_esta_vacia(lista));
	print_test("insertar ultimo lista vacia", lista_insertar_ultimo(lista, b));
	print_test("lista no vacia", !lista_esta_vacia(lista));
	print_test("largo de lista no vacia", lista_largo(lista) == 1);
	print_test("primer elemento de lista", lista_ver_primero(lista) == b);
	print_test("ultimo elemento de lista", lista_ver_ultimo(lista) == b);
	print_test("vaciar lista", lista_borrar_primero(lista) == b);
	print_test("lista vacia", lista_esta_vacia(lista));
	print_test("largo de lista vacia", lista_largo(lista) == 0);
	lista_destruir(lista,NULL);
}

void prueva_de_destruir_lista_con_elemento_que_usa_heap(){
	lista_t* lista0 = lista_crear();
	lista_t* lista1 = lista_crear();
	print_test("insertar lista en lista", lista_insertar_primero(lista1, lista0));
	lista_destruir(lista1,*wrapper);
	print_test("lista con lista insertada destruida", true);
}

void prueva_de_iterador_externo(lista_t* lista,int* a,int* b){
	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("iterador de lista 2 existe", iter != NULL);
	print_test("lectura de posicion actual", lista_iter_ver_actual(iter) == a);
	for(size_t k = 0;k < *a;k++){
		lista_iter_avanzar(iter);
	}
	print_test("avanzar", lista_iter_avanzar(iter));
	print_test("lectura de posicion actual", lista_iter_ver_actual(iter) == b);
	print_test("no final", !lista_iter_al_final(iter));
	/* pruevas del tamaño de la lista */
	size_t antes_de_insercion = lista_largo(lista);
	print_test("insertar", lista_iter_insertar(iter, a));
	print_test("lectura de posicion actual", lista_iter_ver_actual(iter) == a);
	print_test("largo de lista aumenta", lista_largo(lista) == antes_de_insercion + 1);
	print_test("eliminar", lista_iter_borrar(iter) == a);
	print_test("lectura de posicion actual", lista_iter_ver_actual(iter) == b);
	print_test("largo de lista disminuye", lista_largo(lista) == antes_de_insercion);
	lista_iter_destruir(iter);
	print_test("iterador destruido", true);
}

void prueva_de_insercion_en_primera_y_ultima_posicion(lista_t* lista,int* a,int* b){
	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("iterador de lista 3 existe", iter != NULL);
	print_test("insertar en primera posicion", lista_iter_insertar(iter, b));
	print_test("se verifica posicion la insercion en primera posicion en iterador", lista_iter_ver_actual(iter) == b);
	print_test("se verifica posicion la insercion en primera posicion en lista de iterador", lista_ver_primero(lista) == b);
	enviar_al_final(iter);
	print_test("iterador al final", lista_iter_al_final(iter));
	print_test("insertar en ultima posicion", lista_iter_insertar(iter, a));
	print_test("se verifica posicion la insercion en ultima posicion en iterador", lista_iter_ver_actual(iter) == a);
	print_test("se verifica posicion la insercion en ultima posicion en lista de iterador", lista_ver_ultimo(lista) == a);
	/* ver en ultima posicion */
	print_test("iterador no al final", !lista_iter_al_final(iter));
	enviar_al_final(iter);
	print_test("iterador al final", lista_iter_al_final(iter));
	print_test("en el final NULL", lista_iter_ver_actual(iter) == NULL);
	lista_iter_destruir(iter);
	print_test("iterador destruido", true);
}

void prueva_de_borrado_en_primera_y_ultima_posicion(lista_t* lista,int* a,int* b){
	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("iterador de lista 3 existe", iter != NULL);
	print_test("eliminar todos los elementos desde el primero", eliminar_elementos_del_iterador(iter));
	print_test("lista vacia", lista_esta_vacia(lista));
	print_test("primer elemento NULL", lista_ver_primero(lista) == NULL);
	print_test("ultimo elemento NULL", lista_ver_ultimo(lista) == NULL);
	lista_iter_destruir(iter);
	print_test("iterador destruido", true);
}

void pruevas_del_iterador_interno_y_externo(int* a,int* b){
	lista_t* lista2 = lista_crear();
	lista_t* lista3 = lista_crear();
	void* c = lista3;
	/* pruevas de primitivas del iterador de lista */
	for(size_t i = 0;i < *a;i++){
		lista_insertar_primero(lista2, a);
	}
	for(size_t j = 0;j < *b;j++){
		lista_insertar_ultimo(lista2, b);
	}
	/* pruevas de primitivas del iterador interno de lista */
	lista_iterar(lista2, NULL, c);
	print_test("iteracion nula", lista_esta_vacia(lista3));
	lista_iterar(lista2, *copiar_lista, c);
	print_test("iteracion no nula", !lista_esta_vacia(lista3));
	/* pruevas de primitivas del iterador externo de lista */
	prueva_de_iterador_externo(lista2,a,b);
	/* pruevas de insercion en primera y ultima posicion */
	prueva_de_insercion_en_primera_y_ultima_posicion(lista3,a,b);
	/* pruevas de borrado primera y ultima posicion */
	prueva_de_borrado_en_primera_y_ultima_posicion(lista3,a,b);
	/* destruir listas */
	lista_destruir(lista2,NULL);
	lista_destruir(lista3,NULL);
	print_test("listas destruidas", true);
}

void pruebas_lista_alumno() {
	/* variables a utilizar */
	int a = 15;
	int b = 21;
	/* Inicio de pruebas */
	/* pruevas de primitivas de lista */
	/* con lista vacia */
	pruevas_de_listas_con_lista_vacia();
	/* con un elemento */
	pruevas_de_listas_con_un_elemento(&a,&b);
	/* prueva de destruir lista con elemento que usa heap */
	prueva_de_destruir_lista_con_elemento_que_usa_heap();
	/* pruevas del iterador interno y externo */
	pruevas_del_iterador_interno_y_externo(&a,&b);
}
