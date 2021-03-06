#include "cola.h"
#include <stdlib.h>
#include <stdbool.h>
/* estructuras */
struct nodo{
	void* dato;
	struct nodo* siguiente;
};
typedef struct nodo nodo_t;

struct cola{
	nodo_t* primero;
	nodo_t* ultimo;
};

/* ******************************************************************
 *                    PRIMITIVAS DEL NODO
 * *****************************************************************/
/* crea un nodo vacio */
nodo_t* nodo_crear(void* dato,void* siguiente){
	nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
	if (nodo_nuevo == NULL){
		return NULL;
	}
	nodo_nuevo->dato = dato;
	nodo_nuevo->siguiente = siguiente;
	return nodo_nuevo;
}

/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

/* Crea una cola y la devuelve vacia */
cola_t* cola_crear(void){
	cola_t *cola_nueva = malloc(sizeof(cola_t));
	if (cola_nueva == NULL){
		return NULL;
	}
	cola_nueva->primero = NULL;
	cola_nueva->ultimo = NULL;
	return cola_nueva;
}

/* Destruye la cola. Si se recibe la función destruir_dato por parámetro, para cada uno de los elementos de la cola llama a destruir_dato. Precondicion: la cola fue creada. destruir_dato es una función capaz de destruir los datos de la cola, o NULL en caso de que no se la utilice. Postcondicion: se eliminaron todos los elementos de la cola.*/
void cola_destruir(cola_t *cola, void destruir_dato(void*)){
	while (!cola_esta_vacia(cola)){
		nodo_t* desechar = cola->primero;
		cola->primero = cola->primero->siguiente;
		if (destruir_dato != NULL){
			destruir_dato(desechar->dato);
		}
		free(desechar);
	}
	free(cola);
	return;
}

/* Devuelve verdadero o falso, según si la cola tiene o no elementos encolados. Pre: la cola fue creada. */
bool cola_esta_vacia(const cola_t *cola){
	return(cola->primero == NULL);
}

/* Agrega un nuevo elemento a la cola. Devuelve falso en caso de error. Precondicion: la cola fue creada. Postcondicion: se agregó un nuevo elemento a la cola, valor se encuentra al final de la cola. */
bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo = nodo_crear(valor,NULL);
	if (nodo == NULL){
		return false;
	}
	if (cola_esta_vacia(cola)){
		cola->primero = nodo;
	}
	else {
		cola->ultimo->siguiente = nodo;
	}
	cola->ultimo = nodo;
	return true;
}

/* Obtiene el valor del primer elemento de la cola. Si la cola tiene elementos, se devuelve el valor del primero, si está vacía devuelve NULL. Precondicion: la cola fue creada. Postcondicion: se devolvió el primer elemento de la cola, cuando no está vacía.*/
void* cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	return cola->primero->dato;
}

/* Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL. Pre: la cola fue creada. Post: se devolvió el valor del primer elemento anterior, la cola contiene un elemento menos, si la cola no estaba vacía. */
void* cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	nodo_t* nodo_desenganchado = cola->primero;
	void* valor = nodo_desenganchado->dato;
	if (cola->primero == cola->ultimo){
		cola->primero = NULL;
		cola->ultimo = NULL;
	}
	else{
		cola->primero = cola->primero->siguiente;
	}
	free(nodo_desenganchado);
	return valor;
}
