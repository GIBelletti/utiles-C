/*MARTIN CURZEL 100049
  CASIMIRO PASTINE*/
#include "strutil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char** split(const char* str, char sep){
	size_t cant = 2;
	size_t i = 0;
	size_t cant_caracteres = strlen(str);
	for(i = 0; i < cant_caracteres; i++){
		if(str[i] == sep)
			cant ++;
	}
	size_t corte[cant];
	corte[0] = 0;
	size_t j = 1;
	size_t cant_corte[cant];
	for(i = 0; i < cant; i++)
		cant_corte[i] = 1;
	for(i = 0; i <= cant_caracteres; i++){
		if(str[i] == sep || str[i] == '\0'){
			corte[j] = i + 1;
			cant_corte[j - 1] = corte[j] - corte[j - 1];
			j++;
		}
	}

	char** strv = malloc(sizeof(char*) * cant);
		if (strv == NULL)return NULL;
	for(i=0; i < cant-1; i++){
		strv[i] = malloc(sizeof(char) * cant_corte[i]);
		if (strv[i] == NULL)return NULL;
		memcpy(strv[i], str + corte[i], cant_corte[i]);
		cant_corte[i] --;
		strv[i][cant_corte[i]] = '\0';
	}
	strv[cant - 1] = NULL;
	return strv;
}

void free_strv(char* strv[]){
	int i = 0;
	while(strv[i] != NULL){
		free(strv[i]);
		i++;
	}
	free(strv[i]);
	free(strv);
}