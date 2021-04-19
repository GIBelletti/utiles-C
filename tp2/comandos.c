#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 200809L //getline
#include "strutil.h"
#include "CONSTANTES.h"
#include "hash.h"
#include "abb.h"
#include "heap.h"
#include "pila.h"
#include "comandos.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#define TIME_FORMAT "%FT%T%z"


//STRUCTS QUE UTILIZA LA PRIMITIVA//

struct info_log{
	char* IP;
	//size_t fecha_hora[CANTIDAD_FECHA_HORA];
	time_t fecha_hora;
	char* metodo;
	char* URL;
};

typedef struct info_log info_log_t;//esta linea deveria estar en el tp2.h

typedef struct elemento{// PARA LAS PAGINAS MAS VISITADAS
	void* dato;
	char* clave;
}elemento_t;

typedef struct dif_tiempo{//PARA LOS ATAQUES DOS
	time_t tiempo_anterior;
	time_t tiempo_nuevo;
	double diferencia;
	size_t contador_de_visitas;
	bool avisado;
	char* ip;
} diferencia_t;

typedef struct limite{
	char* inicial;
	char* final;
} limit_t;

//FUNCIONES DE COMPARAR//

int comparar_ip_recursivo(char** ip1, char** ip2, size_t iteracion){
	if(ip1[iteracion] == NULL || ip2[iteracion] == NULL || iteracion >= CANT_IP){
		return 0;
	}
	if(strtoul(ip1[iteracion], NULL, 0) > strtoul(ip2[iteracion], NULL, 0))
		return 1;
	if(strtoul(ip1[iteracion], NULL, 0) < strtoul(ip2[iteracion], NULL, 0))
		return -1;
	return comparar_ip_recursivo(ip1, ip2, iteracion +1);
}

int comparar_ip_rec(char** ip1, char** ip2){
	return comparar_ip_recursivo(ip1, ip2, 0);
}

char* rellenar_ip(const char* ip, bool es_inicial){
	char* ip_rellenada = malloc(sizeof(char) * (CANT_CHAR_IP));
	char* relleno = "000";
	size_t a_rellenar = 2;//0 + sep
	if (!es_inicial){
		relleno = "255";
		a_rellenar = 4;//2 5 5 + sep
	}
	char separador = '.';
	size_t cant_sep = 0;
	size_t i = 0;
	while (ip[i] != '\0'){
		if (i >= CANT_CHAR_IP || cant_sep >= 4){return ip_rellenada;}
		if (ip[i] == separador){cant_sep++;}
		ip_rellenada[i] = ip[i];
		i++;
	}
	while (cant_sep != 3){
		for (size_t j = 0; j < a_rellenar; j++){
			if (j == 0){
				ip_rellenada[i] = separador;
				cant_sep++;
			}
			else {
				ip_rellenada[i] = relleno[j - 1];
			}
			i++;
		}
	}
	return ip_rellenada;
}

int comparar_ip(const char* ip1, const char* ip2){// PARA COMPARAR IPS PARA EL ABB
	char** arrip1 = split(ip1, '.');
	char** arrip2 = split(ip2, '.');
	int estado = comparar_ip_rec(arrip1, arrip2);
	free_strv(arrip1);
	free_strv(arrip2);
	return estado;
}

//FUNCIONES AUXILIARES//

