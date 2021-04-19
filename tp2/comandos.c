#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 200809L //getline
#include "strutil.h"
#include "CONSTANTES.h"
#include "hash.h"
#include "abb.h"
#include "heap.h"
#include "pila.h"
#include "lista.h"
#include "comandos.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#define TIME_FORMAT "%FT%T%z"
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>



//STRUCTS QUE UTILIZA LA PRIMITIVA//

struct info_log{
	char* IP;
	//size_t fecha_hora[CANTIDAD_FECHA_HORA];
	time_t fecha_hora;
	char* metodo;
	char* URL;
};

typedef struct info_log info_log_t;//esta linea deveria estar en el tp2.h

typedef struct elemento{
	void* dato;
	char** clave;
}elemento_t;

typedef struct dif_tiempo{//PARA LOS ATAQUES DOS
	lista_t* tiempos;
	double diferencia;
	size_t contador_de_visitas;
	bool avisado;
	bool reportado;
	char* ip;
} diferencia_t;

typedef struct orden_externo{//lo que se encola en el heap de minimos del ordenamiento externo
	char* linea;
	time_t tiempo;
	FILE* archivo;
	bool final;
	char* IP;
//	char* recurso;
} oe_t;

//DEF
time_t iso8601_to_time(const char* iso8601);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
int asprintf(char **strp, const char *fmt, ...);
int vasprintf(char **strp, const char *fmt, va_list ap);

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

int comparar_ip(const char* ip1, const char* ip2){// PARA COMPARAR IPS PARA EL ABB
	char** arrip1 = split(ip1, '.');
	char** arrip2 = split(ip2, '.');
	int estado = comparar_ip_rec(arrip1, arrip2);
	free_strv(arrip1);
	free_strv(arrip2);
	return estado;
}

int comparar_ip_heap(const void* ip01, const void* ip02){
	const char* ip1 = (const char*)ip01;
	const char* ip2 = (const char*)ip02;
	return comparar_ip(ip2,ip1);//heap de minimos
}

//si t1 > t2 => 1 si t1 < t2 => -1 si t1 = t2 => 0
int comparar_tiempo(time_t t1, time_t t2){
	if (t1 != t2){
		if (t1 - t2 > 0){
			return 1;
		}
		return -1;
	}
	return 0;
}

//comparacion invertida de elementos
int inverso_elem (const void* a, const void* b){
	const elemento_t* dato_a = a;
	const elemento_t* dato_b = b;
	char** primero = dato_a->clave;//[ip,fecha,metodo,url,null]
	char** segundo = dato_b->clave;//[ip,fecha,metodo,url,null]
	time_t tiempo1 = iso8601_to_time(primero[1]);
	time_t tiempo2 = iso8601_to_time(segundo[1]);
	int comparacion_tiempo = comparar_tiempo(tiempo2,tiempo1);//invierto el orden para que el inverso
	if (comparacion_tiempo != 0){return comparacion_tiempo;}
	int comparacion_IP = comparar_ip(segundo[0],primero[0]);//invierto el orden para que el inverso
	if (comparacion_IP != 0){return comparacion_IP;}
	int comparacion_recurso = strcmp(segundo[2],primero[2]);
	if (comparacion_recurso != 0){return comparacion_recurso;}
	return strcmp(segundo[3],primero[3]);
}

int comparar_orden_externo(oe_t* orden1, oe_t* orden2){
	return comparar_tiempo(orden1->tiempo,orden2->tiempo);
}

int comparar_orden_invertido_externo(const void* orden1, const void* orden2){
	oe_t* ord1 = (oe_t*)orden1;
	oe_t* ord2 = (oe_t*)orden2;
	return comparar_orden_externo(ord2,ord1);//invertido
}

int ordenamiento_de_archivo(const void* orden1, const void* orden2){
	int comparacion_tiempo = comparar_orden_invertido_externo(orden1, orden2);
	if (comparacion_tiempo != 0){
		return comparacion_tiempo;
	}
	oe_t* ord1 = (oe_t*)orden1;
	oe_t* ord2 = (oe_t*)orden2;
	int comparacion_IP = comparar_ip(ord2->IP,ord1->IP);
	if (comparacion_IP != 0){
		return comparacion_IP;
	}
	return strcmp(ord2->linea,ord1->linea);
}

