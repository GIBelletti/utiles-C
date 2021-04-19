#include <stdbool.h>
#include <stdio.h>
#include "cola.h"
#include "pila.h"
#include "lista.h"
#include "strutil.h"
#include "usuarios.h"
#include "wachencoin.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* STRUCTURA PAGOS PENDIENTES */
struct pago_pendiente{
	double monto;
	size_t id_recibe;
	size_t id_paga;
};
typedef struct pago_pendiente pago_pendiente_t;

/* PRIMITIVA */

pago_pendiente_t* crear_pago_pendiente(double monto,size_t id_recibe,size_t id_paga){
	pago_pendiente_t* nuevo = malloc(sizeof(pago_pendiente_t));
	if (nuevo == NULL){
		return NULL;
	}
	nuevo->monto = monto;
	nuevo->id_recibe = id_recibe;
	nuevo->id_paga = id_paga;
	return nuevo;
}
void destruir_pago_pendiente(pago_pendiente_t* pago){
	free(pago);
	return;
}
/* wrappers */

void wrapper_pagos(void* dato){
	destruir_pago(dato);
}

void wrapper_pagos_pendiente(void* dato){
	destruir_pago_pendiente(dato);
}

void wrapper_usuario(void* dato){
	eliminar_usuario(dato);
}

void wrapper_simple(void* dato){
	free(dato);
}

void wrapper_split(void* dato){
	free_strv(dato);
}

void wrapper_pila_con_elementos(void* dato){
	while (!pila_esta_vacia(dato)){
		free(pila_desapilar(dato));
	}
	//vaciar_pila(dato,wrapper_simple);
	pila_destruir(dato);
}

