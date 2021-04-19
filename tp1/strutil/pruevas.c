#include"strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main(){
//	char* cadena = "";
//	printf("original %s\n",cadena);
//	char** cad = split(cadena,',');
//	printf("original %s, cad1 %s\n",cadena,cad[0]);
//	char* al = join(cad, ',');
//	printf("original %s, cad1 %s, final %s\n",cadena,cad[0],al);
//	free_strv(cad);
//	free(al);
	char *strv[] = {NULL};
	char* al = join(strv, ',');
	free(al);
	return 0;
}
