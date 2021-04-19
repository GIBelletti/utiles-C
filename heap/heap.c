#include <stdbool.h>
#include <stdlib.h>
#include "heap.h"
#include <unistd.h> 
#include <stddef.h>
#include <stdio.h>
//Alumno: Belletti Gabriel Ignacio Padron: 100053 Corrector: Jorge Collinet Grupo: G38
/* CONSTANTES */
const size_t TAM_BASE = 20;

struct heap{
	size_t cantidad;
	size_t cant_max;
	cmp_func_t comparar;
	void** arreglo;
};

void heap_redimensionar(heap_t* heap){
	if(heap->cantidad == heap->cant_max)
		heap->cant_max = heap->cant_max * 2;
	if(heap->cantidad <= (heap->cant_max / 4) && heap->cantidad >= 20)
		heap->cant_max = heap->cant_max / 2;
	void* datos_nuevo = realloc(heap->arreglo, heap->cant_max * sizeof(void*));
	heap->arreglo = datos_nuevo;
}
/* cambia las pociciones de dos punteros en un arreglo */
void swap_vector(void** vector,size_t pos_1, size_t pos_2){
	void* algo = vector[pos_1];
	vector[pos_1] = vector[pos_2];
	vector[pos_2] = algo;
	return;
}

ssize_t posicion_padre(size_t posicion_hijo){
	if (posicion_hijo == 0){return -1;}
	return (ssize_t)((posicion_hijo - 1) / 2);
}

size_t posicion_hijo_der(size_t posicion_padre){
	return ((posicion_padre * 2) + 2);
}

size_t posicion_hijo_izq(size_t posicion_padre){
	return ((posicion_padre * 2) + 1);
}

void upheap(heap_t* heap, size_t pos){
	ssize_t padre = posicion_padre(pos);
	if(padre == -1)
		return;
	if(heap->comparar(heap->arreglo[pos], heap->arreglo[padre]) > 0){
		swap_vector(heap->arreglo, pos, (size_t)padre);
		upheap(heap, (size_t)padre);
	}
}

void downheap(heap_t* heap, size_t pos){
	while(pos < heap->cantidad){
		size_t pos_max = pos;
		size_t izq = posicion_hijo_izq(pos);
		size_t der = posicion_hijo_der(pos);
		if(izq < heap->cantidad && heap->comparar(heap->arreglo[izq], heap->arreglo[pos_max]) > 0)
			pos_max = izq;
		if(der < heap->cantidad && heap->comparar(heap->arreglo[der], heap->arreglo[pos_max]) > 0)
			pos_max = der;
		if(pos_max == pos)
			return;
		swap_vector(heap->arreglo, pos_max, pos);
		downheap(heap, pos_max);
		pos = pos_max;
	}
}

heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap_nuevo = malloc(sizeof(heap_t));
	if (heap_nuevo == NULL){return NULL;}
	heap_nuevo->arreglo = malloc(sizeof(void*) * TAM_BASE);
	if (heap_nuevo->arreglo == NULL){return NULL;}
	heap_nuevo->cantidad = 0;
	heap_nuevo->comparar = cmp;
	heap_nuevo->cant_max = TAM_BASE;
	return heap_nuevo;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap_nuevo = malloc(sizeof(heap_t));
	if (heap_nuevo == NULL){return NULL;}
	heap_nuevo->arreglo = malloc(sizeof(void*) * n);
	for(size_t i = 0; i < n; i++){
		heap_nuevo->arreglo[i] = arreglo[i];
	}
	heap_nuevo->cantidad = n;
	heap_nuevo->cant_max = n;
	heap_nuevo->comparar = cmp;
	ssize_t pos = posicion_padre(n - 1);//toma +- desde el ultimo "nodo" con al menos un hijo
	while (pos >= (pos/2)){
		downheap(heap_nuevo, (size_t)pos);
		pos--;
	}
	return heap_nuevo;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if(destruir_elemento == NULL){
		free(heap->arreglo);
		free(heap);
		return;
	}
	for(size_t i = 0; i < heap->cantidad; i++){
		destruir_elemento(heap->arreglo[i]);
	}
	free(heap->arreglo);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return (heap->cantidad == 0);
}

bool heap_encolar(heap_t *heap, void *elem){
	heap_redimensionar(heap);
	heap->arreglo[heap->cantidad] = elem;
	upheap(heap,heap->cantidad);
	heap->cantidad++;
	heap_redimensionar(heap);
	return true;
}

void *heap_ver_max(const heap_t *heap){
	if(heap_esta_vacio(heap))
		return NULL;
	return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap){
	if (heap->cantidad == 0){return NULL;}
	heap->cantidad--;
	swap_vector(heap->arreglo,0,heap->cantidad);//ya se le resto uno antes
	void* devolver = heap->arreglo[heap->cantidad];
	heap->arreglo[heap->cantidad] = NULL;
	downheap(heap,0);
	heap_redimensionar(heap);
	return devolver;
}

/* FUNCION DE ORDENAMIENTO */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heap_t* heap = heap_crear_arr(elementos, cant, cmp);
	for(size_t i = 0; i < cant; i++)
		elementos[(cant-1) - i] = heap_desencolar(heap);
	heap_destruir(heap, NULL);
}
//////////////////////////////////////////////////
