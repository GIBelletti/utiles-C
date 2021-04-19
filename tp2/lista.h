#define LISTA_H
#include <stdbool.h>
#include <stdio.h>
/* DEFINICION DE TIPO DE DATO */
typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;
/* PRIMITIVAS DE LA LISTA */
/* crea una lista vacia, post: devuelve una lista vacia o NULL en caso de que falle */
lista_t *lista_crear(void);
/* verifica si la lista esta vacia, pre: la lista deve existir, post: devuelve true en caso de que no tenga elementos false si tiene al menos uno */
bool lista_esta_vacia(const lista_t *lista);
/* inserta un elemento al principio de la lista, pre: la lista deve existir, post: devuelve true si se inserto el elemento con exito */
bool lista_insertar_primero(lista_t *lista, void *dato);
/* inserta un elemento al final de la lista, pre: la lista deve existir, post: devuelve true si se inserto el elemento con exito */
bool lista_insertar_ultimo(lista_t *lista, void *dato);
/* elimina y devuelve el primer elemento de la lista, pre: la lista deve existir, post: devuelve el dato del primer elemento y lo elimina de la lista */
void *lista_borrar_primero(lista_t *lista);
/* muestra el primer elemento de la lista, pre: la lista deve existir, post: devuelve el dato del primer elemento */
void *lista_ver_primero(const lista_t *lista);
/* muestra el ultimo elemento de la lista, pre: la lista deve existir, post: devuelve el dato del ultimo elemento */
void *lista_ver_ultimo(const lista_t* lista);
/* muestra la cantidad de elementos que tiene la lista, pre: la lista deve existir, post: devuelve la cantidad de elementos que posea la lista */
size_t lista_largo(const lista_t *lista);
/* destruye la lista, pre: la lista deve existir se le puede agregar una funcion para borrar los datos dentro de esta, post: destruye la lista junto con sus datos */
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));
/* PRIMITIVAS DEL ITERADOR INTERNO */
/* aplica una funcion a los elementos de la lista y guarda los resultados en un puntero extra, pre: la lsita existe y el puntero donde guardar los datos puede guardarlos, post: se guardan los datos en el puntero aplicandoles la funcion */
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);
/* PRIMITIVAS DEL ITERADOR EXTERNO */
/* crea un iterador para una lista, pre la lista existe, post: devuelve el iterador de la lista */
lista_iter_t *lista_iter_crear(lista_t *lista);
/* el iterador avanza en la lista, pre: el iterador existe, post: el iterador avanza */
bool lista_iter_avanzar(lista_iter_t *iter);
/* muestra lo que hay en la posicion actual del iterador, pre: el iterador existe, post: devuelve el dato que hay en s posicion actual */
void *lista_iter_ver_actual(const lista_iter_t *iter);
/* dice si llego al final de la lista, pre: el iterador existe, post: si la posicion actual coincide con el final de la lista devuelve true caso contrario false */
bool lista_iter_al_final(const lista_iter_t *iter);
/* destruye el iterador, pre: el iterador existe, post: el iterador queda destruido */
void lista_iter_destruir(lista_iter_t *iter);
/* inserta un elemento en la posicion actual del iterador, pre: el iterador existe, post: se le agrega a la lista un elemento en la poscion donde este el iterador (el dato anerior de dicha posicion se movera a una posicion posterior) */
bool lista_iter_insertar(lista_iter_t *iter, void *dato);
/* elimina el elemento de la posicion actual del iterador, pre: el iterador existe, post: se le borra a la lista un elemento en la poscion donde este el iterador (el dato siguiente de dicha posicion se movera a la posicion actual) */
void *lista_iter_borrar(lista_iter_t *iter);
