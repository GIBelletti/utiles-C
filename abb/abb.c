#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abb.h"
#include "pila.h"
//Alumno: Belletti Gabriel Ignacio Padron: 100053 Corrector: Jorge Collinet Grupo: G38
typedef struct nodo {
	struct nodo* izquierda;
	const char* clave;
	void* valor;
	struct nodo* derecha;
} nodo_t;

struct abb {
	nodo_t* raiz;
	abb_comparar_clave_t comparar;
	abb_destruir_dato_t destruir;
	size_t cantidad;
};

struct abb_iter{
	const nodo_t* actual;
	const abb_t* arbol;
	size_t cont;
	pila_t* pila;
};
/*
abb_comparar_clave_t : recibe dos const char* y devuelve un int que es 0 si las dos cadenas son iguales <0 si cadena1 < cadena2 (va derecha) y >0 si cadena1 > cadena2 (va izquierda)
*/

nodo_t* encontrar(nodo_t* nodo, const char* clave, nodo_t** padre, abb_comparar_clave_t comparar){ 
	if(nodo == NULL){
		return NULL;
	}
	if(comparar(nodo->clave, clave) != 0){
		*padre = nodo;
		if(comparar(nodo->clave, clave) < 0){
			return encontrar(nodo->derecha, clave, padre, comparar);
		}
		return encontrar(nodo->izquierda, clave, padre, comparar);
	}
	return nodo;
}

void abb_destruir_nodos(nodo_t* nodo, abb_destruir_dato_t destruir){
	if (nodo == NULL){
		return;
	}
	abb_destruir_nodos(nodo->izquierda,destruir);
	abb_destruir_nodos(nodo->derecha,destruir);
	if (destruir != NULL){
		destruir(nodo->valor);
	}
	free((void*)nodo->clave);//agrego el void por que es un cosnt char* y no permite eliminar un const
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
	abb->cantidad = 0;
	return abb;
}

bool crear_nodo(abb_t* arbol, const char *clave, void *dato, nodo_t* padre){
	nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
	if(nodo_nuevo == NULL){
		return false;
	}
	nodo_nuevo->clave = malloc(sizeof(char)* (strlen(clave) + 1));
	if(nodo_nuevo->clave == NULL){
		free(nodo_nuevo);
		return false;
	}
	strcpy((char*)nodo_nuevo->clave, (char*)clave);
	nodo_nuevo->izquierda = NULL;
	nodo_nuevo->derecha = NULL;
	nodo_nuevo->valor = dato;
	if(padre != NULL){
		if (arbol->comparar(padre->clave, clave) > 0){
			padre->izquierda = nodo_nuevo;
		}else {
			padre->derecha = nodo_nuevo;
		}
	}else {
		arbol->raiz = nodo_nuevo;
	}
	arbol->cantidad += 1;
	return true;
}

/* recive la clave valor de un elemento de un arbol y lo coloca en la poscision correcta (menores izq mayores der), caso de que ya exista una clave equivalente o igual remplaza el dato de esta (el dato anterior se destruye en el proceso) */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if(arbol == NULL)
		return false;
	nodo_t** padre = malloc(sizeof(nodo_t*));
	*padre = NULL;
	nodo_t* nodo = encontrar(arbol->raiz, clave, padre, arbol->comparar);
	if(nodo == NULL){
		if (!crear_nodo(arbol,clave,dato,*padre)){
			free(padre);
			return false;
		}
	}else {
		if(arbol->destruir != NULL){
			arbol->destruir(nodo->valor);
		}
		nodo->valor = dato;
	}
	free(padre);
	return true;
}

