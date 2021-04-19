#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "hash.h"
/* Alumno: Belletti Gabriel Ignacio Padron: 100053 Grupo: G38 Corrector: Jorge Collinet */
/* CONSTANTES */
const size_t CAPACIDAD_BASE = 10;
const char NULO = '\0';
const float VALOR_ALPHA_HASH_ABIERTO_INCREMENTAL = 2;
const float VALOR_ALPHA_HASH_ABIERTO_DECREMENTAL = 0.25;
/* FUNCION DE TRANSFORMACION DE CLAVE */
/* recibe una clave (cadena de caracteres) y la transforma en un numero que devuelve */
unsigned int RSHash(const char* str, unsigned int length)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str, ++i)
   {
      hash = hash * a + (unsigned int)(*str);
      a    = a * b;
   }

   return hash;
}

struct hash{
	lista_t** arreglo;
	size_t capacidad;
	size_t cantidad;
	hash_destruir_dato_t destruir;
};

struct hash_iter{
	lista_iter_t* actual;
	size_t pos_lista;
	size_t ultimo;
	const hash_t* hash;
};

struct elemento{
	const char* clave;
	void* dato;
};

typedef struct elemento elemento_t;
void redimensionar(hash_t* hash);

/* dada una lista y una clave avanza sobre la lista y devuelve el elemnto destruyendolo o no de la lista, da NULL en caso de no encontrarse en la lista el elemnto o falla */
elemento_t* dar_elemento(lista_t* lista, const char* clave, bool destruir){
	if (lista == NULL || clave == NULL){
		return NULL;
	}
	lista_iter_t* iter = lista_iter_crear(lista);
	elemento_t* elemento = lista_iter_ver_actual(iter);
	if (elemento == NULL){
		lista_iter_destruir(iter);
		return NULL;
	}
	while(strcmp(elemento->clave, clave) != 0){
		if (!lista_iter_avanzar(iter)){
			lista_iter_destruir(iter);
			return NULL;
		}
		elemento = lista_iter_ver_actual(iter);
		if (elemento == NULL){
			lista_iter_destruir(iter);
			return NULL;
		}
	}
	if (destruir){
		lista_iter_borrar(iter);
	}
	lista_iter_destruir(iter);
	return elemento;
}

/* crea un elemento dandole clave y valor */
elemento_t* crear_elemento(const char* clave,void* dato){
	elemento_t* elemento_nuevo = malloc(sizeof(elemento_t));
	if(elemento_nuevo == NULL)
		return NULL;
	char* clave_guardar = malloc(strlen(clave)+1);
	if(clave_guardar == NULL){
		free(elemento_nuevo);
		return NULL;
	}
	strcpy(clave_guardar, clave);
	elemento_nuevo->clave = clave_guardar;
	elemento_nuevo->dato = dato;
	return elemento_nuevo;
}

/* crea un hash vacio de n tamaño (se usa para redimension) */
hash_t* hash_crear_tam(hash_destruir_dato_t destruir_dato, size_t capacidad){
	hash_t* nuevo_hash = malloc(sizeof(hash_t));
	if (nuevo_hash == NULL){
		return NULL;
	}
	nuevo_hash->capacidad = capacidad;
	nuevo_hash->cantidad = 0;
	nuevo_hash->arreglo = malloc(sizeof(lista_t*) * capacidad);
	if(nuevo_hash->arreglo == NULL){
		free(nuevo_hash);
		return NULL;
	}
	for (size_t i = 0; i < capacidad; i++){
		nuevo_hash->arreglo[i] = lista_crear();
	}
	nuevo_hash->destruir = destruir_dato;
	return nuevo_hash;
}

