#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abb.h"

typedef struct nodo {
	struct nodo* izquierda;
	const char* clave;
	void* valor;
	struct nodo* derecha;
	abb_comparar_clave_t comparar;
	abb_destruir_dato_t destruir;
	struct nodo* padre;
} nodo_t;

struct abb {
	nodo_t* raiz;
	abb_comparar_clave_t comparar;
	abb_destruir_dato_t destruir;
};

struct abb_iter{
	const nodo_t* actual;
	const abb_t* arbol;
	size_t cont;
};
/*
abb_comparar_clave_t : recibe dos const char* y devuelve un int que es 0 si las dos cadenas son iguales <0 si cadena1 < cadena2 (va derecha) y >0 si cadena1 > cadena2 (va izquierda)
*/

nodo_t* encontrar(nodo_t* abb, const char* clave, nodo_t** padre){ 
	if(abb == NULL){
		return NULL;
	}
	if(abb->comparar(abb->clave, clave) > 0){
		*padre = abb;
		return encontrar(abb->izquierda, clave, padre);
	}
	if(abb->comparar(abb->clave, clave) < 0){
		*padre = abb;
		return encontrar(abb->derecha, clave, padre);
	}
	return abb;
}

size_t abb_cantidad_nodos(nodo_t* nodo){
	if(nodo == NULL)
		return 0;
	if (nodo->clave == NULL){
		return 0;
	}
	if (nodo->derecha == NULL && nodo->izquierda == NULL){
		return 1;
	}
	return (1 + abb_cantidad_nodos(nodo->derecha) + abb_cantidad_nodos(nodo->izquierda));
}

void abb_destruir_nodos(nodo_t* nodo){
	if (nodo == NULL){
		return;
	}
	abb_destruir_nodos(nodo->izquierda);
	abb_destruir_nodos(nodo->derecha);
	if (nodo->destruir != NULL){
		nodo->destruir(nodo->valor);
	}
	free((char*)nodo->clave);
	free(nodo);
	return;
}

/* crea un nuevo arbol vacio con sus propias funciones de comparar claves y destruir valores, caso de falla al reservar memoria devuelve NULL */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb = malloc(sizeof(abb_t));
	if(abb == NULL)
		return NULL;
	abb->raiz = NULL;
	abb->comparar = cmp;
	abb->destruir = destruir_dato;
	return abb;
}

/* recive la clave valor de un elemento de un arbol y lo coloca en la poscision correcta (menores izq mayores der), caso de que ya exista una clave equivalente o igual remplaza el dato de esta (el dato anterior se destruye en el proceso) */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if(arbol == NULL)
		return false;
	nodo_t** padre = malloc(sizeof(nodo_t*));
	*padre = NULL;
	nodo_t* nodo = encontrar(arbol->raiz, clave, padre);
	if(nodo == NULL){
		nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
		if(nodo_nuevo == NULL){
			free(padre);
			return false;
		}
		nodo_nuevo->clave = malloc(sizeof(char)*sizeof(clave));
		if(nodo_nuevo->clave == NULL){
			free(padre);
			free(nodo);
			return false;
		}
		strcpy((char*)nodo_nuevo->clave, (char*)clave);
		nodo_nuevo->padre = *padre;
		nodo_nuevo->izquierda = NULL;
		nodo_nuevo->derecha = NULL;
		nodo_nuevo->valor = dato;
		nodo_nuevo->comparar = arbol->comparar;
		nodo_nuevo->destruir = arbol->destruir;
		if(*padre != NULL){
			if (arbol->comparar((*padre)->clave, clave) > 0){
				(*padre)->izquierda = nodo_nuevo;
			}
			else {
				(*padre)->derecha = nodo_nuevo;
			}
		}
		else {
			arbol->raiz = nodo_nuevo;
		}
		free(padre);
		return true;
	}
	else {
		if(nodo->destruir != NULL)
			nodo->destruir(nodo->valor);
		nodo->valor = dato;
		free(padre);
		return true;
	}
}

/* regresa el dato y borra el "nodo" del arbol */
void *abb_borrar_nodos(nodo_t* nodo, const char *clave, abb_t* arbol){
	if (nodo == NULL){
		return NULL;
	}
	void* dato = nodo->valor;
	if (nodo->comparar(nodo->clave, clave) == 0){
		if (nodo->derecha == NULL && nodo->izquierda == NULL){//caso sin hijos
			if (nodo->padre != NULL){
				nodo_t* referencia = nodo->padre;
				if(nodo->comparar(referencia->clave, nodo->clave) > 0){
					abb_destruir_nodos(nodo);
					referencia->izquierda = NULL;
				}
				else{
					abb_destruir_nodos(nodo);
					referencia->derecha = NULL;
				}
			}
			else {
				abb_destruir_nodos(nodo);
				arbol->raiz = NULL;
			}
			return dato;
		}
		nodo_t* padre_del_borrado_a_borrar = nodo->padre;
		if ((nodo->derecha == NULL && nodo->izquierda != NULL) || (nodo->derecha != NULL && nodo->izquierda == NULL)){//caso 1 hijo
				if (nodo->derecha != NULL && nodo->izquierda == NULL){
					nodo->derecha->padre = padre_del_borrado_a_borrar;
					if(padre_del_borrado_a_borrar != NULL){
						if(padre_del_borrado_a_borrar->derecha == nodo)
							nodo->padre->derecha = nodo->derecha;
						else nodo->padre->izquierda = nodo->derecha;
					}
					else arbol->raiz = nodo->derecha;
					nodo->derecha = NULL;
					abb_destruir_nodos(nodo);
				}
				else {
					nodo->izquierda->padre = padre_del_borrado_a_borrar;
					if(padre_del_borrado_a_borrar != NULL){
						if(padre_del_borrado_a_borrar->derecha == nodo)
							nodo->padre->derecha = nodo->izquierda;
						else nodo->padre->izquierda = nodo->izquierda;
					}
					else arbol->raiz = nodo->izquierda;
					nodo->izquierda = NULL;
					abb_destruir_nodos(nodo);
				}
				return dato;
		}//si llega hasta aca tiene dos hijos
		nodo_t* siguiente_max = nodo->derecha;
		while (siguiente_max->izquierda != NULL){
			siguiente_max = siguiente_max->izquierda;
		}//uno a la derecha todo a la izquierda
		const char* clave_siguiente_max = siguiente_max->clave;
		void* valor_siguiente_max = abb_borrar_nodos(siguiente_max,clave_siguiente_max, arbol);//borra el nodo
		//suplanta los valores del nodo, manteniendo su ubicacion
		nodo->valor = valor_siguiente_max;
		nodo->clave = realloc((char*)nodo->clave, sizeof(clave_siguiente_max));
		strcpy((char*)nodo->clave, (char*)clave_siguiente_max);
		return dato;
	}
	return NULL;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if (arbol == NULL){
		return NULL;
	}
	nodo_t** padre = malloc(sizeof(nodo_t*));
	*padre = NULL;
	nodo_t* nodo = encontrar(arbol->raiz, clave, padre);
	void* valor = abb_borrar_nodos(nodo, (char*)clave, arbol);
	free(padre);
	return valor;
}

