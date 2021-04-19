#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
/* CONSTANTES */
const char NULO_W = '\0';//caracter nulo
/* FUNCIONES AUXILIARES */
/* recibe dos cadenas y el tamaño de una y copia lo de una cadena a la otra */
void agregar_a_cadena(char* cadena,const char* str,size_t tam){
	if (tam == 0){
		return;
	}
	for (size_t i = 0; i < (tam - 1); i++){
		cadena[i] = str[i];
	}
	cadena[(tam - 1)] = NULO_W;
	return;
}

/* */
/* recibe una cadena y un separador y devuelve un arreglo de cadenas que terminan donde estaria el separador o al final del texto */
char** split(const char* str, char sep){
	size_t contador = 0;//letras en la cadena
	size_t cantidad = 0;//veces que se repite el separador
	while (str[contador] != NULO_W){
		if (str[contador] == sep){
			cantidad++;
		}
		contador++;//llega hasta el ultimo sin contar el nulo
	}
	contador++;//le sumo el \0
	char** arreglo_de_cadenas = malloc((sizeof(char*) * (cantidad + 1 + 1)));//"+1"por la divicion "+1"por el NULL final
	size_t cadena = 0;
	size_t ultimo_sep = 0;
	for (size_t i = 0; i < contador; i++){
		if (str[i] == sep || str[i] == NULO_W){
			size_t diferencia_mas_nulo = (i - ultimo_sep + 1);//"+1"por el \0
			char* n_cadena = malloc(sizeof(char) * diferencia_mas_nulo);
			agregar_a_cadena(n_cadena,&str[ultimo_sep],diferencia_mas_nulo);
			arreglo_de_cadenas[cadena] = n_cadena;
			cadena++;
			ultimo_sep = (i + 1);
		}
	}//este for es O(2n) dado que la funcion O(n) que usa la usa esporadica veces
	arreglo_de_cadenas[cadena] = NULL;
	return arreglo_de_cadenas;
}

/* libera la memoria del arreglo con cadenas */
void free_strv(char* strv[]){
	size_t contador = 0;
	while (strv[contador] != NULL){
		free(strv[contador]);
		contador++;
	}
	free(strv);
	return;
}

/* recibe un arreglo de cadenas terminado en NULL y un caracter y devuelve una cadena concatenando todas las cadenas del arreglo con el caracter recivido, da NULL en cas de error */
char* join(char** arreglo,char caracter){
	char* cadena;
	if (arreglo[0] == NULL){
		cadena = malloc(sizeof(char));
		if (cadena == NULL){
			return NULL;
		}
		cadena[0] = '\0';
		return cadena;
	}
	size_t tam_cadena_02;
	if (arreglo[1] == NULL){
		tam_cadena_02 = (strlen(arreglo[0]) + 1);
		cadena = malloc(sizeof(char) * tam_cadena_02);
		if (cadena == NULL){
			return NULL;
		}
		for (size_t i = 0; i < tam_cadena_02; i++){
			cadena[i] = arreglo[0][i];
		}
		return cadena;
	}
	size_t tam_de_cadena = 0;
	size_t i = 0;
	while (arreglo[i] != NULL){
		tam_de_cadena += (strlen(arreglo[i]) + 1);
		i++;
	}
	i = 0;
	tam_cadena_02 = 0;
	cadena = malloc(sizeof(char) * tam_de_cadena);
	if (cadena == NULL){
		return NULL;
	}
	while (arreglo[i] != NULL){
		size_t tam_cadena_i = (strlen(arreglo[i]) + 1);
		for (size_t j = 0; j < tam_cadena_i; j++){
			if (arreglo[i][j] == '\0'){
				cadena[j + tam_cadena_02] = caracter;
			}
			else {cadena[j + tam_cadena_02] = arreglo[i][j];}
		}
		tam_cadena_02 += tam_cadena_i;
		i++;
	}
	cadena[tam_de_cadena - 1] = '\0';//remplaza el caracter final que esta de mas
	return cadena;
}
/////////////////////////////////////
