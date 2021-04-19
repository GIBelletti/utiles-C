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

const char* COMANDOS_VALIDOS[] = {"agregar_archivo", "ver_visitantes", "ordenar_archivo", NULL};

typedef struct elemento{// PARA LAS PAGINAS MAS VISITADAS
	void* dato;
	char* clave;
}elemento_t;


void wrapper_split(void* dato){
	free_strv(dato);
}

/* recibe una cadena devuelve true si es un numero, si es NULL da false */
bool es_un_numero(const char* cadena){
	if (cadena == NULL){
		return false;
	}
	size_t i = 0;
	bool entero = true;
	while(cadena[i] != '\0'){
		if (cadena[i] == '.' || cadena[i] == ',' || cadena[i] < '0' || cadena[i] > '9'){
			entero = false;
			break;
		}
		i++;
	}
	return entero;
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

//PROGRAMA PRINCIPAL
int main(int argc, char* argv[]){
	if (argc != 2){
		return 1;
	}
	if (!es_un_numero(argv[1])){
		return 1;
	}
	int numero = atoi(argv[1]);
///////////////////////
	heap_t* heap_visitados = heap_crear(comparar_elementos);
	abb_t* abb = abb_crear(comparar_ip, wrapper_log);
	//////////////////////
	char* linea = NULL; size_t capacidad = 0; ssize_t leidos; //Lee las operaciones del usuario
	while((leidos = getline(&linea,&capacidad,stdin)) > 0){
		if (linea == NULL){break;}
		bool valido = false;
		bool finalizar = false;
		bool corte = false;
		size_t tam = strlen(linea);
		linea[tam-1] = '\0';
		char** comando = split(linea, ' ');
//		pila_apilar(basurero_de_split,comando);
		if (!verificacion_de_tam(comando,MIN_TAM_PARAMETROS,MAX_TAM_PARAMETROS)){
			fprintf(stderr, "Error en comando %s\n", comando[0]);
			free_strv(comando);
			break;
		}
		size_t i = 0;
		while (COMANDOS_VALIDOS[i] != NULL){
			if(strcmp(comando[0], COMANDOS_VALIDOS[i]) == 0){
				valido = true;
				break;
			}
			i++;
		}
////////////
		if (valido){
			if(i == 0 && !corte){
				if (agregar_archivo(comando[1], abb)){
					fprintf(stdout, "OK\n");
				}
				else{
					corte = true;
				}
			}
			if (comando[2] == NULL && i >= 1){
					corte = true;
			}
			if(i == 1 && !corte){
				if (ver_ips(abb, comando[1], comando[2])){
					fprintf(stdout, "OK\n");
				}
				else{
					corte = true;
				}
			}
			if(i == 2 && !corte){
				if (ordenar_archivo(comando[1], comando[2], (size_t)numero)){
					fprintf(stdout, "OK\n");
				}
				else{
					corte = true;
				}
			}
		}
/////////
		if ((COMANDOS_VALIDOS[i] == NULL && !valido) || corte){
			fprintf(stderr, "Error en comando %s\n", comando[0]);
			finalizar = true;
		}
		free_strv(comando);
		if (finalizar){
			break;
		}
	}
	free(linea);
	heap_destruir(heap_visitados, free);
	abb_destruir(abb);
	return 0;
}