//FUNCIONES AUXILIARES//

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

char* rellenar_ip(const char* ip, bool es_inicial){
//no toma en cuenta casos particulares de separadores aislados
	char* ip_rellena;
	char separador = '.';
	char** IPs = split(ip,separador);
	if (IPs == NULL){
		return NULL;
	}
	size_t i = CANT_IP;
	while (IPs[CANT_IP - i] != NULL){
		if (i == 0){//caso borde de superar el tamaño de IP
			char* IPs_c[CANT_IP + 1];
			for (size_t p = 0; p < CANT_IP; p++){
				IPs_c[p] = IPs[p];
			}
			IPs_c[CANT_IP] = NULL;
			ip_rellena = join(IPs_c,separador);
			free_strv(IPs);
			return ip_rellena;
		}
		i--;
	}
	if (i == 0){
		ip_rellena = join(IPs,separador);
		free_strv(IPs);
		return ip_rellena;
	}
	char* relleno = "000";
	if (!es_inicial){
		relleno = "255";
	}
	char** ip_a_juntar = malloc(sizeof(char*) * CANT_IP);
	if (ip_a_juntar == NULL){return NULL;}
	for (size_t j = 0; j < CANT_IP; j++){
		if (j < i){
			ip_a_juntar[j] = IPs[j];
		}else{
			ip_a_juntar[j] = relleno;
		}
	}
	ip_rellena = join(ip_a_juntar,separador);
	free(ip_a_juntar);
	free_strv(IPs);
	return ip_rellena;
}

void agregar_al_arbol(abb_t* abb, info_log_t* log, pila_t* rechazados){//AGREGA LAS IPS A UN ABB
	if (abb_pertenece(abb,log->IP)){
		pila_apilar(rechazados,log);
		return;
	}
	char* clave = log->IP;
	abb_guardar(abb, clave, log);
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
	nuevo_log->metodo = malloc(sizeof(char) * strlen(metodo) + 1);
	strcpy(nuevo_log->metodo, metodo);
	nuevo_log->URL = malloc(sizeof(char) * strlen(URL) + 1);
	strcpy(nuevo_log->URL, URL);
	return nuevo_log;
}

void destruir_info_log(info_log_t* log){
	free(log->IP);
	free(log->URL);
	free(log->metodo);
	free(log);
	return;
}
void destruir_orden_externo(oe_t* orden){
	free(orden->linea);
	free(orden->IP);
	fclose(orden->archivo);
	free(orden);
	return;
}

void destruir_diferencia(diferencia_t* diferencia,void (*destruir_dato)(void *)){
	lista_destruir(diferencia->tiempos,destruir_dato);
	free(diferencia);
	return;
}

void destruir_elem(elemento_t* elem){
	free_strv(elem->clave);
	free(elem);
}

void wrapper_log(void* dato){
	if (dato == NULL){return;}
	destruir_info_log(dato);
	return;
}

void wrapper_orden_externo(void* dato){
	destruir_orden_externo(dato);
	return;
}

void wrapper_dif(void* dato){
	destruir_diferencia(dato,NULL/*free*/);
	return;
}