/* crea un hash vacio de un tamaño fijo (primitiva que puede usar el usuario) */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* nuevo_hash = hash_crear_tam(destruir_dato,CAPACIDAD_BASE);
	return nuevo_hash;//si regresa NULL devuelve NULL
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la estructura, la reemplaza. De no poder guardarlo devuelve false. Pre: La estructura hash fue inicializada Post: Se almacenó el par (clave, dato) */
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	size_t valor = RSHash(clave, (unsigned int)strlen(clave)) % hash->capacidad;
	elemento_t* elemento = dar_elemento(hash->arreglo[valor],clave,false);
	if (elemento != NULL){
		if (hash->destruir != NULL){
			hash->destruir(elemento->dato);
		}
		elemento->dato = dato;
		return true;
	}
	elemento = crear_elemento(clave,dato);
	if (elemento == NULL){return false;}
	hash->cantidad++;
	lista_insertar_ultimo(hash->arreglo[valor], elemento);
	/* redimension */
	redimensionar(hash);
	return true;
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve NULL si el dato no estaba. Pre: La estructura hash fue inicializada Post: El elemento fue borrado de la estructura y se lo devolvió, en el caso de que estuviera guardado. */
void *hash_borrar(hash_t *hash, const char *clave){
	size_t valor = RSHash(clave, (unsigned int)strlen(clave)) % hash->capacidad;
	elemento_t* elemento = dar_elemento(hash->arreglo[valor],clave,true);
	if (elemento == NULL){//da null si no existe el elemento
		return NULL;
	}
	void* dato = elemento->dato;
	hash->cantidad--;
	/* redimension */
	redimensionar(hash);
	free((char*)elemento->clave);
	free(elemento);
	return dato;
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra devuelve NULL. Pre: La estructura hash fue inicializada */
void *hash_obtener(const hash_t *hash, const char *clave){
	size_t valor = RSHash(clave, (unsigned int)strlen(clave)) % hash->capacidad;
	elemento_t* elemento = dar_elemento(hash->arreglo[valor],clave,false);
	if (elemento == NULL){
		return NULL;
	}
	return elemento->dato;
}

/* Determina si clave pertenece o no al hash. Pre: La estructura hash fue inicializada */
bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t valor = RSHash(clave, (unsigned int)strlen(clave)) % hash->capacidad;
	elemento_t* elemento = dar_elemento(hash->arreglo[valor],clave,false);
	if (elemento == NULL){
		return false;
	}
	return true;
}

/* Devuelve la cantidad de elementos del hash. Pre: La estructura hash fue inicializada */
size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función destruir para cada par (clave, dato). Pre: La estructura hash fue inicializada Post: La estructura hash fue destruida */
void hash_destruir(hash_t *hash){
	if (hash == NULL){
		return;
	}
	for (size_t contador = 0; contador < hash->capacidad; contador++){
		lista_t* lista = hash->arreglo[contador];
		while (!lista_esta_vacia(lista)){
			elemento_t* elemento = lista_borrar_primero(lista);
			if (hash->destruir != NULL){
				hash->destruir(elemento->dato);
			}
			free((char*)elemento->clave);
			free(elemento);
			hash->cantidad--;
		}
		lista_destruir(lista,NULL);
	}
	free(hash->arreglo);
	free(hash);
	return;
}

/* Iterador del hash */

/* Crea iterador */
hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* nuevo_iter = malloc(sizeof(hash_iter_t));
	if (nuevo_iter == NULL){
		return NULL;
	}
	nuevo_iter->hash = hash;
	size_t listas = hash->capacidad;
	nuevo_iter->pos_lista = 0;
	nuevo_iter->ultimo = listas;
	size_t i = 0;
	for(i = 0; i < listas;i++){
		if(lista_esta_vacia(hash->arreglo[i]))
			continue;
		else break;
	}
	if(i < listas){
		nuevo_iter->actual = lista_iter_crear(hash->arreglo[i]);
		nuevo_iter->pos_lista = i;
	}
	else nuevo_iter->actual = lista_iter_crear(hash->arreglo[i-1]);
	return nuevo_iter;
}

/* Avanza iterador */
bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter))
		return false;
	if(!lista_iter_al_final(iter->actual)){
		lista_iter_avanzar(iter->actual);
		if(!lista_iter_al_final(iter->actual)){
			return true;
		}
	}
	iter->pos_lista++;
	if(iter->pos_lista == iter->ultimo)
		return false;
	lista_iter_destruir(iter->actual);
	iter->actual = lista_iter_crear(iter->hash->arreglo[iter->pos_lista]);
	while(lista_esta_vacia(iter->hash->arreglo[iter->pos_lista])){
		iter->pos_lista++;
		if(iter->pos_lista < iter->ultimo){
			lista_iter_destruir(iter->actual);
			iter->actual = lista_iter_crear(iter->hash->arreglo[iter->pos_lista]);
		}
		else break;
	}
	return true;
}