void heapear_paginas(hash_t* hash, heap_t* heap){//RECORRE TODO EL HASH AGREGANDOLO AL HEAP DE VISITADOS
	hash_iter_t* iter = hash_iter_crear(hash);
	while(hash_iter_al_final(iter) == false){
		elemento_t* elem = malloc(sizeof(elemento_t));
		if(elem == NULL)
			return;
		elem->dato = (char*) hash_iter_ver_actual(iter);
		elem->clave = hash_obtener(hash, elem->dato);
		heap_encolar(heap, elem);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	return;
}

void hashear_paginas(info_log_t* log, hash_t* hash, hash_t* logs){//AGREGA LOS LOGS A UN HASH
	if(hash_pertenece(hash, log->URL)){
		/* caso de que sea dos no se cuenta */
		if (hash_pertenece(logs, log->URL)){
			diferencia_t* tiempos = hash_obtener(hash, log->IP);
			if (tiempos->avisado == true){
				return;
			}
		}
		size_t cont = *(size_t*)hash_obtener(hash, log->URL);
		cont ++;
		*(size_t*)hash_obtener(hash, log->URL)= cont;
		return;
	}
	size_t* cont = malloc(sizeof(size_t));
	if(cont == NULL){
		return;
	}
	*cont = 0;
	hash_guardar(hash, log->URL, cont);
	return;
}

void sort_ips(abb_t* abb, info_log_t* log){//AGREGA LAS IPS A UN ABB
	//printf("%lu \n",strlen(log->IP));//debug
	if (abb_pertenece(abb,log->IP)){return;}
	char* clave = log->IP;
	abb_guardar(abb, clave, NULL);
	return;
}

time_t iso8601_to_time(const char* iso8601)
{
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

info_log_t* crear_info_log(char* IP, char* fecha, char* metodo, char* URL){
	info_log_t* nuevo_log = malloc(sizeof(info_log_t));
	if (nuevo_log == NULL/* || !verificacion_de_tam || !verificacion_de_tam*/){
		return NULL;
	}
	nuevo_log->IP = malloc(sizeof(char) * strlen(IP) + 1);
	strcpy(nuevo_log->IP, IP) ;//la IP y fecha_hora usan malloc
	nuevo_log->fecha_hora = iso8601_to_time(fecha);//el ip y la fecha deveran ser verificados en otra funcion
	nuevo_log->metodo = metodo;
	nuevo_log->URL = malloc(sizeof(char) * strlen(URL) + 1);
	strcpy(nuevo_log->URL, URL);
	return nuevo_log;
}

void destruir_info_log(info_log_t* log){
	free(log->IP);
	free(log->URL);
	free(log);
	return;
}

void wrapper_log(void* dato){
	destruir_info_log(dato);
}

/*ESTA FUNCION LO QUE HACE ES IR CALCULANDO LA SUMA DE LAS DIFERENCIAS DE VISITAS SIEMPRE Y CUANDO SEAN MENORES A 2 SEG, 
SI LAS ULTIMAS 5 VISITAS SUMAN MENOS DE 2 SEGUNDOS SE TIRA ERROR, SI NO LO SUMAN, SE VUELVE A EMPEZAR EL CONTADOR*/
void dos_attack(hash_t* hash, info_log_t* log){//AGREGA POSIBLES ATAQUES AL SERVIDOR
	if(hash_pertenece(hash, log->IP) == true){
		diferencia_t* tiempos = hash_obtener(hash, log->IP);
		double dif = difftime(log->fecha_hora, tiempos->tiempo_nuevo);
		if((dif + tiempos->diferencia) > 2.0){
			tiempos->diferencia = 0.0;
			tiempos->tiempo_anterior = tiempos->tiempo_nuevo;
			tiempos->tiempo_nuevo = log->fecha_hora;
			tiempos->contador_de_visitas = 1;
		}
		else{
			tiempos->diferencia += dif;
			tiempos->tiempo_anterior = tiempos->tiempo_nuevo;
			tiempos->tiempo_nuevo = log->fecha_hora;
			tiempos->contador_de_visitas ++;
		}
		if(tiempos->contador_de_visitas == 5 && tiempos->avisado == false){
		//	fprintf(stderr, "DoS: %s\n", log->IP);
			tiempos->avisado = true;
		}
		return;
	}
	char* nulo = "0000-00-00T00:00:00+00:00";
	time_t tiempo_nulo = iso8601_to_time(nulo);
	diferencia_t* nueva_dif = malloc(sizeof(diferencia_t));
	if(nueva_dif == NULL)
		return;
	nueva_dif->diferencia = 0.0;
	nueva_dif->tiempo_anterior = tiempo_nulo;
	nueva_dif->tiempo_nuevo = tiempo_nulo;
	nueva_dif->contador_de_visitas = 0;
	nueva_dif->avisado = false;
	nueva_dif->ip = log->IP;
	hash_guardar(hash, log->IP, nueva_dif);
	return;
}

bool visitar(const char* clave, void* dato, void* extra){
	if(comparar_ip((char*) clave, ((limit_t*)extra)->inicial) >= 0 && comparar_ip((char*)clave, ((limit_t*)extra)->final) <= 0){
		printf("	%s\n",clave);
		return true;
	}
	return false;
}

//PRIMITIVA AGREGAR ARCHIVO//

bool agregar_archivo(char* nombre_archivo, hash_t* logs, hash_t* hash_visitados , heap_t* heap_visitados, abb_t* abb, pila_t* basurero_de_split, pila_t* basurero_de_logs){
	FILE* archivo_de_logs = fopen(nombre_archivo , "r");
	if (archivo_de_logs == NULL){
		return false;//caso de que el archivo no exista
	}
	char* linea = NULL; size_t capacidad = 0; ssize_t leidos;	
    while((leidos = getline(&linea,&capacidad,archivo_de_logs)) > 0){
    	linea[strlen(linea)-1] = '\0';
    	char** info_logs = split(linea,'\t');//[ip,fecha,metodo,url,null]
	pila_apilar(basurero_de_split,info_logs);
        info_log_t* log = crear_info_log(info_logs[0], info_logs[1], info_logs[2], info_logs[3]);
	pila_apilar(basurero_de_logs,log);
    	dos_attack(logs, log);
    	hashear_paginas(log, hash_visitados, logs);
    	sort_ips(abb, log);
    }
    heapear_paginas(hash_visitados, heap_visitados);
	/*imprecion de dos*/
/*	heap_t* DOS = heap_crear(comparar_ip);
	hash_iter_t* iter = hash_iter_crear();
	while (!hash_iter_esta_al_final(iter)){
		diferencia_t* dif = hash_iter_ver_actual(iter);
		if (dif->avisado){
			heap_encolar(heap, dif->ip);
		}
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	while (!heap_esta_vacio(DOS)){
		fprintf(stderr, "DoS: %s\n", (char*)heap_desencolar(DOS));
	}
	heap_destruir(DOS);*/
    free(linea);
    fclose(archivo_de_logs);
    return true;
}

//PRIMITIVA DE VER PAGINAS VISITADAS//

bool ver_visitados(heap_t* heap_visitados, size_t cant){
	elemento_t* array[cant];
	fprintf(stdout, "Sitios más visitados:\n");
	for(size_t i = 0; i < cant; i++){
		array[i] = heap_desencolar(heap_visitados);
		fprintf(stdout, "\t%s - %lu\n", (char*)array[i]->dato, strtoul(array[i]->clave, NULL, 0));
	}
	for(size_t i = 0; i < cant; i++){
		heap_encolar(heap_visitados, array[i]);
	}
	return true;
}

//PRIMITIVA DE VER RANGOS DE IPS//

bool ver_ips(abb_t*abb, char* ip_inicial, char* ip_final){
	printf("Visitantes:\n");
	limit_t* limite = malloc(sizeof(limit_t));
	char* ip1_completa = rellenar_ip(ip_inicial,true);
	char* ip2_completa = rellenar_ip(ip_final,false);
	limite->inicial = ip1_completa;
	limite->final = ip2_completa;
	abb_t* arbol_recortado = abb_in_order(abb, visitar, limite);
	limite->inicial = NULL;
	limite->final = NULL;
	free(ip1_completa);
	free(ip2_completa);
	free(limite);
	abb_destruir(arbol_recortado);
	return true;
}
