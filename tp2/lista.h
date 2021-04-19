#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista esta planteada como una lista de punteros genericos*/
typedef struct lista lista_t;
/* El iterador externo actua sobre la lista en concreto*/
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene o no elementos enlistados.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Inserta en la primera posicion de la lista un elemento.
// Pre: la lista fue creada.
// Post: se agrego un elemento a la lista, se encuentra en la primer posicion.
bool lista_insertar_primero	(lista_t *lista, void *dato);

// Inserta en la ultima posicion de la lista un elemento.
// Pre: la lista fue creada.
// Post: se agrego un elemento a la lista, se encuentra en la ultima posicion.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Obtiene la cantidad de elementos enlistados.
// Pre: la lista fue creada.
// Post: se devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));


/* ******************************************************************
 *                 PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera la lista hasta que visitar de false o termine de iterar.
// Pre: la lista fue creada.
// Post: Se itero hasta que funcion dio false o se termino la lista.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                 PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea el iterador externo para su uso.
// Pre: la lista fue creada.
// Post: el iterador se encuentra en la primer posicion de la lista, a menos que no haya elementos, por lo cual apunta al ultimo.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posicion sobre la lista.
// Pre: el iterador fue creado.
// Post: el iterador apunta al siguiente, si no hay siguiente devuelve false.
bool lista_iter_avanzar(lista_iter_t *iter);

// Muestra el elemento sobre el cual el iterador esta parado.
// Pre: el iterador fue creado.
// Post: se devolvio el valor sobre el que se encuentra. Si esta al final, devuelve NULL.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Indica true o false dependiendo si se encuentra al final de la lista o no.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye al estructura del iterador.
// Pre: el iterador fue creado.
// Post: el iterador fue destruido.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento a la lista en la posicion del iterador.
// Pre: el iterador fue creado.
// Post: la lista posee un elemento mas.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el elemento al que apunta el iterador.
// Pre: el iterador fue creado.
// Post: se devuelve el valor eliminado.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_alumno(void);

#endif // LISTA_H