/* Devuelve clave actual, esa clave no se puede modificar ni liberar. */
const char *hash_iter_ver_actual(const hash_iter_t *iter){
	elemento_t* elemento = lista_iter_ver_actual(iter->actual);
	if (elemento == NULL){
		return NULL;
	}
	return elemento->clave;
}

/* Comprueba si terminó la iteración */
bool hash_iter_al_final(const hash_iter_t *iter){
	if (lista_iter_al_final(iter->actual)){
		for(size_t i = iter->pos_lista + 1; i < iter->ultimo; i++){
			if(!lista_esta_vacia(iter->hash->arreglo[i])){
				return false;
			}
		}
		return true;
	}
	return false;
}

/* Destruye iterador */
void hash_iter_destruir(hash_iter_t* iter){
	free(iter->actual);
	free(iter);
	return;
}

/* intercambia un hash por otro */
void swap_hash(hash_t* hash1, hash_t* hash2){
	
	lista_t** aux_l = hash1->arreglo;
	size_t aux_cap = hash1->capacidad;
	size_t aux_cant = hash1->cantidad;
	hash1->arreglo = hash2->arreglo;
	hash1->capacidad = hash2->capacidad;
	hash1->cantidad = hash2->cantidad;
	hash2->arreglo = aux_l;
	hash2->capacidad = aux_cap;
	hash2->cantidad = aux_cant;
	return;
}

/* redimension */
/* recibe un hash que sera redimensionado y toma sus clave valor y los pasa guarda en si mismo luego de la redimension, solo si llega el caso de la condicion de redimension */
void redimensionar(hash_t* hash){
	float alpha = (float)hash->cantidad / (float)hash->capacidad;
	size_t nueva_capacidad = 0;
	if(alpha <= VALOR_ALPHA_HASH_ABIERTO_DECREMENTAL && hash->capacidad > CAPACIDAD_BASE){//no puede ser menor a la capacidad base
		nueva_capacidad = hash->capacidad / 2;
	}
	if(alpha >= VALOR_ALPHA_HASH_ABIERTO_INCREMENTAL){
		nueva_capacidad = hash->capacidad * 2;
	}
	if (nueva_capacidad == 0)
		return;
	lista_t** arreglo = hash->arreglo;
	elemento_t* auxiliar[hash->cantidad];
	size_t cantidad_elemento = 0;
	for (size_t i = 0; i < hash->capacidad; i++){
		lista_t* l_actual = arreglo[i];
		lista_iter_t* iter =lista_iter_crear(l_actual);
		while (!lista_iter_al_final(iter)){
			elemento_t* elemento_base = lista_iter_ver_actual(iter);
			auxiliar[cantidad_elemento] = crear_elemento(elemento_base->clave,elemento_base->dato);
			lista_iter_avanzar(iter);
			cantidad_elemento++;
			if (cantidad_elemento > hash->cantidad)
				break;
		}
		lista_iter_destruir(iter);
	}
	hash_t* hash_auxiliar = hash_crear_tam(hash->destruir,nueva_capacidad);
	if (hash_auxiliar == NULL){
		return;
	}//caso de fallo no redimensiona
	for(size_t j = 0; j < cantidad_elemento; j++){
		elemento_t* elemento_nuevo = auxiliar[j];
		size_t valor = RSHash(elemento_nuevo->clave, (unsigned int)strlen(elemento_nuevo->clave)) % nueva_capacidad;
		lista_t* lista_hash = hash_auxiliar->arreglo[valor];
		lista_insertar_ultimo(lista_hash, elemento_nuevo);
		hash_auxiliar->cantidad++;//en este punto no hay nesesidad de verificar que sean de clave diferentes por que lo son
	}
	arreglo = NULL;
	swap_hash(hash,hash_auxiliar);
	hash_auxiliar->destruir = NULL;
	hash_destruir(hash_auxiliar);
	return;
}
///////////////////////////
