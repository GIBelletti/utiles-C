#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


/* ******************************************************************
 *			PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
// Alumno: Belletti Gabriel Ignacio Padron: 100053
// FUNCIONES AUXILIARES
void wrapper_lista(void* dato){
	lista_destruir(dato,NULL);
}

void wrapper_free(void* dato){
	free(dato);
}

bool copiar_lista(void* dato, void* extra){
	lista_insertar_ultimo(extra, dato);
	return true;
}

bool es_igual(void* dato, void* extra){
	int *V1 = (int*)dato;
	int *V2 = (int*)extra;
	if(*V1 == *V2){
		return true;
	}
	dato = NULL;//cambio el dato para demostrar que se corta la iteracion en la posicion, si no se cortara en dicha pocicion todos los demas datos serian NULL
	return false;
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

bool son_iguales(lista_t* lista1, lista_t* lista2){
	lista_iter_t* iter1 = lista_iter_crear(lista1);
	lista_iter_t* iter2 = lista_iter_crear(lista2);
	while (!lista_iter_al_final(iter1) && !lista_iter_al_final(iter2)){
		int* elemento1 = lista_iter_ver_actual(iter1);
		int* elemento2 = lista_iter_ver_actual(iter2);
		if (*elemento1 != *elemento2){
			printf("%d %d\n",*elemento1,*elemento2);
			lista_iter_destruir(iter1);
			lista_iter_destruir(iter2);
			return false;
		}
		lista_iter_avanzar(iter1);
		lista_iter_avanzar(iter2);
	}
	bool terminan_igual = false;
	if (lista_iter_al_final(iter1) && lista_iter_al_final(iter2)){
		terminan_igual = true;
	}
	lista_iter_destruir(iter1);
	lista_iter_destruir(iter2);
	return terminan_igual;
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
	lista_destruir(lista1,*wrapper_lista);
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
	print_test("verificacion de copia igual", son_iguales(lista2,lista3));
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

void pruevas_volumen_y_corte(int* a,int* b){
	lista_t* lista = lista_crear();
	print_test("lista existe", lista != NULL);
	for(size_t i = 0;i < *a;i++){
		int* elemento = malloc(sizeof(int));
		lista_insertar_primero(lista, elemento);
	}
	for(size_t i = 0;i < *b;i++){
		int* elemento = malloc(sizeof(int));
		lista_insertar_ultimo(lista, elemento);
	}
	print_test("lista no vacia", !lista_esta_vacia(lista));
	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("iterador existe", iter != NULL);
	enviar_al_final(iter);
	print_test("iterador al final", lista_iter_al_final(iter));
	print_test("iterador al final da false cuando intenta avanzar en lista con elementos, prueva 1", !lista_iter_avanzar(iter));
	print_test("iterador al final da false cuando intenta avanzar en lista con elementos, prueva 2", !lista_iter_avanzar(iter));
	lista_iter_destruir(iter);
	print_test("iterador destruido", true);
	lista_destruir(lista,wrapper_free);
	print_test("lista con muchos elementos destruida", true);
}

void prueva_de_corte_lista_vacia(){
	lista_t* lista = lista_crear();
	print_test("lista existe", lista != NULL);
	lista_iter_t* iter = lista_iter_crear(lista);
	print_test("iterador existe", iter != NULL);
	print_test("iterador da false cuando intenta avanzar en lista vacia, prueva 1", !lista_iter_avanzar(iter));
	print_test("iterador da false cuando intenta avanzar en lista vacia, prueva 2", !lista_iter_avanzar(iter));
	lista_iter_destruir(iter);
	print_test("iterador destruido", true);
	lista_destruir(lista,wrapper_free);
	print_test("lista con muchos elementos destruida", true);
}

void pruevas_de_iterador_interno(int* a,int* b){
	lista_t* lista = lista_crear();
	print_test("lista existe", lista != NULL);
	for(size_t i = 0;i < *a;i++){
		lista_insertar_primero(lista, a);
	}
	print_test("lista no vacia", !lista_esta_vacia(lista));
	lista_iterar(lista, *es_igual, a);
	lista_iter_t* iter1 = lista_iter_crear(lista);
	bool es_igual_1 = true;
	while (!lista_iter_al_final(iter1)){
		int* c = lista_iter_ver_actual(iter1);
		if (*c != *a){
			es_igual_1 = false;
			break;
		}
		lista_iter_avanzar(iter1);
	}
	lista_iter_destruir(iter1);
	print_test("prueva del iterador interno que da true", es_igual_1);
	for(size_t i = 0;i < *b;i++){
		lista_insertar_ultimo(lista, b);
	}
	lista_iterar(lista, *es_igual, a);
	lista_iter_t* iter2 = lista_iter_crear(lista);
	bool es_igual_2 = true;
	int* d;
	size_t i = 0;
	while (!lista_iter_al_final(iter2) && i < *a){
		lista_iter_avanzar(iter2);
		i++;
	}//el while termina en el primer valor diferente
	d = lista_iter_ver_actual(iter2);
	if (d == NULL){
		es_igual_2 = false;
	}
	//verificacion de que sea NULL el primer y solo el primer dato diferente
	else {
		lista_iter_avanzar(iter2);
		d = lista_iter_ver_actual(iter2);
		if (d == NULL){//se incrementa uno y se verifica que no sea NULL si lo es el iterador NO corto cuando dio false
			es_igual_2 = false;
		}
	}
	lista_iter_destruir(iter2);
	print_test("prueva del iterador interno que da false", es_igual_2);
	lista_destruir(lista,NULL);
	print_test("lista con muchos elementos destruida", true);
}

void pruebas_lista_alumno() {
	/* variables a utilizar */
	int a = 150;
	int b = 210;
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
	/* pruevas de volumen y corte de iterador */
	pruevas_volumen_y_corte(&a,&b);
	prueva_de_corte_lista_vacia();
	/* pueva del iterador interno */
	pruevas_de_iterador_interno(&a,&b);
}