/* regresa el dato y borra el "nodo" del arbol */
void *abb_borrar_nodos(nodo_t* nodo, const char *clave, abb_t* arbol, nodo_t* padre){
	if (nodo == NULL){
		return NULL;
	}
	void* dato = nodo->valor;
	if (nodo->derecha == NULL && nodo->izquierda == NULL){//caso sin hijos
		if (padre != NULL){
			if(padre->izquierda == nodo){
				abb_destruir_nodos(nodo, arbol->destruir);
				padre->izquierda = NULL;
			}else{
				abb_destruir_nodos(nodo, arbol->destruir);
				padre->derecha = NULL;
			}
		}else {
			abb_destruir_nodos(nodo, arbol->destruir);
			arbol->raiz = NULL;
		}
		return dato;
	}
	nodo_t* padre_del_borrado_a_borrar = padre;
	if ((nodo->derecha == NULL && nodo->izquierda != NULL) || (nodo->derecha != NULL && nodo->izquierda == NULL)){//caso 1 hijo
		if (nodo->derecha != NULL && nodo->izquierda == NULL){
			if(padre_del_borrado_a_borrar != NULL){
				if(padre_del_borrado_a_borrar->derecha == nodo){
					padre->derecha = nodo->derecha;
				}else {padre->izquierda = nodo->derecha;}
			}else {arbol->raiz = nodo->derecha;}
			nodo->derecha = NULL;
			abb_destruir_nodos(nodo, arbol->destruir);
		}else {
			if(padre_del_borrado_a_borrar != NULL){
				if(padre_del_borrado_a_borrar->derecha == nodo){
					padre->derecha = nodo->izquierda;
				}else {padre->izquierda = nodo->izquierda;}
			}else {arbol->raiz = nodo->izquierda;}
			nodo->izquierda = NULL;
			abb_destruir_nodos(nodo, arbol->destruir);
		}
		return dato;
	}//si llega hasta aca tiene dos hijos
	nodo_t* siguiente_max = nodo->derecha;
	nodo_t* anterior = nodo;
	while (siguiente_max->izquierda != NULL){
		anterior = siguiente_max;
		siguiente_max = siguiente_max->izquierda;
	}//uno a la derecha todo a la izquierda
	const char* clave_siguiente_max = malloc(sizeof(siguiente_max->clave) + sizeof(char));
	strcpy((char*)clave_siguiente_max, siguiente_max->clave);
	void* valor_siguiente_max = abb_borrar_nodos(siguiente_max,clave_siguiente_max, arbol, anterior);//borra el nodo
	//suplanta los valores del nodo, manteniendo su ubicacion
	nodo->valor = valor_siguiente_max;
	free((void*)nodo->clave);
	nodo->clave = clave_siguiente_max;
	return dato;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if (arbol == NULL){
		return NULL;
	}
	nodo_t** padre = malloc(sizeof(nodo_t*));
	*padre = NULL;
	nodo_t* nodo = encontrar(arbol->raiz, clave, padre, arbol->comparar);
	void* valor = NULL;
	if (nodo != NULL){
		valor = abb_borrar_nodos(nodo, (char*)clave, arbol, *padre);
		arbol->cantidad -=1;
	}
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
	nodo_t* nodo = encontrar(arbol->raiz, clave, padre, arbol->comparar);
	free(padre);
	if(nodo == NULL){
		return NULL;
	}
	return nodo->valor;
}

/* recibe un arbol y una clave y devuelve el true si existe la clave recibida en el arbol o false si no se encontro */
bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(arbol == NULL)
		return false;
	if (arbol->raiz == NULL){
		return false;
	}
	nodo_t** padre = malloc(sizeof(nodo_t*));
	nodo_t* nodo = encontrar(arbol->raiz, clave, padre, arbol->comparar);
	free(padre);
	if(nodo == NULL){
		return false;
	}
	return true;
}

/* recibe un arbol y cuenta la cantidad de elementos que tiene */
size_t abb_cantidad(abb_t *arbol){
	if(arbol == NULL)
		return 0;
	return arbol->cantidad;
}

/* recibe un arbol y destruye todos sus nodos valores y a si mismo */
void abb_destruir(abb_t *arbol){
	if (arbol == NULL){
		return;
	}
	abb_destruir_nodos(arbol->raiz, arbol->destruir);
	free(arbol);
	return;
}

/* ITERADOR */
bool abb_in_order_nodo(nodo_t* nodo, bool visitar(const char*, void*, void*), void* extra){
	if(nodo == NULL)
		return true;
	if (!abb_in_order_nodo(nodo->izquierda, visitar, extra)){
		return false;
	}
	if(!visitar(nodo->clave, nodo->valor, extra))
		return false;
	if(!abb_in_order_nodo(nodo->derecha, visitar, extra)){
		return false;
	}
	return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	nodo_t* nodo = arbol->raiz;
	abb_in_order_nodo(nodo, visitar, extra);
	return;
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	if (arbol == NULL){return NULL;}
	abb_iter_t* abb_iter_nuevo = malloc(sizeof(abb_iter_t));
	if(abb_iter_nuevo == NULL){
		return NULL;
	}
	pila_t* pila = pila_crear();
	if (pila == NULL){
		free(abb_iter_nuevo);
		return NULL;
	}
	abb_iter_nuevo->actual = arbol->raiz;
	abb_iter_nuevo->arbol = arbol;
	abb_iter_nuevo->cont = 0;
	abb_iter_nuevo->pila = pila;
	pila_apilar(pila,(void*)arbol->raiz);
	if (arbol->raiz != NULL){//en caso de que sea abb no vacio
		while(abb_iter_nuevo->actual->izquierda != NULL){
			abb_iter_nuevo->actual = abb_iter_nuevo->actual->izquierda;
			pila_apilar(pila,(void*)abb_iter_nuevo->actual);
		}
	}
	return abb_iter_nuevo;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if (abb_iter_in_al_final(iter)){
		return false;
	}
	pila_desapilar(iter->pila);
	const nodo_t* der = iter->actual->derecha;
	iter->cont++;
	if (der == NULL){
		iter->actual = pila_ver_tope(iter->pila);
		return true;
	}
	iter->actual = der;
	pila_apilar(iter->pila,(void*)iter->actual);
	while(iter->actual->izquierda != NULL){
		iter->actual = iter->actual->izquierda;
		pila_apilar(iter->pila,(void*)iter->actual);
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if(iter->actual == NULL)
		return NULL;
	return iter->actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return (iter->actual == NULL);
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
	return;
}

/////////////////////////////////////////////////