/* recibe una pila y la borra junto a su contenido */
void vaciar_pila(pila_t* pila,void (*destruir_dato)(void*)){
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

/* CONSTANTES */
const char* FINALIZAR = "finalizar";
const char* AYUDA = "Los comandos son:\n agregar_pago <id> <monto> <contraseña>: la id y la contraseña pertenecen al usuario el usuario.\n pagos_pendientes: muestra la cantidad y monto total de los pagos sin procesar.\n procesar <numero>: procesa hasta un número NO negativo de pagos pendientes.\n guardar_cuentas <archivo>: guarda en un archivo el estado actual de las cuentas. Si no existe lo debe crear, y si existe lo sobreescribe.\n finalizar: finaliza el programa (si quedan pagos sin procesar, no se procesan).\n Recuerde que los comandos se escribiran en una sola línea, separando las distintas instrucciones con el caracter punto y coma (;). El monto es siempre positivo.";
const int MIN_COMANDO = 250;
const size_t CANT_IDENTIFICADORES = 8;
const char* IDENTIFICADORES[] = {"pagos_pendientes","procesar","agregar_pago","guardar_cuentas","validar_usuario","pagar","validar_pago",NULL};
const size_t CANT_ELEM_IDEMTIFICADORES[] = {0,1,2,1,2,3,2,0};
const size_t MAX_INFO = 1000;
const size_t CANTIDAD_INFO_USUARIO = 3;
const char* ERROR = "Error en";
const char* INFO_USUARIO = "lectura de datos de usuario.";
const char* INTRODUCCION_DE_COMANDO = "Ingrese el comando: ";
const char SEPARADOR_INFO_USUARIO = ',';
const char SEPARADOR_OPERACIONES = ';';
const char* OPERACION_ERRONEA = "reconocimiento de operacion (escritura de operacion erronea).";
const char* EXISTENCIA_COLA = "creacion de cola";
const char* EXISTENCIA_LISTA = "creacion de lista";
const char* EXISTENCIA_ARCHIVO = "archivo, no se pudo abrir correctamente el archivo.";
const char* EXISTENCIA_PILA = "creacion de pila";
const char* OPERACION_PARAMETROS = "parametros, cantidad de parametros erroneas.";
const char* PARAMETRO_ERRONEO = "parametro, tipo de parametro erroneo.";
const char SEPARADOR_DE_PAGOS = ' ';
const char* CANTIDAD_DE_PROCESOS = "cantidad de procesos, la cantidad de pagos a procesar supera la cantidad de pagos (esto no termina la ejecucion).";
const char* ID_USUARIO = "id de usuario, el usuario no existe.";
const char* SALDO_INSUFICIENTE = "cantidad de saldo insuficiente (el usuario no tiene fondos para realizar la transferencia) o no procesable (no se ha creado un pago de este monto anteriormente).";
const char* FALLA_DE_DATOS = "creacion de datos a guardar";
const char* USUARIO_COORDENADA_C = "coordenada de usuario, la coordenada no es valida para este usuario.";
const char* PAGO_INVALIDO = "pago, el pago es invalido por no haberse creado anteriormente";
const char* FUNCION_INVALIDA = "comando, comando inexistente.";

/* recibe dos cadenas y las fuciona en una */
bool unir_cadenas(char destino[], char origen[], int largo)
{
	size_t largo_origen = strlen(origen);
	size_t largo_destino = strlen(destino);
	if ( largo_origen+largo_destino+1 > largo ) {
		return false;
	}
	for (size_t i=0; i<largo_origen;i++) {
		destino[largo_destino+i] = origen[i];
	}
	destino[largo_destino+largo_origen]='\0';
	return true;
}

/* compara dos cadenas (copia modificada de strcmp) */
int strcmpc(const char* a ,const char* b){
	if (a == NULL){return -1;}
	if (b == NULL){return 1;}
	size_t i = 0;
	while (a[i] != '\0' || b[i] != '\0') {
		if (  a[i] != b[i] ){
			return (a[i] - b[i]);
		}
		i++;
	}
	if ( a[i]  == '\0' && b[i] == '\0' ){
		return 0;
	}
	return (a[i] - b[i]);
}

/* recibe una cadena devuelve true si es un numero, si es NULL da false */
bool es_un_numero(char* cadena){
	if (cadena == NULL){
		return false;
	}
	size_t i = 0;
	size_t punto = 0;
	while(cadena[i] != '\0'){
		if (cadena[i] == '.'){
			punto++;
			i++;
			continue;
		}
		if(cadena[i] < '0' || cadena[i] > '9'){
			return false;
		}
		i++;
	}
	if (punto > 1){
		return false;
	}
	return true;
}

/* INTERFAZ */

/* FUNCIONES DE INTERFAZ */
/*agregará a la cola de procesamiento la transacción. El código se manifiesta en una sola línea, separando las distintas instrucciones con el caracter punto y coma (;). El monto es siempre positivo. (caso de ser negatvo no agregara nada a la cola de procesaeinto)*/
bool agregar_pago (pila_t* pila,lista_t* usuarios,lista_t* pagos,cola_t* nada,pila_t* basurero){//¿bool?
	char* desapilado1 = pila_desapilar(pila);
	char* desapilado2 = pila_desapilar(pila);
	pila_apilar(basurero,desapilado1);
	pila_apilar(basurero,desapilado2);
	if (!es_un_numero(desapilado1) || !es_un_numero(desapilado2)){
		fprintf(stderr,"Error en comando agregar_pago\n");
		//fprintf(stderr,"%s %s\n",ERROR,OPERACION_PARAMETROS);
		return false;
	}
	size_t id_pago = (size_t)atol((const char*)desapilado1);
	double monto = atof((const char*)desapilado2);
	lista_iter_t* iter2 = lista_iter_crear(pagos);
	while(!lista_iter_al_final(iter2)){
		pago_t* pagos_existentes = lista_iter_ver_actual(iter2);
		if (id_pago == identidad_del_pago(pagos_existentes)){
			lista_iter_destruir(iter2);
			return aumentar_pago(pagos_existentes,monto);
		}
		lista_iter_avanzar(iter2);
	}
	lista_iter_destruir(iter2);
	lista_insertar_ultimo(pagos,crear_pago(id_pago,NULL,monto));
	printf("OK\n");
	return true;
}

/* imprime (stdout) la cantidad y monto total de los pagos sin procesar. (de los "argregar_pagos" sin procesar)*/
bool pagos_pendientes(pila_t* pila,lista_t* usuario,lista_t* pagos,cola_t* cola,pila_t* basurero){
	double arreglo[2] = {0,0};
	lista_iter_t* iter = lista_iter_crear(pagos);
	while (!lista_iter_al_final(iter)){
		pago_t* pagar = lista_iter_ver_actual(iter);
		double pago = saldo_pago(pagar);
		if (pago == 0){
			lista_iter_avanzar(iter);
			continue;
		}
		arreglo[1] += pago;
		lista_iter_avanzar(iter);
		arreglo[0]++;
	}
	lista_iter_destruir(iter);
	printf("%.0f,%.2f\n",arreglo[0],arreglo[1]);
	printf("OK\n");
	return true;
}

/* procesa hasta un número no negativo de pagos pendientes. (si el numero es negativo o cero no procesa ningun pago pendiente)*/
bool procesar(pila_t* pila,lista_t* usuario,lista_t* pagos,cola_t* pendientes,pila_t* basurero){//¿bool?
	char* C_num = pila_desapilar(pila);
	pila_apilar(basurero,C_num);
	if (!es_un_numero(C_num)){
		fprintf(stderr,"Error en comando procesar\n");
		//fprintf(stderr,"%s %s",ERROR,PARAMETRO_ERRONEO);
		return false;
	}
	size_t numero = (size_t)atol((const char*)C_num);//!es_un_numero no toma negativos
	if (numero == 0){
		return true;
	}
	for (size_t i = 0; i < numero; i++){
		if (cola_esta_vacia(pendientes)){
			//fprintf(stderr,"%s %s\n",ERROR,CANTIDAD_DE_PROCESOS);
			break;
		}
		pago_pendiente_t* pago_pendiente = cola_desencolar(pendientes);
		double pago = pago_pendiente->monto;
		size_t recibe = pago_pendiente->id_recibe;
		size_t paga = pago_pendiente->id_paga;
		//printf("debug pago %f recibe %ld paga %ld",pago,recibe,paga);//debug
		destruir_pago_pendiente(pago_pendiente);
		size_t maximo = recibe > paga? recibe : paga;
		if (lista_largo(usuario) < maximo){
			fprintf(stderr,"Error en %s\n",ID_USUARIO);
			return false;
		}
		lista_iter_t* iter_pagos = lista_iter_crear(pagos);
		lista_iter_t* iter_usuario = lista_iter_crear(usuario);
		pago_t* actual = NULL;
		while (!lista_iter_al_final(iter_pagos)){
			actual = lista_iter_ver_actual(iter_pagos);
			size_t id_actual_de_pago = identidad_del_pago(actual);
			long int verificacion_de_id_de_pago = (long int)id_actual_de_pago - (long int)paga;
			if(verificacion_de_id_de_pago == 0){
				break;//rompe el while
			}
			lista_iter_avanzar(iter_pagos);
		}
		if (actual == NULL){
			fprintf(stderr,"Error en %s\n",PAGO_INVALIDO);
			lista_iter_destruir(iter_usuario);
			lista_iter_destruir(iter_pagos);
			return false;
		}
		for (size_t j = 0; j < recibe; j++){
			lista_iter_avanzar(iter_usuario);
		}
		usuario_t* usuario_recibe = lista_iter_ver_actual(iter_usuario);
		lista_iter_destruir(iter_usuario);
		lista_iter_t* iter_usuario_2 = lista_iter_crear(usuario);
		for (size_t k = 0; k < paga; k++){
			lista_iter_avanzar(iter_usuario_2);
		}
		usuario_t* usuario_paga = lista_iter_ver_actual(iter_usuario_2);
		lista_iter_destruir(iter_usuario_2);
		lista_iter_destruir(iter_pagos);
		if (pago_ver_codigo(actual) != NULL){//caso de que el pago halla sido invalidado por coordenada errornea
			//printf("OK\n");
			//return true;
			continue;
		}
		double monto_usuario = ver_saldo(usuario_paga);
		if (monto_usuario < pago){
			fprintf(stderr,"Error en %s\n",SALDO_INSUFICIENTE);
			printf("OK\n");//error tipo 1
			return false;
		}

		/*if (identidad_del_pago(actual) != paga){//caso de que el pagop no halla sido creado pero se nesesite que se pague
			if (!extraer_saldo(usuario_paga,pago)){
				fprintf(stderr,"Error en %s\n",SALDO_INSUFICIENTE);
				return false;
			}
		}*/

		if (!extraer_saldo(usuario_paga,pago) || !pagar_pagos(actual,pago)){
			fprintf(stderr,"Error en %s\n",SALDO_INSUFICIENTE);
			return false;
		}
		pagar_saldo(usuario_recibe,pago);
	}
	printf("OK\n");
	return true;
}

/* guarda en un archivo el estado actual de las cuentas. Si no existe lo debe crear, y si existe lo sobreescribe. */
bool guardar_cuentas (pila_t* pila,lista_t* usuarios,lista_t* pagos,cola_t* cola,pila_t* basurero){
	char* archivo = pila_desapilar(pila);
	pila_apilar(basurero,archivo);
	FILE* archivo_usuarios = fopen(archivo,"w");
	lista_iter_t* iter = lista_iter_crear(usuarios);
	size_t id = 0;
	while (lista_iter_ver_actual(iter) != lista_ver_ultimo(usuarios)){
		usuario_t* usuario = lista_iter_ver_actual(iter);
		fprintf(archivo_usuarios,"%zd,%.2f,%s",id,ver_saldo(usuario),usuario_coordenada(usuario));
		fprintf(archivo_usuarios,"\n");
		id++;
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	fclose(archivo_usuarios);
	printf("OK\n");
	return true;
}

/* finaliza el programa (si quedan pagos sin procesar, no se procesan).*/
void finalizar(){
	printf("OK\n");
	return;
}

/* FUNCIONES DEL PROGRAMA */
/* desapila dos elementos y revisa que las coordenadas sean apropiadas para el usuario. */
bool validar_usuario(pila_t* pila,lista_t* usuarios,lista_t* pagos,cola_t* nada,pila_t* basurero){
	char* coordenada = pila_desapilar(pila);
	char* c_id = pila_desapilar(pila);
	//pila_apilar(basurero,coordenada);
	//pila_apilar(basurero,c_id);
	/*supongo que no se pueden tener los nombres de funciones como coordenadas*/

	for (size_t y = 0; y < CANT_IDENTIFICADORES; y++){
		const char* identidad = IDENTIFICADORES[y];
		if (strcmpc(coordenada,identidad) == 0){
			fprintf(stderr,"Error en comando %s\n",(char*)coordenada);
			return false;
		}
		if (identidad == NULL){
			break;
		}
	}


	if (!es_un_numero(c_id)){
		fprintf(stderr,"Error en comando validar_usuario\n");
		//fprintf(stderr,"%s %s\n",ERROR,OPERACION_PARAMETROS);
		return false;
	}
	size_t id = (size_t)atol((const char*)c_id);
	if (lista_largo(usuarios) < id){
		fprintf(stderr,"Error en %s\n",ID_USUARIO);
		printf("OK\n");//error tipo 1
		return false;
	}
	lista_iter_t* iter = lista_iter_crear(usuarios);
	for (size_t i = 0; i < id; i++){
		lista_iter_avanzar(iter);//las id de usuarios corresponden a su posicion en la lista dado que los usuarios NO se borran ni se agregan no hay nesesidad de verificacion
	}
	usuario_t* usuario = lista_iter_ver_actual(iter);
	char* coordenada_de_usuario = usuario_coordenada(usuario);
	lista_iter_destruir(iter);
	if (strcmpc(coordenada,coordenada_de_usuario) == 0){
		return true;
	}
	//fprintf(stderr,"%s %s",ERROR,USUARIO_COORDENADA_C);
	//return false;//¿deve continuar ejecutando?
	int numero_de_pago = 1;
	lista_iter_t* iter_pagos = lista_iter_crear(pagos);
	while (!lista_iter_al_final(iter_pagos)){
		pago_t* pago = lista_iter_ver_actual(iter_pagos);
		if (identidad_del_pago(pago) == id){
			pago_codigo(pago,"N");//pago invalidado
			fprintf(stderr,"Error en pago %d\n",numero_de_pago);
			break;
		}
		lista_iter_avanzar(iter_pagos);
		numero_de_pago++;
	}
	lista_iter_destruir(iter_pagos);
	return true;
}

/* desapila dos elementos y revisa que el usuario tenga fondos suficientes para hacer la operación. */
bool validar_pago(pila_t* pila,lista_t* usuarios,lista_t* pagos,cola_t* pendientes,pila_t* basurero){
	char* char_monto = pila_desapilar(pila);
	char* char_id = pila_desapilar(pila);
	//pila_apilar(basurero,char_monto);
	//pila_apilar(basurero,char_id);
	if (!es_un_numero(char_id) || !es_un_numero(char_monto)){
		fprintf(stderr,"Error en comando validar_pago\n");
		return false;
	}
	double monto = atof((const char*)char_monto);
	size_t id = (size_t)atol((const char*)char_id);
	if (lista_largo(usuarios) < id){
		fprintf(stderr,"Error en %s\n",ID_USUARIO);
		printf("OK\n");//error tipo 1
		return false;
	}
	lista_iter_t* iter = lista_iter_crear(usuarios);
	for (size_t i = 0; i <= id; i++){
		lista_iter_avanzar(iter);
	}
	usuario_t* usuario_paga = lista_iter_ver_actual(iter);
	lista_iter_destruir(iter);
	if (ver_saldo(usuario_paga) < monto){
		/* caso de que el usuario obtenga el monto nesesario antes de ser procesado el pago */
		pago_pendiente_t* pago_anterior =cola_ver_primero(pendientes);
		if (pago_anterior->id_recibe == id){
			double cantidad_antes_de_procesar = pago_anterior->monto + monto;
			if (ver_saldo(usuario_paga) < cantidad_antes_de_procesar){return true;}
		}
		/* saldo insuficente */
		//fprintf(stderr,"%s %s ...usu %f ...monto %f\n",ERROR,SALDO_INSUFICIENTE,ver_saldo(usuario_paga),monto);
		//printf("OK\n");//error tipo 1
		//return false;
		int numero_de_pago = 1;
		lista_iter_t* iter_pagos = lista_iter_crear(pagos);
		while (!lista_iter_al_final(iter_pagos)){
			pago_t* pago = lista_iter_ver_actual(iter_pagos);
			if (identidad_del_pago(pago) == id){
				pago_codigo(pago,"N");//pago invalidado
				fprintf(stderr,"Error en pago %d\n",numero_de_pago);
				//printf("OK\n");//error tipo 1
				break;
			}
			lista_iter_avanzar(iter_pagos);
			numero_de_pago++;
		}
		lista_iter_destruir(iter_pagos);
	}
	return true;
}

/* desapila tres elementos, el movimiento de fondos lo hace el procesar. */
bool pagar(pila_t* pila,lista_t* usuarios,lista_t* pagos,cola_t* pendientes,pila_t* basurero){
	char* char_paga = pila_desapilar(pila);
	char* char_recibe = pila_desapilar(pila);
	char* char_monto = pila_desapilar(pila);
	//pila_apilar(basurero,char_paga);
	//pila_apilar(basurero,char_recibe);
	//pila_apilar(basurero,char_monto);
	//printf("debug paga %s recive %s monto %s\n",char_paga,char_recibe,char_monto);
	if (!es_un_numero(char_recibe) || !es_un_numero(char_paga) || !es_un_numero(char_monto)){
		fprintf(stderr,"Error en comando pagar\n");
		return false;
	}
	double monto = atof((const char*)char_monto);
	size_t id_recibe = (size_t)atol((const char*)char_recibe);
	size_t id_paga = (size_t)atol((const char*)char_paga);
	lista_iter_t* iter = lista_iter_crear(usuarios);
	for (size_t i = 0; i <= id_paga; i++){
		lista_iter_avanzar(iter);
	}
	usuario_t* usuario_paga = lista_iter_ver_actual(iter);
	lista_iter_destruir(iter);

	bool existe_pago_correspondiente = false;//borrar?
	bool pago_anulado = false;
	lista_iter_t* iter_pagos = lista_iter_crear(pagos);
	int numero_de_pago = 1;
	while (!lista_iter_al_final(iter_pagos)){
		pago_t* pago = lista_iter_ver_actual(iter_pagos);
		if (identidad_del_pago(pago) == id_paga){
			existe_pago_correspondiente = true;
			if (pago_ver_codigo(pago) != NULL){
				pago_anulado = true;
			}
			break;
		}
		lista_iter_avanzar(iter_pagos);
		numero_de_pago++;
	}
	lista_iter_destruir(iter_pagos);
	if (!existe_pago_correspondiente || pago_anulado){
		/*en caso de que no se halla creado un pago anteriormente*/
		//return true;//no se crea el pago???
		monto = 0;//se crea el pago con monto 0???
	}

	else if (ver_saldo(usuario_paga) < monto){
		//fprintf(stderr,"%s %s\n",ERROR,SALDO_INSUFICIENTE);
		fprintf(stderr,"Error en pago %d\n",numero_de_pago);
		printf("OK\n");//error tipo 1
		monto = 0;
		return true;
	}
	pago_pendiente_t* pago_pendiente = crear_pago_pendiente(monto,id_recibe,id_paga);
	cola_encolar(pendientes,pago_pendiente);
	return true;
}

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

/* recibe un arreglo (de cadenas) (cuyo ultimo elemento es NULL) y un numero (mayor que 0) y devuelve true si la cantidad de elementos es igual al numero, caso contrario da false */
bool verificacion_de_tam(char** arreglo, size_t tam){
	if (tam == 0){//tam nunca < 0
		return false;
	}
	size_t i = 0;
	while (arreglo[i] != NULL){
		i++;
	}
	return (i == tam);
}

/* recive un puntero generico y un mensaje si el puntero es NULL imprime en error el mensaje y devuleve false, caso de que no sea */
bool verificacion_de_existencia(void* existe, const char* mensaje){
	if (existe == NULL){
		fprintf(stderr,"%s %s\n",ERROR,mensaje);
		return false;
	}
	return true;
}

/* recive la cola (y la lista) con los procesos a realizar y ejecuta los comandos */
bool ejecutar(cola_t* procesamientos,lista_t* usuarios){
	pila_t* basurero = pila_crear();//pila donde estan todas las cadenas que usan malloc
	pila_t* auxiliar = pila_crear();//elementos posteriores
	lista_t* pagos = lista_crear();//struct pago de transacciones faltantes
	cola_t* pendientes = cola_crear();//valores de transacciones a procesar
	while (!cola_esta_vacia(procesamientos)){
		/* verificacion de comando invalido */
		const char* comando_validar = pila_ver_tope(cola_ver_primero(procesamientos));
		for (size_t y = 0; y < CANT_IDENTIFICADORES; y++){
			const char* identidad = IDENTIFICADORES[y];
			if (strcmpc(comando_validar,identidad) == 0){
				break;//sale del for
			}
			if (identidad == NULL){
				//fprintf(stderr,"%s %s '%s'\n",ERROR,FUNCION_INVALIDA,(char*)pila_ver_tope(pagos));
				fprintf(stderr,"%s comando %s\n",ERROR,(char*)comando_validar);
				lista_destruir(pagos,wrapper_pagos);
				cola_destruir(pendientes,wrapper_pagos_pendiente);
				pila_destruir(auxiliar);
				if (!pila_esta_vacia(basurero)){
					vaciar_pila(basurero,wrapper_simple);
				}
				else {pila_destruir(basurero);}
				return false;
			}
		}
		pila_t* proceso = cola_desencolar(procesamientos);
		while (!pila_esta_vacia(proceso)){
			char* comando = pila_desapilar(proceso);
			pila_apilar(basurero,comando);
			if (comando == NULL){continue;}
			size_t numero = CANT_IDENTIFICADORES;
			for (size_t i = 0; i < CANT_IDENTIFICADORES; i++){
				const char* identidad = IDENTIFICADORES[i];
				if (strcmpc(comando,identidad) == 0){
					numero = i;
					break;//sale del for
				}
				if (identidad == NULL){
					pila_apilar(auxiliar,comando);
				}
			}
			if (numero < CANT_IDENTIFICADORES){//divicion de procesos no se puede mover a una funcion separada
				/* arreglo de funciones */
				bool (*funciones[CANT_IDENTIFICADORES])(pila_t*,lista_t*,lista_t*,cola_t*,pila_t*);
				funciones[0] = pagos_pendientes;
				funciones[1] = procesar;
				funciones[2] = agregar_pago;
				funciones[3] = guardar_cuentas;
				funciones[4] = validar_usuario;
				funciones[5] = pagar;
				funciones[6] = validar_pago;
				if (numero < 4){
					if (!funciones[numero](proceso,usuarios,pagos,pendientes,basurero)){
						lista_destruir(pagos,wrapper_pagos);
						cola_destruir(pendientes,wrapper_pagos_pendiente);
						pila_destruir(auxiliar);
						pila_destruir(proceso);
						if (!pila_esta_vacia(basurero)){
							vaciar_pila(basurero,wrapper_simple);
						}
						else {pila_destruir(basurero);}
						return false;
					}
				}
				else {
					if (!funciones[numero](auxiliar,usuarios,pagos,pendientes,basurero)){
						lista_destruir(pagos,wrapper_pagos);
						cola_destruir(pendientes,wrapper_pagos_pendiente);
						pila_destruir(auxiliar);
						pila_destruir(proceso);
						if (!pila_esta_vacia(basurero)){
							vaciar_pila(basurero,wrapper_simple);
						}
						else {pila_destruir(basurero);}
						return false;
					}
				}
			}
		}
		pila_destruir(proceso);
		//printf("OK\n");//si no falla es OK
	}
	if (!pila_esta_vacia(basurero)){
		vaciar_pila(basurero,wrapper_simple);
	}
	else {pila_destruir(basurero);}
	lista_destruir(pagos,wrapper_pagos);
	cola_destruir(pendientes,wrapper_pagos_pendiente);
	pila_destruir(auxiliar);
	return true;
}

/* caso de que falle la cantidad de parametros que se deve imprimir antes del error de comando invalido pero despues del finalizar */
void error_finalizar(size_t c_anteriores,const char* comando){
	char proceso[250] = "";//MIN_COMANDO
	//size_t c_posteriores = 0;
	while (strcmpc(proceso,FINALIZAR) != 0){
		fgets(proceso,MIN_COMANDO,stdin);
		remplazar_chr13(proceso);
		//c_posteriores++;
	}
	for (size_t a = 0; a < c_anteriores; a++){
		finalizar();
	}
	fprintf(stderr,"%s comando %s\n",ERROR,comando);
	//for (size_t b = 0; b < c_posteriores; b++){
	//	finalizar();
	//}
	return;
}

/* recibe la cola y lista y ejecuta la interfaz, devuelve true si no hay problema false si hubo error */
bool interfaz(cola_t* procesamientos,lista_t* usuarios,pila_t* free_split){
	char comando[250] = "";//MIN_COMANDO
	size_t c_anteriores = 0;//verificacion de cantidad de parametros
	//printf("%s\n",AYUDA);//muestra los comandos
	while (strcmpc(comando,FINALIZAR) != 0){
		pila_t* pila_auxiliar = pila_crear();
		pila_t* pagos = pila_crear();
		if (!verificacion_de_existencia(pagos,EXISTENCIA_PILA) || !verificacion_de_existencia(pila_auxiliar,EXISTENCIA_PILA)){
			while (!cola_esta_vacia(procesamientos)){
				pila_destruir(cola_desencolar(procesamientos));
			}
			pila_destruir(pila_auxiliar);
			pila_destruir(pagos);
			return false;
		}
		//printf("%s",INTRODUCCION_DE_COMANDO);//pide ingresar comando
		fgets(comando,MIN_COMANDO,stdin);
		remplazar_chr13(comando);
		if (strcmpc(comando,FINALIZAR) == 0/* ||strcmpc(comando,"") == 0*/){
			if (!pila_esta_vacia(pagos)){
				vaciar_pila(pagos,wrapper_simple);
			}
			else {pila_destruir(pagos);}
			pila_destruir(pila_auxiliar);
			//finalizar();
			continue;
		}
		char** operaciones = split(comando,SEPARADOR_OPERACIONES);
//		if (strcmpc(operaciones[0],IDENTIFICADORES[1]) == 0 && !es_un_numero(operaciones[1])){
//			fprintf(stderr,"Error en comando procesar\n");
//			if (!pila_esta_vacia(pagos)){
//				vaciar_pila(pagos,wrapper_simple);
//			}
//			else {pila_destruir(pagos);}
//			pila_destruir(pila_auxiliar);
//			return false;
//		}

		pila_apilar(free_split,operaciones);
		size_t orden = 0;
		while (operaciones[orden] != NULL){
			char** elementos = split(operaciones[orden],SEPARADOR_DE_PAGOS);
			pila_apilar(free_split,elementos);
			/* verificacion de cantidad de parametros */
			if (orden == 0){
				bool terminar = false;
				for (size_t y = 0; y < CANT_IDENTIFICADORES; y++){
					const char* identidad = IDENTIFICADORES[y];
					if (strcmpc(elementos[0],identidad) == 0){
						for (size_t z = 1; z <= CANT_ELEM_IDEMTIFICADORES[y]; z++){
							if (elementos[z] == NULL){
								//identidad = NULL;
								error_finalizar(c_anteriores,identidad);
/*								for (size_t b = 0; b < c_anteriores; b++){
									finalizar();
								}
								fprintf(stderr,"%s comando %s\n",ERROR,elementos[0]);
								finalizar();*/
								if (!pila_esta_vacia(pagos)){
									vaciar_pila(pagos,wrapper_simple);
								}
								else{pila_destruir(pagos);}
								pila_destruir(pila_auxiliar);
								return false;
								//break;
							}
						}
						terminar = true;
					}
					if (identidad == NULL && !terminar){
						//fprintf(stderr,"%s %s '%s'\n",ERROR,FUNCION_INVALIDA,(char*)pila_ver_tope(pagos));
						//fprintf(stderr,"%s comando %s\n",ERROR,elementos[0]);
						//cola_encolar(procesamientos,pagos);
						//ejecutar(procesamientos,usuarios);//¿ejecutar?
						//error_finalizar(0,elementos[0]);
						error_finalizar(c_anteriores,elementos[0]);
						if (!pila_esta_vacia(pagos)){
							vaciar_pila(pagos,wrapper_simple);
						}
						else{pila_destruir(pagos);}
						pila_destruir(pila_auxiliar);
						return false;
					}
					if (terminar){
						break;
					}
				}
			}
			size_t orden_elemento = 0;
			while (elementos[orden_elemento] != NULL){
				size_t tam_elemento = strlen(elementos[orden_elemento]);
				//char nuevo_elemento[tam_elemento];
				char* nuevo_elemento = malloc(sizeof(char) * (tam_elemento + 1));
				if (nuevo_elemento == NULL){
					while (cola_esta_vacia(procesamientos)){
						pila_destruir(cola_desencolar(procesamientos));
					}
					pila_destruir(pila_auxiliar);
					pila_destruir(pagos);
					//free_strv(elementos);
					//free_strv(operaciones);
					return false;
				}
				strcpy(nuevo_elemento,elementos[orden_elemento]);
				nuevo_elemento[tam_elemento] = '\0';
				//copiar_cadenas(nuevo_elemento,elementos[orden_elemento]);
				pila_apilar(pila_auxiliar,nuevo_elemento);
				//pila_apilar(pila_auxiliar,elementos[orden_elemento]);
				orden_elemento++;
			}
			orden++;
			//free_strv(elementos);
		}
		while (!pila_esta_vacia(pila_auxiliar)){
			pila_apilar(pagos,pila_desapilar(pila_auxiliar));
		}
		pila_destruir(pila_auxiliar);
		/* verificacion de comando invalido */
//		for (size_t y = 0; y < CANT_IDENTIFICADORES; y++){
//			const char* identidad = IDENTIFICADORES[y];
//			if (strcmpc(pila_ver_tope(pagos),identidad) == 0){
//				break;//sale del for
//			}
//			if (identidad == NULL){
//				//fprintf(stderr,"%s %s '%s'\n",ERROR,FUNCION_INVALIDA,(char*)pila_ver_tope(pagos));
//				fprintf(stderr,"%s comando %s\n",ERROR,(char*)pila_ver_tope(pagos));
//				if (!pila_esta_vacia(pagos)){
//					vaciar_pila(pagos,wrapper_simple);
//				}
//				else{pila_destruir(pagos);}
//				//free_strv(operaciones);
//				return false;
//			}
//		}
		cola_encolar(procesamientos,pagos);
		c_anteriores++;
		//free_strv(operaciones);
	}
	if (!ejecutar(procesamientos,usuarios)){
		return false;
	}
	return true;
}

/* recibe un archivo abierto y una lista y pasa los datos del archivo a la lista en caso de error cierra el archivo */
bool archivo(FILE* usuarios,lista_t* lista_de_usuarios,pila_t* free_split){
	char cadena[250] = "";//MIN_COMANDO
	while (!feof(usuarios)){
		fgets(cadena,MIN_COMANDO,usuarios);
		remplazar_chr13(cadena);
		char** info_usuario = split(cadena,SEPARADOR_INFO_USUARIO);
		pila_apilar(free_split,info_usuario);
		if (!verificacion_de_tam(info_usuario,CANTIDAD_INFO_USUARIO)){//ERROR en info de usuarios
			fprintf(stderr,"%s %s\n",ERROR,INFO_USUARIO);
			/* caso de error libera la memoria y cierra el archivo */
			lista_destruir(lista_de_usuarios,wrapper_usuario);
			//free_strv(info_usuario);
			fclose(usuarios);
			return false;
		}
		size_t id = (size_t)atol((const char*)info_usuario[0]);
		usuario_t* usuario = crear_usuario(id,info_usuario[2]);
		double saldo = atof((const char*)info_usuario[1]);
		pagar_saldo(usuario,saldo);
		lista_insertar_ultimo(lista_de_usuarios,usuario);
	}
	return true;
}

/* PROGRAMA PRINCIPAL */
/* posicion del info usuario [0] id, [1]monto, [2]coordenada, [3] NULL */
/* main inicio de programa */
int main(int argc, char *argv[]){
	if (argc < 2){
		fprintf(stderr,"%s %s\n",ERROR,OPERACION_PARAMETROS);
		return 0;
	}
	const char* nombre_de_archivo = argv[1];
	cola_t* cola_de_procesamientos = cola_crear();
	bool cola = verificacion_de_existencia(cola_de_procesamientos,EXISTENCIA_COLA);
	lista_t* lista_de_usuarios = lista_crear();
	bool lista = verificacion_de_existencia(lista_de_usuarios,EXISTENCIA_LISTA);
	FILE* archivo_de_usuarios = fopen(nombre_de_archivo , "r");
	bool archivo_existe = verificacion_de_existencia(archivo_de_usuarios,EXISTENCIA_ARCHIVO);
	pila_t* free_split = pila_crear();
	bool pila = verificacion_de_existencia(free_split,EXISTENCIA_PILA);
	if(!cola && !lista && !archivo_existe && !pila){
		cola_destruir(cola_de_procesamientos,NULL);
		lista_destruir(lista_de_usuarios,NULL);
		pila_destruir(free_split);
		fclose(archivo_de_usuarios);
		return 0;//termina en error
	}
	if (!archivo(archivo_de_usuarios,lista_de_usuarios,free_split)){//caso de error antes de leer el archivo
		cola_destruir(cola_de_procesamientos,NULL);//la cola no se destuye dentro del "archivo"
		if (!pila_esta_vacia(free_split)){
			vaciar_pila(free_split,wrapper_split);
		}
		else {pila_destruir(free_split);}
		return 0;//termina en error
	}
	fclose(archivo_de_usuarios);//no se nesesita que el archivo siga abierto
	if (!interfaz(cola_de_procesamientos,lista_de_usuarios,free_split)){
		cola_destruir(cola_de_procesamientos,wrapper_pila_con_elementos);
		lista_destruir(lista_de_usuarios,wrapper_usuario);
		vaciar_pila(free_split,wrapper_split);
		//finalizar();//???
		return 0;//termina en error
	}
	lista_destruir(lista_de_usuarios,wrapper_usuario);
	cola_destruir(cola_de_procesamientos,wrapper_pila_con_elementos);
	vaciar_pila(free_split,wrapper_split);
	finalizar();
	return 0;//si llego hasta aca funciona
}
///////////////////////////////
