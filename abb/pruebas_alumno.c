#include "abb.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
//Alumno: Belletti Gabriel Ignacio Padron: 100053 Corrector: Jorge Collinet Grupo: G38
/////////funciones auxiliares
bool copiar(const char *clave, void *dato, void *extra){
	abb_guardar(extra, clave, dato);
	return true;
}

bool copiar_menor_a_25(const char *clave, void *dato, void *extra){
	int* val = (int*)dato;
	if (*val < 25){
		return false;
	}
	abb_guardar(extra, clave, dato);
	return true;
}

bool verificacion_de_copia(abb_t* abb1, abb_t* abb2){
	bool copia = true;
	abb_iter_t* iter1 = abb_iter_in_crear(abb1);
	abb_iter_t* iter2 = abb_iter_in_crear(abb2);
	while (!abb_iter_in_al_final(iter1) && !abb_iter_in_al_final(iter2)){
		const char* V1 = abb_iter_in_ver_actual(iter1);
		const char* V2 = abb_iter_in_ver_actual(iter2);
		if (strcmp(V1,V2) != 0){
			copia = false;
			break;
		}
		abb_iter_in_avanzar(iter1);
		abb_iter_in_avanzar(iter2);
	}
	if ((abb_iter_in_al_final(iter1) || abb_iter_in_al_final(iter2)) && (!abb_iter_in_al_final(iter1) || !abb_iter_in_al_final(iter2))){
		copia = false;
	}
	abb_iter_in_destruir(iter1);
	abb_iter_in_destruir(iter2);
	return copia;
}
/* ******************************************************************
 *			PRUEBAS UNITARIAS
 * *****************************************************************/
static void prueba_crear_abb_vacio(){
	abb_t* abb = abb_crear(strcmp, NULL);
	print_test("Prueba abb crear abb vacio", abb);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
	print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
	print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
	print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));
	abb_destruir(abb);
}


static void prueba_abb_insertar(){
	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave1 = "perro", *valor1 = "guau";
	char *clave2 = "gato", *valor2 = "miau";
	char *clave3 = "vaca", *valor3 = "mu";
	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
	print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
	print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
	print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
	/* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
	print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
	print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
	print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));
	print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
	print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
	print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
	print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
	abb_destruir(abb);
}

static void prueba_abb_reemplazar(){
	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
	char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";
	/* Inserta 2 valores y luego los reemplaza */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
	print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
	print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
	print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
	print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
	print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
	abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir(){
	abb_t* abb = abb_crear(strcmp, free);
	char *clave1 = "perro", *valor1a, *valor1b;
	char *clave2 = "gato", *valor2a, *valor2b;
	/* Pide memoria para 4 valores */
	valor1a = malloc(10 * sizeof(char));
	valor1b = malloc(10 * sizeof(char));
	valor2a = malloc(10 * sizeof(char));
	valor2b = malloc(10 * sizeof(char));
	/* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
	print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
	print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
	print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
	print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
	print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
	/* Se destruye el abb (se debe liberar lo que quedó dentro) */
	abb_destruir(abb);
}

static void prueba_abb_borrar(){
	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave1 = "perro", *valor1 = "guau";
	char *clave2 = "gato", *valor2 = "miau";
	char *clave3 = "vaca", *valor3 = "mu";
	/* Inserta 3 valores y luego los borra */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
	print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
	print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
	/* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
	print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
	print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
	print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
	print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
	print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
	print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
	print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
	print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave3));
	print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
	print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
	print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
	print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave3));
	print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
	print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
	abb_destruir(abb);
}

static void prueba_abb_clave_vacia(){
	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave = "", *valor = "";
	print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
	print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
	print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
	abb_destruir(abb);
}

static void prueba_abb_valor_null(){
	abb_t* abb = abb_crear(strcmp, NULL);
	char *clave = "", *valor = NULL;
	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
	print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
	print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
	abb_destruir(abb);
}

static void prueba_abb_volumen(size_t largo, bool debug){
	abb_t* abb = abb_crear(strcmp, NULL);
	const size_t largo_clave = 10;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);
	unsigned* valores[largo];
	/* Inserta 'largo' parejas en el abb */
	bool ok = true;
	for (unsigned i = 0; i < largo; i++) {
		valores[i] = malloc(sizeof(int));
		sprintf(claves[i], "%08d", i);
		*valores[i] = i;
		ok = abb_guardar(abb, claves[i], valores[i]);
		if (!ok) break;
	}
	if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
	if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);
	/* Verifica que devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		ok = abb_pertenece(abb, claves[i]);
		if (!ok) break;
		ok = abb_obtener(abb, claves[i]) == valores[i];
		if (!ok) break;
	}
	if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
	if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);
	/* Verifica que borre y devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		ok = abb_borrar(abb, claves[i]) == valores[i];
		if (!ok) break;
	}
	if (debug) print_test("Prueba abb borrar muchos elementos", ok);
	if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
	/* Destruye el abb y crea uno nuevo que sí libera */
	abb_destruir(abb);
	abb = abb_crear(strcmp, free);
	/* Inserta 'largo' parejas en el abb */
	ok = true;
	for (size_t i = 0; i < largo; i++) {
		ok = abb_guardar(abb, claves[i], valores[i]);
		if (!ok) break;
	}
	free(claves);
	/* Destruye el abb - debería liberar los enteros */
	abb_destruir(abb);
}

