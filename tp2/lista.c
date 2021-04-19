#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct nodo{
	void* dato;
	struct nodo* prox;
} nodo_t;

struct lista{
	nodo_t* prim;
	nodo_t* ult;
	size_t largo;
};

struct lista_iter{
	nodo_t* actual;
	nodo_t* anterior;
	lista_t* lista;
};

nodo_t* nodo_crear(void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL)
		return NULL;
	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}

void* nodo_destruir(nodo_t* nodo, void destruir_dato(void*)){
	void* dato = nodo->dato;
	if(destruir_dato != NULL){
		destruir_dato(nodo->dato);
		free(nodo);
		return NULL;
	}
	free(nodo);
	return dato;
}

lista_t *lista_crear(void){
	lista_t* lista = malloc(sizeof(lista_t));
	if(lista == NULL)
		return NULL;
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	if(lista == NULL)
		return NULL;
	return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	if(lista == NULL)
		return false;
	nodo_t* nodo = nodo_crear(dato);
	if(nodo == NULL)
		return NULL;
	if(lista_esta_vacia(lista) == true)
		lista->ult = nodo;
	nodo->prox = lista->prim;
	lista->prim = nodo;
	lista->largo ++;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	if(lista == NULL)
		return false;
	nodo_t* nodo = nodo_crear(dato);
	if(nodo == NULL)
		return NULL;
	nodo->prox = NULL;
	if(lista_esta_vacia(lista) == true){
		lista->prim = nodo;
	}
	else lista->ult->prox = nodo;
	lista->ult = nodo;
	lista->largo ++;
	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if(lista == NULL || lista_esta_vacia(lista) == true)
		return NULL;
	if(lista->prim == lista->ult)lista->ult = NULL;
	nodo_t* actual = lista->prim->prox;
	void* dato = nodo_destruir(lista->prim, NULL);
	lista->prim = actual;
	lista->largo --;
	return dato;
}

void *lista_ver_primero(const lista_t *lista){
	if(lista_esta_vacia(lista) == true)
		return NULL;
	return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if(lista_esta_vacia(lista) == true)
		return NULL;
	return lista->ult->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	while(lista->prim != NULL){
		nodo_t* aux = lista->prim->prox;
		nodo_destruir(lista->prim, destruir_dato);
		lista->prim = aux;
	}
	free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if(visitar == NULL)
		return;
	nodo_t* iter = lista->prim;
	while(iter != NULL){
		nodo_t* aux = iter->prox;
		if(visitar(iter->dato, extra) == false)
			return;
		iter = aux;
	}
	return;
}

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(iter == NULL)
		return NULL;
	iter->lista = lista;
	iter->actual = lista->prim;
	iter->anterior = NULL;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if(iter->actual == NULL)
		return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if(iter == NULL || iter->actual == NULL)
		return NULL;
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter){
	if(iter == NULL)
		return;
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_t* nodo = nodo_crear(dato);
	if(nodo == NULL)
		return NULL;
	nodo->prox = iter->actual;
	iter->actual = nodo;
	iter->lista->largo ++;
	if(iter->actual->prox == NULL)
		iter->lista->ult = iter->actual;
	if(iter->anterior == NULL)
		iter->lista->prim = iter->actual;
	else iter->anterior->prox = iter->actual;
	return true;
}

void* lista_iter_borrar(lista_iter_t *iter){
	void* dato;
	if(iter->actual == NULL)
		return NULL;
	if(iter->anterior != NULL){
		iter->anterior->prox = iter->actual->prox;
		if(iter->actual->prox == NULL)
			iter->lista->ult = iter->anterior;
		dato = nodo_destruir(iter->actual, NULL);
		iter->actual = iter->anterior->prox;
	}
	else{
		iter->lista->prim = iter->actual->prox;
		dato = nodo_destruir(iter->actual, NULL);
		iter->actual = iter->lista->prim;
	}
	iter->lista->largo --;
	return dato;
}
