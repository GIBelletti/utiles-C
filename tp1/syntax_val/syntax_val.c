#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "pila.h"
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

const size_t CANT_CARACTERES = 3;
const char* COMILLAS = "'";
const char* CARACTERES_ABIERTOS = "{[(";
const char* CARACTERES_CERRADOS = "}])";
const char* DESBALANCEADO = "ERROR";
const char* BALANCEADO = "OK";


/* lee y devuelve una linea del archivo que recibe (la linea que devuelve nesesita ser liberada incluso en caso de falla) */
char* obtener_linea(FILE* arch){
	char* linea = NULL;
	size_t capacidad = 0;
	ssize_t val = getline(&linea, &capacidad, arch);
	if (val < 0){
		free(linea);
		return NULL;
	}
	size_t ultimo = strlen(linea) - 1;
	if (linea[ultimo] != '\0'){
		linea[ultimo] = '\0';
	}
	return linea;
}

/* recibe una pila y la vacia */
void vaciar_pila(pila_t* pila){
	while (!pila_esta_vacia(pila)){
		pila_desapilar(pila);
	}
	return;
}

/* recibe un caracter y una lista de caracteres, si se encuentra en la lista da true si no false */
bool verificacion_de_caracter(char caracter, const char* lista, size_t tam){
	for (size_t i = 0; i < tam; i++){
		if (caracter == lista[i]){
			return true;
		}
	}
	return false;
}

/* recibe dos caracteres da true si el segundo es el cerramiento del primero (El primero de ve estar en la lista de abiertos) */
bool verificacion_de_cerrado(char c_abierto, char c_cerrado, size_t tam, const char* abiertos, const char* cerrados){
	for (size_t i = 0; i < tam; i++){
		if (c_abierto == abiertos[i] && c_cerrado == cerrados[i]){
			return true;
		}
	}
	return false;
}

/* recibe una pila en la cual va apilando ' " [ ( { y desapilandolos segun encuentre sus cierres */
void es_balanceado(pila_t* abierto){
	bool final_de_archivo = false;
	while (!final_de_archivo){
		char* linea = obtener_linea(stdin);
		if (linea == NULL){
			final_de_archivo = true;
			free(linea);
			continue;
		}
		size_t i = 0;
		bool esta_balanceado = true;
		while (linea[i] != '\0' && esta_balanceado){
			if (linea[i] == COMILLAS[0]){
				size_t j = i + 1;
				while (linea[j] != COMILLAS[0]){//caso de ser comilla simple saltea todo hasta encontrar otra comilla simple
					if (linea[j] == '\0'){
						esta_balanceado = false;
						j = i - 1;
						break;
					}
					j++;
				}
				i = j + 1;//seria i = j - 1 despues +1 (por que el linea[i + 1] = comilla) +1 (para no ser comilla)
				continue;
			}
			if (verificacion_de_caracter(linea[i], CARACTERES_ABIERTOS, CANT_CARACTERES)){
				pila_apilar(abierto,&linea[i]);
			}
			else if (verificacion_de_caracter(linea[i], CARACTERES_CERRADOS, CANT_CARACTERES)){
				if (pila_esta_vacia(abierto)){
					esta_balanceado = false;
					continue;
				}
				char* caracter = pila_desapilar(abierto);
				if (!verificacion_de_cerrado(*caracter, linea[i], CANT_CARACTERES, CARACTERES_ABIERTOS, CARACTERES_CERRADOS)){
					esta_balanceado = false;
					vaciar_pila(abierto);
					continue;
				}
			}
			i++;
		}
		if (!pila_esta_vacia(abierto) || !esta_balanceado){
			fprintf(stdout,"%s\n",DESBALANCEADO);
		}
		//else if (pila_esta_vacia(abierto) && esta_balanceado) {//p'+q'=(pq)'
		else {
			fprintf(stdout,"%s\n",BALANCEADO);
		}
		vaciar_pila(abierto);
		free(linea);
	}
	return;
}

int main (){
	pila_t* abierto = pila_crear();
	if(abierto == NULL){
		return 1;
	}
	es_balanceado(abierto);
	pila_destruir(abierto);
	return 0;
}
