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
char* ERROR_PARAMETROS = "Cantidad de parametros erronea";
size_t ARCHIVOS_A_COMPARAR = 2;
char* ERROR_ARCHIVO = "Archivo erroneo";
void imprecion_de_diferencia(char* linea1,char* linea2,size_t numero_de_linea);
//////////FUNCIONES AUXILIARES
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

/* recibe un archivo, una referencia a una cadena del archivo, junto con su tamaño max y un numero y ejecuta una funcion para cada una de las restantes lineas del archivo */
void imprimir_faltantes(FILE* archivo, char* linea, int tam, size_t num, bool primero, pila_t* pila){
	if (tam < (1 + 1)){//"+1" por el "\0" y 1 por al menos un caracter
		return;
	}
	bool fin_del_archivo = false;
	while (!fin_del_archivo){
		if (primero){
			imprecion_de_diferencia(linea,NULL,num);
		}
		else {imprecion_de_diferencia(NULL,linea,num);}
		linea = obtener_linea(archivo);
		pila_apilar(pila,linea);
		if (linea == NULL){
			fin_del_archivo = true;
		}
		num++;
	}
	return;
}

void pila_borrar(pila_t* pila){
	while (!pila_esta_vacia(pila)){
		free(pila_desapilar(pila));
	}
	pila_destruir(pila);
	return;
}
//////////FUNCIONES AUXILIARES

/* imprime las dos cadenas que recive indicando que son diferentes */
void imprecion_de_diferencia(char* linea1,char* linea2,size_t numero_de_linea){
	if (linea1 == NULL){
		linea1 = "";
	}
	if (linea2 == NULL){
		linea2 = "";
	}
	fprintf(stdout,"Diferencia en linea %ld\n",numero_de_linea);
	fprintf(stdout,"< %s\n---\n> %s\n",linea1,linea2);
	return;
}

/* recive 2 cadenas y verifica que sean iguales en caso de ser diferentes devuelve false y si son iguales true */
bool comparar_lineas(char* cadena1,char* cadena2){
	if (cadena1 == NULL && cadena2 == NULL){return true;}
	if (cadena1 == NULL || cadena2 == NULL){return false;}
	size_t i = 0;
	while (cadena1[i] != '\0' || cadena2[i] != '\0') {
		if (cadena1[i] != cadena2[i]){
			return false;
		}
		i++;
	}
	return(cadena1[i]  == '\0' && cadena2[i] == '\0');
}

/* recibe dos nombres de archivos que abre y lee */
bool archivos(const char* archivo1,const char* archivo2){
	FILE* arch1 = fopen(archivo1,"r");
	if (arch1 == NULL){
		fprintf(stderr,"%s\n",ERROR_ARCHIVO);
		return false;
	}
	FILE* arch2 = fopen(archivo2,"r");
	if (arch2 == NULL){
		fprintf(stderr,"%s\n",ERROR_ARCHIVO);
		fclose(arch1);//caso de que se haya abierto el primer archivo
		return false;
	}
	bool final[] = {false,false};//final del archivo {1,2}
	size_t num = 0;
	pila_t* pila = pila_crear();
	char* linea01 = NULL;
	char* linea02 = NULL;
	while (!final[0] && !final[1]){
		char* linea1 = obtener_linea(arch1);
		pila_apilar(pila,linea1);
		char* linea2 = obtener_linea(arch2);
		pila_apilar(pila,linea2);
		num++;
		if (linea1 == NULL && linea2 == NULL){
			final[0] = true;
			final[1] = true;
			continue;
		}
		if (linea1 == NULL){
			final[0] = true;
			linea02 = linea2;
			continue;
		}
		if (linea2 == NULL){
			final[1] = true;
			linea01 = linea1;
			continue;
		}
		if (!comparar_lineas(linea1,linea2)){
			imprecion_de_diferencia(linea1,linea2,num);
		}
	}
	if (!(final[0] && final[1])){//en caso de que un archivo termine se imprimiran todas las lineas del faltante
		if (final[0]){
			imprimir_faltantes(arch2, linea02, (int)strlen(linea02), num, false, pila);
		}
		else {
			imprimir_faltantes(arch1, linea01, (int)strlen(linea01), num, true, pila);
		}
	}
	fclose(arch1);
	fclose(arch2);
	pila_borrar(pila);
	return true;
}

int main(int argc, const char* argv[]){
	if (argc != (ARCHIVOS_A_COMPARAR + 1)){//cantidad de archivos a comparar mas el programa
		fprintf(stderr,"%s\n",ERROR_PARAMETROS);
		return 1;
	}
	if (!archivos(argv[1],argv[2])){
		return 1;
	}
	return 0;
}
