#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
//Alumno: Belletti Gabriel Ignacio Padron: 100053 Corrector: Jorge Collinet Grupo: G38
/* ******************************************************************
 *			PRUEBAS UNITARIAS
 * *****************************************************************/

int DEFAULT(const void *a, const void *b){
	if(*(int*)a > *(int*)b)
		return 1;
	if(*(int*)a == *(int*)b)
		return 0;
	else return -1;
}

void wrapper_free(void* dato){
	free(dato);
	return;
}

static void prueba_crear_heap_vacio()
{
	heap_t* heap = heap_crear(DEFAULT);
	print_test("Prueba heap crear heap vacio", heap);
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
	print_test("Prueba heap desencolar es NULL", !heap_desencolar(heap));
	print_test("Prueba heap vermaximo es NULL", !heap_ver_max(heap));
	heap_destruir(heap, NULL);
}

static void prueba_heap_agregar()
{
	int valor1 = 3; int valor2 = 2; int valor3 = 20; int valor4 = 6; int valor5 = -21; int valor6 = 99; int valor7 = 76; int valor8 = 34; int valor9 = -31; int valor10 = 0;
	heap_t* heap = heap_crear(DEFAULT);
	print_test("Prueba heap crear heap", heap);
	print_test("Prueba heap insertar valor1", heap_encolar(heap, &valor1));
	print_test("Prueba heap ver maximo es valor1", heap_ver_max(heap) == &valor1);
	print_test("Prueba heap desencolar es valor1", heap_desencolar(heap) == &valor1);
	print_test("Prueba heapesta vacio es true", heap_esta_vacio(heap));
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
	print_test("Prueba heap ver maximo es NULL", heap_ver_max(heap) == NULL);
	print_test("Prueba heap desencolar es NULL", heap_desencolar(heap) == NULL);


	print_test("Prueba heap insertar valor1", heap_encolar(heap, &valor1));
	print_test("Prueba heap insertar valor2", heap_encolar(heap, &valor2));
	print_test("Prueba heap insertar valor3", heap_encolar(heap, &valor3));
	print_test("Prueba heap insertar valor4", heap_encolar(heap, &valor4));
	print_test("Prueba heap insertar valor5", heap_encolar(heap, &valor5));
	print_test("Prueba heap insertar valor6", heap_encolar(heap, &valor6));
	print_test("Prueba heap insertar valor7", heap_encolar(heap, &valor7));
	print_test("Prueba heap insertar valor8", heap_encolar(heap, &valor8));
	print_test("Prueba heap insertar valor9", heap_encolar(heap, &valor9));
	print_test("Prueba heap insertar valor10", heap_encolar(heap, &valor10));
	print_test("Prueba heap la cantidad de elementos es 10", heap_cantidad(heap) == 10);
	print_test("Prueba heap ver maximo es valor6", heap_ver_max(heap) == &valor6);
	heap_destruir(heap, NULL);
}
static void prueba_heap_con_destruir()
{
	int* valor1 = malloc(sizeof(int*)); int* valor2 = malloc(sizeof(int*)); int* valor3 = malloc(sizeof(int*));int* valor4 = malloc(sizeof(int*)); int* valor5 = malloc(sizeof(int*)); 
	*valor1 = 3; *valor2 = 2; *valor3 = 20; *valor4 = 6; *valor5 = -21;
	heap_t* heap = heap_crear(DEFAULT);
	print_test("Prueba heap crear heap", heap);
	print_test("Prueba heap insertar valor1", heap_encolar(heap, valor1));
	print_test("Prueba heap insertar valor2", heap_encolar(heap, valor2));
	print_test("Prueba heap insertar valor3", heap_encolar(heap, valor3));
	print_test("Prueba heap insertar valor4", heap_encolar(heap, valor4));
	print_test("Prueba heap insertar valor5", heap_encolar(heap, valor5));
	heap_destruir(heap, wrapper_free);
}
static void prueba_heap_desde_array()
{
	size_t TAM = 5;
	int** arr = malloc(sizeof(int*)*TAM);
	int arr1[5] = {-2, 3, 20, 564, -324};
	for(int i = 0; i < TAM; i++)
		arr[i] = &arr1[i];
	heap_t* heap = heap_crear_arr((void*)arr, TAM, DEFAULT);
	print_test("Prueba heap crear heap", heap);
	print_test("Prueba heap ver maximo es 564", heap_ver_max(heap) == &arr1[3]);
	print_test("Prueba heap desencolar es 564", heap_desencolar(heap) == &arr1[3]);
	print_test("Prueba heap la cantidad de elementos es 4", heap_cantidad(heap) == 4);
	print_test("Prueba heap ver maximo es 20", heap_ver_max(heap) == &arr1[2]);
	print_test("Prueba heap desencolar es 20", heap_desencolar(heap) == &arr1[2]);
	print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
	print_test("Prueba heap ver maximo es 3", heap_ver_max(heap) == &arr1[1]);
	print_test("Prueba heap desencolar es 3", heap_desencolar(heap) == &arr1[1]);
	print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
	print_test("Prueba heap ver maximo es -2", heap_ver_max(heap) == &arr1[0]);
	print_test("Prueba heap desencolar es -2", heap_desencolar(heap) == &arr1[0]);
	print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap ver maximo es -324", heap_ver_max(heap) == &arr1[4]);
	print_test("Prueba heap desencolar es -324", heap_desencolar(heap) == &arr1[4]);
	print_test("Prueba heapesta vacio es true", heap_esta_vacio(heap));
	heap_destruir(heap, wrapper_free);
	free(arr);
}

static void prueba_heap_volumen()
{
 	size_t TAM = 1000;
	int** arr = malloc(sizeof(int*)*TAM);
	int arr1[TAM];
	for(int i = 0; i < TAM; i++)
		arr1[i] = rand();
	for(int i = 0; i < TAM; i++)
		arr[i] = &arr1[i];
	heap_t* heap = heap_crear_arr((void*)arr, TAM, DEFAULT);
	print_test("Prueba heap crear heap", heap);
	print_test("Prueba heap la cantidad de elementos es 1000", heap_cantidad(heap) == TAM);
	heap_destruir(heap, NULL);
	free(arr);
}

static void prueba_heapsort()
{
	size_t TAM = 5;
	int** arr = malloc(sizeof(int*)*TAM);
	int arr1[5] = {-2, 4, 20, 564, -324};
	for(int i = 0; i < TAM; i++)
		arr[i] = &arr1[i];
	heap_sort((void*)arr, TAM, DEFAULT);
	bool ordenado = true;
	for(int i = 1; i < TAM; i++){
		//printf("%i\n", *arr[i]);
		if (*arr[i - 1] > *arr[i]){
			ordenado = false;
		}
	}
	print_test("el arreglo esta ordenado", ordenado);
	free(arr);
}

void pruebas_heap_alumno()
{
	prueba_crear_heap_vacio();
	prueba_heap_agregar();
	prueba_heap_con_destruir();
	prueba_heap_desde_array();
	prueba_heap_volumen();
	prueba_heapsort();
}
