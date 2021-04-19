#define _POSIX_C_SOURCE 200809L //getline
#include "strutil.h"
#include "hash.h"
#include "heap.h"
#include "abb.h"
#include "pila.h"
#include "CONSTANTES.h"
#include "comandos.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

const char* COMANDOS_VALIDOS[] = {"agregar_archivo", "ver_mas_visitados", "ver_visitantes", NULL};

typedef struct elemento{// PARA LAS PAGINAS MAS VISITADAS
	void* dato;
	char* clave;
}elemento_t;


void wrapper_split(void* dato){
	free_strv(dato);
}

/* recibe una pila y la borra junto a su contenido */
void vaciar_y_destruir_pila(pila_t* pila,void (*destruir_dato)(void*)){
	if (destruir_dato == NULL){
		pila_destruir(pila);
		return;
	}
	while (!pila_esta_vacia(pila)){
		destruir_dato(pila_desapilar(pila));
	}
	pila_destruir(pila);
	return;
}

/* recibe un arreglo (de cadenas) (cuyo ultimo elemento es NULL) y dos numero (mayor que 0) y devuelve true si la cantidad de elementos es igual o esta entre los numeros, caso contrario da false */
bool verificacion_de_tam(char** arreglo, size_t tam_min, size_t tam_max){
	if (tam_max == 0 || tam_min > tam_max){//tam nunca < 0
		return false;
	}
	size_t i = 0;
	while (arreglo[i] != NULL){
		i++;
	}
	return (i >= tam_min && i <= tam_max);
}

int comparar_elementos(const void* elem1, const void* elem2){//PARA COMPARAR LA CANTIDAD DE PAGINAS VISITADAS EN EL HEAP
	if(strtoul(((elemento_t*)elem1)->clave, NULL, 0) > strtoul(((elemento_t*)elem2)->clave, NULL, 0))
		return 1;
	if(strtoul(((elemento_t*)elem1)->clave, NULL, 0) == strtoul(((elemento_t*)elem2)->clave, NULL, 0))
		return 0;
	return -1;
}   

/* EJECUTAR FUNCIONES */

bool ejcutar_agregar_archivo(char** arreglo,hash_t* logs,hash_t* hash_visitados,heap_t* heap_visitados,abb_t* abb,pila_t* basurero_de_split,pila_t* basurero_de_logs){
	return agregar_archivo(arreglo[1], logs, hash_visitados, heap_visitados, abb, basurero_de_split, basurero_de_logs);
}

bool ejecutar_ver_visitantes(char** arreglo,hash_t* logs,hash_t* hash_visitados,heap_t* heap_visitados,abb_t* abb,pila_t* basurero_de_split,pila_t* basurero_de_logs){
	return ver_visitados(heap_visitados, strtoul(arreglo[1], NULL, 0));
}

bool ejecutar_ver_mas_visitados(char** arreglo,hash_t* logs,hash_t* hash_visitados,heap_t* heap_visitados,abb_t* abb,pila_t* basurero_de_split,pila_t* basurero_de_logs){
	return ver_ips(abb, arreglo[1], arreglo[2]);
}

//PROGRAMA PRINCIPAL
int main(int argc, char* argv[]){
	hash_t* logs = hash_crear(free);
	hash_t* hash_visitados = hash_crear(free);
	heap_t* heap_visitados = heap_crear(comparar_elementos);
	abb_t* abb = abb_crear(comparar_ip, NULL);
	pila_t* basurero_de_split = pila_crear();
	pila_t* basurero_de_logs = pila_crear();
	/* arreglo de funciones */
	bool (*funciones[CANT_COMANDOS])(char**,hash_t*,hash_t*,heap_t*,abb_t*,pila_t*,pila_t*);
	funciones[0] = ejcutar_agregar_archivo;
	funciones[1] = ejecutar_ver_visitantes;
	funciones[2] = ejecutar_ver_mas_visitados;
	//////////////////////
	char* linea = NULL; size_t capacidad = 0; ssize_t leidos; //Lee las operaciones del usuario
	while((leidos = getline(&linea,&capacidad,stdin)) > 0){
		linea[strlen(linea)-1] = '\0';
		if (strcmp(linea, "^D") == 0){//si la line esta vacia termina la interfaz
			break;
		}
		char** comando = split(linea, ' ');
		pila_apilar(basurero_de_split,comando);
		if (!verificacion_de_tam(comando,MIN_TAM_PARAMETROS,MAX_TAM_PARAMETROS)){
			fprintf(stderr, "Error en comando %s\n", comando[0]);
			continue;
		}
		size_t i = 0;
		while (COMANDOS_VALIDOS[i] != NULL){
			if(strcmp(comando[0], COMANDOS_VALIDOS[i]) == 0){
				bool estado = funciones[i](comando, logs, hash_visitados,heap_visitados, abb, basurero_de_split, basurero_de_logs);
				if(estado == true)
					fprintf(stdout, "OK\n");
				else {fprintf(stderr, "Error en comando %s\n", comando[0]);}
			}
			i++;
		}
		if (COMANDOS_VALIDOS == NULL) {fprintf(stderr, "Error en comando %s\n", comando[0]);}
	}
	hash_destruir(hash_visitados);
	hash_destruir(logs);
	heap_destruir(heap_visitados, free);
	abb_destruir(abb);
	if(!pila_esta_vacia(basurero_de_split)){vaciar_y_destruir_pila(basurero_de_split,wrapper_split);}
	else {pila_destruir(basurero_de_split);}
	if(!pila_esta_vacia(basurero_de_logs)){vaciar_y_destruir_pila(basurero_de_logs,wrapper_log);}
	else {pila_destruir(basurero_de_logs);}
	free(linea);
	return 0;
}