void wrapper_elem(void* dato){
	destruir_elem((elemento_t*)dato);
	return;
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

/*ESTA FUNCION LO QUE HACE ES IR CALCULANDO LA SUMA DE LAS DIFERENCIAS DE VISITAS SIEMPRE Y CUANDO SEAN MENORES A 2 SEG, 
SI LAS ULTIMAS 5 VISITAS SUMAN MENOS DE 2 SEGUNDOS SE TIRA ERROR, SI NO LO SUMAN, SE VUELVE A EMPEZAR EL CONTADOR*/
void dos_attack(hash_t* hash, info_log_t* log){//AGREGA POSIBLES ATAQUES AL SERVIDOR
	void* punter_a_tiempo;// = malloc(sizeof(void*));
	punter_a_tiempo = &log->fecha_hora;
	if(hash_pertenece(hash, log->IP) == true){
		diferencia_t* tiempos = hash_obtener(hash, log->IP);
		if (tiempos->avisado){
			return;
		}
		if (tiempos->contador_de_visitas == (CANT_T_DOS - 1)){
			time_t* primero = lista_borrar_primero(tiempos->tiempos);
			double dif = difftime(log->fecha_hora, *primero);
			if(dif < T_DOS && tiempos->avisado == false){
				tiempos->avisado = true;
			}
			tiempos->diferencia = dif;
			lista_insertar_ultimo(tiempos->tiempos,punter_a_tiempo);
		}
		else{
			lista_insertar_ultimo(tiempos->tiempos,punter_a_tiempo);
			tiempos->contador_de_visitas = lista_largo(tiempos->tiempos);
		}
		return;
	}
	lista_t* l_tiempos = lista_crear();
	if(l_tiempos == NULL){
		return;
	}
	diferencia_t* nueva_dif = malloc(sizeof(diferencia_t));
	if(nueva_dif == NULL){return;}
	lista_insertar_ultimo(l_tiempos,punter_a_tiempo);
	nueva_dif->diferencia = 0.0;
	nueva_dif->tiempos = l_tiempos;
	nueva_dif->contador_de_visitas = 1;
	nueva_dif->avisado = false;
	nueva_dif->reportado = false;
	nueva_dif->ip = log->IP;
	hash_guardar(hash, log->IP, nueva_dif);
	return;
}

bool visitar(const char* clave, void* dato, void* extra){
	printf("	%s\n",clave);
	return true;
}

FILE** particion(size_t N){
	FILE** resultado = malloc(sizeof(FILE*) * N);
	if (resultado == NULL){return NULL;}
	for (size_t i = 0; i < N; i++){
		char* nombre;
		asprintf(&nombre, "sub_%ld.txt", i);
		FILE* archivo = fopen(nombre,"r");
		if (archivo == NULL){//caso de error
			for (size_t j = 0; j < i; j++){
				fclose(resultado[j]);
			}//en caso de error se cierran todos los archivos a ordenar
			free(resultado);
			return NULL;
		}
		resultado[i] = archivo;
		free(nombre);
	}
	return resultado;
}

bool ordenamiento_interno(char** arreglo, size_t tam, char* nombre){
	heap_t* heap = heap_crear(inverso_elem);//ordenamiento_de_archivo
	if (heap == NULL){return NULL;}
	for (size_t i = 0; i < tam; i++){
		if (arreglo[i] == NULL){
			break;
		}
		elemento_t* elemento = malloc(sizeof(elemento_t));
		if (elemento == NULL){//caso de falla
			heap_destruir(heap,wrapper_elem);
			return false;
		}
		elemento->dato = arreglo[i];
		char** valores = split(arreglo[i],'\t');//[ip,fecha,metodo,url,null]
		elemento->clave = valores;
		heap_encolar(heap,elemento);
	}
	FILE* archivo = fopen(nombre,"w");
	if (archivo == NULL){
		heap_destruir(heap,wrapper_elem);
		return false;
	}
	while (!heap_esta_vacio(heap)){
		elemento_t* elem = heap_desencolar(heap);
		char* linea = (char*)elem->dato;
		fprintf(archivo,"%s\n",linea);
		destruir_elem(elem);
	}
	heap_destruir(heap,wrapper_elem);//verificar
	fclose(archivo);
	return true;
}

bool ordenamiento_externo(FILE** particiones, size_t N, FILE* archivo_ordenado){
	heap_t* heap = heap_crear(ordenamiento_de_archivo);
	if (heap == NULL){return NULL;}
	for (size_t i = 0; i < N; i++){
		oe_t* orden = malloc(sizeof(oe_t));
		if (orden == NULL){
			heap_destruir(heap,wrapper_orden_externo);
			return false;
		}
		orden->archivo = particiones[i];
		char* linea = obtener_linea(particiones[i]);//se supone que no daria NULL ya que inicia un archivo con al menos una linea
		orden->final = false;
		orden->linea = linea;
		char** valores = split(linea,'\t');//[ip,fecha,metodo,url,null]
		time_t tiempo = iso8601_to_time(valores[1]);
		orden->tiempo = tiempo;
		orden->IP = malloc(sizeof(char) * strlen(valores[0]) + 1);
		strcpy(orden->IP, valores[0]);
		free_strv(valores);
		heap_encolar(heap,orden);
	}
	pila_t* basurero_de_orden = pila_crear();
	while (!heap_esta_vacio(heap)){
		oe_t* orden = heap_desencolar(heap);
		fprintf(archivo_ordenado,"%s\n",orden->linea);
		char* linea_vieja = orden->linea;
		char* nueva_linea = obtener_linea(orden->archivo);
		if (nueva_linea == NULL){
			orden->final = true;
			pila_apilar(basurero_de_orden,orden);
			continue;
		}
		orden->linea = nueva_linea;
		free(linea_vieja);
		char** valores = split(nueva_linea,'\t');//[ip,fecha,metodo,url,null]
		time_t tiempo = iso8601_to_time(valores[1]);
		orden->tiempo = tiempo;
		free(orden->IP);
		orden->IP = malloc(sizeof(char) * strlen(valores[0]) + 1);
		strcpy(orden->IP, valores[0]);
		free_strv(valores);
		heap_encolar(heap,orden);
	}
	while (!pila_esta_vacia(basurero_de_orden)){
		wrapper_orden_externo(pila_desapilar(basurero_de_orden));
	}
	pila_destruir(basurero_de_orden);
	heap_destruir(heap,wrapper_orden_externo);
	return true;
}


//verifica que el archivo a ordenar esta ordenado en caso de no estarlo de false
bool verificacion_de_orden(char* nombre){
	bool ordenado = true;
	FILE* archivo = fopen(nombre,"r");
	if (archivo == NULL){
		return false;
	}
	char* linea = obtener_linea(archivo);
	char** valores;
	valores = split(linea,'\t');//[ip,fecha,metodo,url,null]
	time_t tiempo_anterior = iso8601_to_time(valores[1]);
	free_strv(valores);
	while (linea != NULL){
		free(linea);
		linea = obtener_linea(archivo);
		if (linea == NULL){
			continue;
		}
		valores = split(linea,'\t');//[ip,fecha,metodo,url,null]
		time_t tiempo = iso8601_to_time(valores[1]);
		free_strv(valores);
		if (comparar_tiempo(tiempo_anterior,tiempo) > 0){
			ordenado = false;
			break;
		}
		tiempo_anterior = tiempo;
	}
	free(linea);
	fclose(archivo);
	return ordenado;
}

bool copiar_archivo(char* archivo_entrante,char* archivo_saliente){
	if (strcmp(archivo_entrante,archivo_saliente) == 0){
		return true;
	}
	FILE* origen = fopen(archivo_entrante,"r");
	if (origen == NULL){
		return false;
	}
	FILE* destino = fopen(archivo_saliente,"w");
	if (destino == NULL){
		fclose(origen);
		return false;
	}
	char* linea = obtener_linea(origen);
	while (linea != NULL){
		fprintf(destino,"%s\n",linea);
		free(linea);
		linea = obtener_linea(origen);
	}
	free(linea);
	fclose(origen);
	fclose(destino);
	return true;
}

size_t calcular_tam_de_arreglo(size_t byte, size_t aprox){
	size_t kilob = byte * 1000;
	return (kilob / aprox);
}

//AGREGAR ARCHIVO//

bool agregar_archivo(char* nombre_archivo, abb_t* abb){
	hash_t* logs = hash_crear(wrapper_dif);
	if (logs == NULL){return false;}
	FILE* archivo_de_logs = fopen(nombre_archivo , "r");
	if (archivo_de_logs == NULL){
		hash_destruir(logs);
		return false;//caso de que el archivo no exista
	}
	pila_t* rechazados = pila_crear();
	char* linea = NULL; size_t capacidad = 0; ssize_t leidos;	
	while((leidos = getline(&linea,&capacidad,archivo_de_logs)) > 0){
		linea[strlen(linea)-1] = '\0';
		char** info_logs = split(linea,'\t');//[ip,fecha,metodo,url,null]
		info_log_t* log = crear_info_log(info_logs[0], info_logs[1], info_logs[2], info_logs[3]);
		free_strv(info_logs);
	//	pila_apilar(basurero_de_logs,log);
		dos_attack(logs, log);
		agregar_al_arbol(abb, log, rechazados);
	}
	/*imprecion de dos*/
	heap_t* DOS = heap_crear(comparar_ip_heap);
	hash_iter_t* iter = hash_iter_crear(logs);
	while (!hash_iter_al_final(iter)){
		const char* clave = hash_iter_ver_actual(iter);
		diferencia_t* dif = hash_obtener(logs,clave);
		if (dif->avisado && !dif->reportado){
			heap_encolar(DOS, dif->ip);
			dif->reportado = true;
		}
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	while (!heap_esta_vacio(DOS)){
		fprintf(stdout, "DoS: %s\n", (char*)heap_desencolar(DOS));
	}
	heap_destruir(DOS,NULL);//fin dos, verificar NULL
	free(linea);
	fclose(archivo_de_logs);
	hash_destruir(logs);
	vaciar_y_destruir_pila(rechazados,wrapper_log);
	return true;
}

//ORDENAR ARCHIVO//
bool ordenar_archivo(char* archivo_entrante, char* archivo_saliente, size_t numero){
	if (verificacion_de_orden(archivo_entrante)){
		return copiar_archivo(archivo_entrante,archivo_saliente);
	}
	FILE* archivo_desordenado = fopen(archivo_entrante , "r");
	if (archivo_desordenado == NULL){
		return false;//caso de que el archivo no exista
	}
	FILE** particiones;
	bool final = false;
	size_t N_arch = 0;
	while (!final){
		bool k_cero = false;
		char* linea;
		size_t tam_arr = calcular_tam_de_arreglo(numero, TAM_APROX_LINEA);
		char** arreglo = malloc(sizeof(char*) * tam_arr);
		for (size_t k = 0; k < tam_arr; k++){
			linea = obtener_linea(archivo_desordenado);
			if (linea == NULL){
				final = true;
				if (k == 0){
					free(arreglo);
					k_cero = true;
					break;
				}
				tam_arr = k + 1;
			}
			arreglo[k] = linea;
		}
		if (k_cero){break;}
		char* nombre;
		asprintf(&nombre, "sub_%ld.txt", N_arch);
		if(!ordenamiento_interno(arreglo,tam_arr,nombre)){
			for (size_t m = 0; m < tam_arr; m++){
				free(arreglo[m]);
			}
			free(nombre);
			free(arreglo);
			fclose(archivo_desordenado);
			return false;
		}
		free(nombre);
		for (size_t m = 0; m < tam_arr; m++){
			free(arreglo[m]);
		}
		free(arreglo);
		N_arch++;
	}
	fclose(archivo_desordenado);//ya no nesesito el archivo desordenado todos sus datos estan en archivos auxiliares
	FILE* archivo_ordenado = fopen(archivo_saliente , "w");
	if (archivo_ordenado == NULL){
		return false;//caso de que el archivo no exista
	}
	particiones = particion(N_arch);//abro y obtengo los punteros a los archivos auxiliares
	if (particiones == NULL){
		fclose(archivo_ordenado);
		return false;
	}
	ordenamiento_externo(particiones,N_arch,archivo_ordenado);
	fclose(archivo_ordenado);
	free(particiones);
	for (size_t m = 0; m < N_arch; m++){
		char* nombre;
		asprintf(&nombre, "sub_%ld.txt", m);
		remove(nombre);
		free(nombre);
	}
	return true;
}

//VER RANGOS DE IPS//

bool ver_ips(abb_t*abb, char* ip_inicial, char* ip_final){
	char* ip1_completa = rellenar_ip(ip_inicial,true);
	if (ip1_completa == NULL){
		return false;
	}
	char* ip2_completa = rellenar_ip(ip_final,false);
	if (ip2_completa == NULL){
		free(ip1_completa);
		return false;
	}
	printf("Visitantes:\n");
	abb_in_order_desde_hasta(abb, visitar, NULL, (const char*)ip1_completa, (const char*)ip2_completa);
	free(ip1_completa);
	free(ip2_completa);
	return true;
}