static ssize_t buscar(const char* clave, char* claves[], size_t largo){
	for (size_t i = 0; i < largo; i++) {
		if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
	}
	return -1;
}

static void prueba_abb_iterar(){
	abb_t* abb = abb_crear(strcmp, NULL);
	char *claves[] = {"perro", "gato", "vaca"};
	char *valores[] = {"guau", "miau", "mu"};
	/* Inserta 3 valores */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
	print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
	print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));
	// Prueba de iteración sobre las claves almacenadas.
	abb_iter_t* iter = abb_iter_in_crear(abb);
	const char *clave;
	ssize_t indice;
	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));
	/* Primer valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));
	/* Segundo valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));
	/* Tercer valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	/* Se anula esta prueba por diferencias de criterios */
	abb_iter_in_avanzar(iter);
	print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));
	/* Vuelve a tratar de avanzar, por las dudas */
	print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
	print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
	print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));
	abb_iter_in_destruir(iter);
	abb_destruir(abb);
}
static void prueba_abb_iterar_vacio(){
	abb_t* abb = abb_crear(strcmp, NULL);
	abb_iter_t* iter = abb_iter_in_crear(abb);
	print_test("Prueba abb iterador existe de abb vacio", iter != NULL);
	print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
	print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));
	print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
	abb_iter_in_destruir(iter);
	abb_destruir(abb);
}

static void prueba_abb_iterar_volumen(size_t largo){
	abb_t* abb = abb_crear(strcmp, NULL);
	const size_t largo_clave = 10;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);
	size_t valores[largo];
	/* Inserta 'largo' parejas en el abb */
	bool ok = true;
	for (unsigned i = 0; i < largo; i++) {
		sprintf(claves[i], "%08d", i);
		valores[i] = i;
		ok = abb_guardar(abb, claves[i], &valores[i]);
		if (!ok) break;
	}
	// Prueba de iteración sobre las claves almacenadas.
	abb_iter_t* iter = abb_iter_in_crear(abb);
	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));
	ok = true;
	unsigned i;
	const char *clave;
	size_t *valor;
	for (i = 0; i < largo; i++) {
		if ( abb_iter_in_al_final(iter) ) {
			ok = false;
			break;
		}
		clave = abb_iter_in_ver_actual(iter);
		if ( clave == NULL ) {
			ok = false;
			break;
		}
		valor = abb_obtener(abb, clave);
		if ( valor == NULL ) {
			ok = false;
			break;
		}
		*valor = largo;
		abb_iter_in_avanzar(iter);
	}
	print_test("Prueba abb iteración en volumen", ok);
	print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
	print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));
	ok = true;
	for (i = 0; i < largo; i++) {
		if ( valores[i] != largo ) {
			ok = false;
			break;
		}
	}
	print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);
	free(claves);
	abb_iter_in_destruir(iter);
	abb_destruir(abb);
}

