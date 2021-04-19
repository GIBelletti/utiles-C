#include "pila.h"
#include <stdlib.h>
#include <stdbool.h>
/* CONSTANTES */
const size_t CAPACIDAD_INICIAL_PILA = 10;
const size_t REDIMENCION_INCREMENTAR_M = 3;//si la cantidad de elementos supera o es igual al 75% de la capacidad se redimensiona para aumentar su capacidad
const size_t REDIMENCION_INCREMENTAR_D = 4;//si la cantidad de elementos supera o es igual al 75% de la capacidad se redimensiona para aumentar su capacidad
const size_t REDIMENCION_DECREMENTAR = 4;//si la cantidad de elementos es menor al 25% de la capacidad se redimensiona para disminuir su capacidad
/* Definición del struct pila proporcionado por la cátedra.*/
struct pila {
	void** datos;
	size_t cantidad;  // Cantidad de elementos almacenados.
	size_t capacidad;  // Capacidad del arreglo 'datos'.
};
/* *****************************************************************
 *			PRIMITIVAS DE LA PILA
 * *****************************************************************/
// Alumno: Belletti Gabriel Ignacio Padron: 100053
/* Crea una pila vacia de 0 elementos con capacidad por defecto (inicial) de 10 */
pila_t *pila_crear(void){
	pila_t *pila_vacia = malloc(sizeof(pila_t));
	if (pila_vacia == NULL){
		return NULL;
	}
	pila_vacia->cantidad = 0;
	pila_vacia->capacidad = CAPACIDAD_INICIAL_PILA;
	pila_vacia->datos = malloc(pila_vacia->capacidad * sizeof(void*));
	if (pila_vacia->datos == NULL) {
		free(pila_vacia);
		return NULL;
	}
	return pila_vacia;
}
/* incrementa el tamaño de la capacidad de la pila */
bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva) {
	void** datos_nuevo = realloc(pila->datos, capacidad_nueva * sizeof(void*));
	if (datos_nuevo == NULL) {
		return false;
	}
	pila->datos = datos_nuevo;
	pila->capacidad = capacidad_nueva;
	return true;
}
/* recibe un puntero a un elemento y otro a una pila y agrega en los datos de la pila la direccion de memoria al elemento recibido, si la cantidad iguala la capacidad antes de agregar el dato se redimencionara la pila para aumentar su capacidad, devuelve false en caso de error */
bool pila_apilar(pila_t *pila,void* elemento){
	if (pila->cantidad * REDIMENCION_INCREMENTAR_D >= pila->capacidad * REDIMENCION_INCREMENTAR_M){
		if (!pila_redimensionar(pila, pila->capacidad * 2) && pila->cantidad == pila->capacidad){
			return false;
		}
	}
	pila->datos[pila->cantidad] = elemento;
	pila->cantidad += 1;
	return (pila->datos[pila->cantidad - 1] == elemento);
}
/* recibe la pila y devuelve true si esta vacia y false si no lo esta, la pila devio haber sido creada */
bool pila_esta_vacia(const pila_t *pila){
	return (pila->cantidad == 0);
}
/* recibe la pila y devuelve sin desapilar el ultimo elemento agregado si esta vacia devuelve NULL */
void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	void* ultimo = pila->datos[pila->cantidad - 1];
	return ultimo;
}
/* recibe la pila y devuelve el ultimo elemento agregado a esta (como puntero), la pila pierde ese ultimo elemento (direccion de memoria), en caso de que la pila este vacia devuelve NULL */
void* pila_desapilar(pila_t *pila){
	if (pila->capacidad > (pila->cantidad * REDIMENCION_DECREMENTAR) && pila->capacidad > CAPACIDAD_INICIAL_PILA){
		pila_redimensionar(pila, pila->capacidad / 2);
	}
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	void* ultimo = pila->datos[pila->cantidad - 1];
	pila->cantidad -= 1;
	pila->datos[pila->cantidad] = NULL;
	return ultimo;
}
/* Destruye la pila. Precondicion: la pila fue creada.Post: se eliminaron todos los elementos de la pila.*/
void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}
