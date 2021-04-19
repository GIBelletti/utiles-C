#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
/* estructuras */
struct nodo_lista{
	void* dato;
	struct nodo_lista* siguiente;
};
typedef struct nodo_lista nodo_lista_t;

struct lista{
	nodo_lista_t* primero;
	nodo_lista_t* ultimo;
	size_t cantidad;
};

struct lista_iter{
	nodo_lista_t* actual;
	nodo_lista_t* anterior;
	lista_t* lista;
};

/* PRIMITIVAS DEL nodo_lista */
/* crea un nodo_lista vacio */
nodo_lista_t* nodo_lista_crear(void* dato,void* siguiente){
	nodo_lista_t* nodo_lista_nuevo = malloc(sizeof(nodo_lista_t));
	if (nodo_lista_nuevo == NULL){
		return NULL;
	}
	nodo_lista_nuevo->dato = dato;
	nodo_lista_nuevo->siguiente = siguiente;
	return nodo_lista_nuevo;
}

/* PRIMITIVAS DE LA LISTA */

/* crea una lista vacia */
lista_t *lista_crear(void){
	lista_t* lista_nueva = malloc(sizeof(lista_t));
	if (lista_nueva == NULL){
		return NULL;
	}
	lista_nueva->primero = NULL;
	lista_nueva->ultimo = NULL;
	lista_nueva->cantidad = 0;
	return lista_nueva;
}

/* recive una lista y si esta esta vacia devuelve true */
bool lista_esta_vacia(const lista_t *lista){
	return (lista->cantidad == 0);
}

/* recive una lista y un dato e inserta este al comienzo de la lista, devuelve true si se completo con exito */
bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_lista_t* nodo_lista = nodo_lista_crear(dato,lista->primero);
	if (nodo_lista == NULL){
		return false;
	}
	if (lista_esta_vacia(lista)){
		lista->ultimo = nodo_lista;
	}
	lista->primero = nodo_lista;
	lista->cantidad += 1;
	return true;
}

/* recive una lista y un dato e inserta este al final de la lista, devuelve true si se completo con exito */
bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_lista_t* nodo_lista = nodo_lista_crear(dato,NULL);
	if (nodo_lista == NULL){
		return false;
	}
	if (lista_esta_vacia(lista)){
		lista->primero = nodo_lista;
	}
	else{
		lista->ultimo->siguiente = nodo_lista;
	}
	lista->ultimo = nodo_lista;
	lista->cantidad += 1;
	return true;
}

/* elimina y devuelve el primer elemento de la lista, si la lista esta vacia devuelve NULL */
void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	nodo_lista_t* desechar = lista->primero;
	void* valor = desechar->dato;
	lista->primero = desechar->siguiente;
	free(desechar);
	lista->cantidad -= 1;
	return valor;
}

/* devuelve el primer elemento de la lista que recibe, si la lista esta vacia devuelve NULL */
void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->primero->dato;
}

/* muestra el ultimo elemento de la lista que recibe, si la lista esta vacia devuelve NULL */
void *lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->ultimo->dato;
}

/* recibe una lista y devuelve la cantidad de elementos que esta tenga */
size_t lista_largo(const lista_t *lista){
	return lista->cantidad;
}

/* recive una lista y una funcion y destruye la lista y los datos con la funcion recibida */
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
	while (!lista_esta_vacia(lista)){
		nodo_lista_t* desechar = lista->primero;
		if(destruir_dato != NULL){
			destruir_dato(desechar->dato);
		}
		if(lista->cantidad == 1){
			lista->ultimo = NULL;
		}
		lista->primero = desechar->siguiente;
		free(desechar);
		lista->cantidad -= 1;
	}
	free(lista);
	return;
}

/* PRIMITIVAS DEL ITERADOR INTERNO */
/* recive una lista una funcion (booleana) y una direccion de memoria, recorre cada elementi de la lista pasado por la funcion y guarda datos en la direcciond de memoria, si la funcion booleana da false o es NULL corta su ejecucion */
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if (visitar == NULL){
		return;
	}
	nodo_lista_t* actual = lista->primero;
	while (actual != NULL){
		if (visitar(actual->dato,extra) == false){
			return;
		}
		actual = actual->siguiente;
	}
	return;
}

/* PRIMITIVAS DEL ITERADOR EXTERNO */

/* crea un iterador para una lista y devuelve su iterador en caso de falla devuelve NULL */
lista_iter_t *lista_iter_crear(lista_t *lista){
	if (lista == NULL){
		return NULL;
	}
	lista_iter_t* iterador_nuevo = malloc(sizeof(lista_iter_t));
	if (iterador_nuevo == NULL){
		return NULL;
	}
	iterador_nuevo->lista = lista;
	iterador_nuevo->actual = iterador_nuevo->lista->primero;
	iterador_nuevo->anterior = NULL;
	return iterador_nuevo;
}

/* recibe el iterador y avanza su posicion devuelve true si avanso con exito o false si (por estar en el final de la lista) no pudo avanzar */
bool lista_iter_avanzar(lista_iter_t *iter){
	if (lista_iter_al_final(iter)){
		return false;
	}
	iter->anterior = iter->actual;
	iter->actual = iter->actual->siguiente;
	return true;
}

/* recibe el iterador y devuelve el dato de la posicion actual del iterador, si esta al final devuelve NULL */
void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (lista_iter_al_final(iter)){
		return NULL;
	}
	return iter->actual->dato;
}

/* recibe un iterador y devuelve true si este llego al final de la lista */
bool lista_iter_al_final(const lista_iter_t *iter){
	return (iter->anterior == iter->lista->ultimo && iter->actual == NULL);
}

/* elimina el iterador, no modifica la lista */
void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
	return;
}

/* recibe un elemento y lo inserta en la posicion actual del iterador, devuelve true si se inserto correctamente y false si hubo un error, la lista gana un elemento */
bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_lista_t* nodo_lista = nodo_lista_crear(dato,iter->actual);
	if (nodo_lista == NULL){
		return false;
	}
	/* si es el primero */
	if (iter->anterior != NULL){
		iter->anterior->siguiente = nodo_lista;
	}
	else {
		iter->lista->primero = nodo_lista;
	}
	/* si es el ultimo */
	if (lista_iter_al_final(iter)){
		iter->lista->ultimo = nodo_lista;
	}
	iter->actual = nodo_lista;
	iter->lista->cantidad += 1;
	return true;
}

/* elimina el elemento de la posicion actual del iterador y lo devuelve, la lista pierde el elemento, en caso de que este en el final devuelve NULL */
void *lista_iter_borrar(lista_iter_t *iter){
	/* si es el ultimo o esta vacia */
	if (lista_iter_al_final(iter)){
		return NULL;
	}
	nodo_lista_t* desechar = iter->actual;
	void* valor = desechar->dato;
	/* si es el primero */
	if (iter->lista->primero == iter->actual){
		iter->lista->primero = iter->actual->siguiente;
	}
	else {
		iter->anterior->siguiente = iter->actual->siguiente;
	}
	/* si es el ultimo */
	if (iter->lista->ultimo == iter->actual){
		iter->lista->ultimo = iter->anterior;
	}
	iter->actual = iter->actual->siguiente;
	free(desechar);
	iter->lista->cantidad -= 1;
	return valor;
}
///////////////////////////////////////////