/* recibe un arbol y una clave y devuelve el valor de dicha clave, caso de que no xista la clave en el arbol devuelve NULL */
void *abb_obtener(const abb_t *arbol, const char *clave){
	if (arbol == NULL)
		return NULL;
	if(arbol->raiz == NULL)
		return NULL;
	nodo_t** padre = malloc(sizeof(nodo_t*));
	nodo_t* nodo = encontrar(arbol->raiz, clave, padre);
	if(nodo == NULL){
		free(padre);
		return NULL;
	}
	if(arbol->comparar(nodo->clave, (char*)clave) == 0){
		free(padre);
		return nodo->valor;
	}
	free(padre);
	return NULL;
}

/* recibe un arbol y una clave y devuelve el true si existe la clave recibida en el arbol o false si no se encontro */
bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(arbol == NULL)
		return false;
	if (arbol->raiz == NULL){
		return false;
	}
	nodo_t** padre = malloc(sizeof(nodo_t*));
	nodo_t* nodo = encontrar(arbol->raiz, clave, padre);
	if(nodo == NULL){
		free(padre);
		return false;
	}
	if(arbol->comparar(nodo->clave, (char*)clave) == 0){
		free(padre);
		return true;
	}
	free(padre);
	return false;
}

/* recibe un arbol y cuenta la cantidad de elementos que tiene */
size_t abb_cantidad(abb_t *arbol){
	if(arbol == NULL)
		return 0;
	return abb_cantidad_nodos(arbol->raiz);
}

/* recibe un arbol y destruye todos sus nodos valores y a si mismo */
void abb_destruir(abb_t *arbol){
	if (arbol == NULL){
		return;
	}
	abb_destruir_nodos(arbol->raiz);
	free(arbol);
	return;
}

/* ITERADOR */
void abb_in_order_nodo(nodo_t* nodo, bool visitar(const char*, void*, void*), void* extra, abb_t* arbol){
	if(nodo == NULL)
		return;
	abb_in_order_nodo(nodo->izquierda, visitar, extra, arbol);
	bool estado = visitar(nodo->clave, nodo->valor, extra);
	if (estado){
		abb_guardar(arbol,nodo->clave,NULL);
	}
	if(!estado && nodo->padre != NULL){
		return;
	}
	abb_in_order_nodo(nodo->derecha, visitar, extra, arbol);
	return;
}

abb_t* abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	abb_t* arbol_recortado = abb_crear(arbol->comparar, arbol->destruir);
	if (arbol_recortado == NULL){return NULL;}
	nodo_t* nodo = arbol->raiz;
	abb_in_order_nodo(nodo, visitar, extra, arbol_recortado);
	return arbol_recortado;
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* abb_iter_nuevo = malloc(sizeof(abb_iter_t));
	if(abb_iter_nuevo == NULL){
		return NULL;
	}
	abb_iter_nuevo->actual = arbol->raiz;
	abb_iter_nuevo->arbol = arbol;
	abb_iter_nuevo->cont = 0;
	while(abb_iter_nuevo->actual->izquierda != NULL){
		abb_iter_nuevo->actual = abb_iter_nuevo->actual->izquierda;
	}
	return abb_iter_nuevo;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)){
		return false;
	}
	if (iter->actual->derecha != NULL){
		iter->actual = iter->actual->derecha;
		while(iter->actual->izquierda != NULL){
			iter->actual = iter->actual->izquierda;
		}
		iter->cont ++;
		return true;
	}
	if (iter->actual != iter->actual->padre->derecha){
		iter->actual = iter->actual->padre;
		iter->cont ++;
		return true;
	}
	while(iter->actual == iter->actual->padre->derecha){
		iter->actual = iter->actual->padre;
		if (iter->actual->padre == NULL){
			return false;
		}
	}
	iter->actual = iter->actual->padre;
	iter->cont ++;
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if(abb_iter_in_al_final(iter) == true)
		return NULL;
	return iter->actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	if((iter->cont + 1) == abb_cantidad((abb_t*)iter->arbol))
		return true;
	return false;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	free(iter);
	return;
}

/////////////////////////////////////////////////
