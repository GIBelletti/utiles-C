///////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "cola.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

int MAX_TAM = 250;
int VALOR_CERO = '0';
char* ERROR_PARAMETROS = "Cantidad de parametros erronea";
char* ERROR_TIPO = "Tipo de parametro incorrecto";

/* remplaza el \n final del fgetline por el \0 de fin de linea */
void remplazar_chr13(char* cadena){
	if (strlen(cadena) < 1){
		return;
	}
	size_t i = 0;
	while (cadena[i] != '\n'){
		if (cadena[i] == '\0'){return;}
		i++;
	}
	cadena[i] = '\0';
	return;
}

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

/* reecibe un numero entero n > 0 e imprime las ultimas n lieneas que recibio por stdin, caso de que n > cantidad de lineas recividas imprime todo el texto */
bool ultimas_n_lineas(int N){
	if (N <= 0){return true;}
	cola_t* lineas = cola_crear();
	if (lineas == NULL){
		return false;
	}
	size_t tam_cola = 0;
	bool final_archivo = false;
	while (!final_archivo){
		char* linea = obtener_linea(stdin);
		if (linea == NULL){
			free(linea);
			final_archivo = true;
			continue;
		}
		cola_encolar(lineas,linea);
		tam_cola++;
		if (tam_cola > (N)){
			char* dato_a_desteruir = cola_desencolar(lineas);
			free(dato_a_desteruir);
			tam_cola--;
		}
	}
	while (!cola_esta_vacia(lineas)){
		char* cadena0 = cola_desencolar(lineas);
		fprintf(stdout,"%s\n",cadena0);
		free(cadena0);
	}
	cola_destruir(lineas,NULL);//la cola esta vacia
	return true;
}

int main(int argc, const char* argv[]){
	if (argc != 2){
		fprintf(stderr,"%s\n",ERROR_PARAMETROS);
		return 1;
	}
	if (!es_un_numero(argv[1])){
		fprintf(stderr,"%s\n",ERROR_TIPO);
		return 1;
	}
	//int numero = convertir_a_int(argv[1]);
	int numero = atoi(argv[1]);
	if (!ultimas_n_lineas(numero)){
		return 1;
	}
	return 0;
}
///////////////////////////////////////////