static void prueba_abb_iterador_interno_vacio(){
	abb_t* abb1 = abb_crear(strcmp, NULL);
	abb_t* abb2 = abb_crear(strcmp, NULL);
	print_test("Prueba abb iterador interno vacio", abb1 != NULL && abb2 != NULL);
	abb_in_order(abb1,copiar,abb2);
	//verificacion de copia
	print_test("Prueba abb iterador interno que copia los elementos a otro abb", verificacion_de_copia(abb1,abb2));
	abb_destruir(abb1);
	abb_destruir(abb2);
}

static void prueba_abb_iterador_interno_verdadero(){
	abb_t* abb1 = abb_crear(strcmp, NULL);
	abb_t* abb2 = abb_crear(strcmp, NULL);
	print_test("Prueba abb iterador interno", abb1 != NULL && abb2 != NULL);
	char *claves[] = {"perro", "gato", "vaca"};
	char *valores[] = {"guau", "miau", "mu"};
	/* Inserta 3 valores */
	abb_guardar(abb1, claves[0], valores[0]);
	abb_guardar(abb1, claves[1], valores[1]);
	abb_guardar(abb1, claves[2], valores[2]);
	abb_in_order(abb1,copiar,abb2);
	//verificacion de copia
	print_test("Prueba abb iterador interno que copia los elementos a otro abb", verificacion_de_copia(abb1,abb2));
	abb_destruir(abb1);
	abb_destruir(abb2);
}

static void prueba_abb_iterador_interno_con_corte(){
	abb_t* abb1 = abb_crear(strcmp, NULL);
	abb_t* abb2 = abb_crear(strcmp, NULL);
	print_test("Prueba abb iterador interno con corte", abb1 != NULL && abb2 != NULL);
	char *claves[] = {"10", "20", "30"};
	int a = 10;
	int b = 20;
	int c = 30;
	int *valores[] = {&a, &b, &c};
	/* Inserta 3 valores */
	abb_guardar(abb1, claves[0], valores[0]);
	abb_guardar(abb1, claves[1], valores[1]);
	abb_guardar(abb1, claves[2], valores[2]);
	abb_in_order(abb1,copiar_menor_a_25,abb2);
	//verificacion
	bool copia = true;
	abb_iter_t* iter2 = abb_iter_in_crear(abb2);
	while (!abb_iter_in_al_final(iter2)){
		const char* clave = abb_iter_in_ver_actual(iter2);
		int* val = abb_obtener(abb2,clave);
		if (*val < 25){
			copia = false;
			break;
		}
		abb_iter_in_avanzar(iter2);
	}
	print_test("Prueba abb iterador interno que copia los elementos con corte", copia);
	abb_iter_in_destruir(iter2);
	abb_destruir(abb1);
	abb_destruir(abb2);
}
/* ******************************************************************
 *			FUNCIÓN PRINCIPAL
 * *****************************************************************/
void pruebas_abb_alumno(){
	/* Ejecuta todas las pruebas unitarias. */
	prueba_crear_abb_vacio();
	prueba_abb_insertar();
	prueba_abb_reemplazar();
	prueba_abb_reemplazar_con_destruir();
	prueba_abb_borrar();
	prueba_abb_clave_vacia();
	prueba_abb_valor_null();
	prueba_abb_volumen(1000, true);
	prueba_abb_iterar();
	prueba_abb_iterar_vacio();
	prueba_abb_iterar_volumen(1000);
	prueba_abb_iterador_interno_vacio();
	prueba_abb_iterador_interno_verdadero();
	prueba_abb_iterador_interno_con_corte();
}

void pruebas_volumen_alumno(size_t largo){
	prueba_abb_volumen(largo, false);
}
