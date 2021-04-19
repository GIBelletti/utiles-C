#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abb.h"

struct abb {
	struct abb* izquierda;
	const char* clave;
	void* valor;
	struct abb* derecha;
	abb_comparar_clave_t comparar;
	abb_destruir_dato_t destruir;
	struct abb* padre;
};

struct abb_iter{
	const abb_t* actual;
};
/*
abb_comparar_clave_t : recibe dos const char* y devuelve un int que es 0 si las dos cadenas son iguales <0 si cadena1 < cadena2 (va derecha) y >0 si cadena1 > cadena2 (va izquierda)
*/

/* crea un nuevo arbol vacio con sus propias funciones de comparar claves y destruir valores, caso de falla al reservar memoria devuelve NULL */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb_nuevo = malloc(sizeof(abb_t));
	if (abb_nuevo == NULL){
		return NULL;
	}
	//funciones
	abb_nuevo->comparar = cmp;
	abb_nuevo->destruir = destruir_dato;
	//valores
	abb_nuevo->clave = NULL;
	abb_nuevo->valor = NULL;
	//hijos
	abb_nuevo->izquierda = NULL;
	abb_nuevo->derecha = NULL;
	//padre
	abb_nuevo->padre = NULL;
	return abb_nuevo;
}

/* recive la clave valor de un elemento de un arbol y lo coloca en la poscision correcta (menores izq mayores der), caso de que ya exista una clave equivalente o igual remplaza el dato de esta (el dato anterior se destruye en el proceso) */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if (arbol == NULL){return false;}
	if (arbol->clave == NULL){
		arbol->clave = clave;
		arbol->valor = dato;
		return true;
	}
	if (arbol->comparar(arbol->clave,clave) == 0){
		arbol->destruir(arbol->valor);
		arbol->valor = dato;
		return true;
	}
	if (arbol->comparar(arbol->clave,clave) > 0){
		if (arbol->izquierda == NULL){
			abb_t* izquierda = abb_crear(arbol->comparar,arbol->destruir);
			izquierda->padre = arbol;
			izquierda->clave = clave;
			izquierda->valor = dato;
			arbol->izquierda = izquierda;
			return true;
		}
		return abb_guardar(arbol->izquierda,clave,dato);
	}
	if (arbol->derecha == NULL){
		abb_t* derecha = abb_crear(arbol->comparar,arbol->destruir);
		derecha->padre = arbol;
		derecha->clave = clave;
		derecha->valor = dato;
		arbol->derecha = derecha;
		return true;
	}
	return abb_guardar(arbol->derecha,clave,dato);
}

/* regresa el dato y borra el "nodo" del arbol */
void *abb_borrar(abb_t *arbol, const char *clave){
	if (arbol == NULL){
		return NULL;
	}
	if (arbol->comparar(arbol->clave,clave) == 0){
		void* dato = arbol->valor;
		if (arbol->derecha == NULL && arbol->izquierda == NULL){//caso sin hijos
			if (arbol->padre != NULL){
				abb_destruir(arbol);
			}
			else {//si es la raiz sin hijos no se destruye
				arbol->clave = NULL;
				arbol->valor = NULL;
			}
			return dato;
		}
		abb_t* padre_del_arbol_a_borrar = arbol->padre;
		if ((arbol->derecha == NULL && arbol->izquierda != NULL) || (arbol->derecha != NULL && arbol->izquierda == NULL)){
			abb_t* der = arbol->derecha;
			abb_t* izq = arbol->izquierda;
			free(arbol);
			arbol = NULL;
			if (arbol->derecha != NULL && arbol->izquierda == NULL){
				arbol = der;
			}
			else {
				arbol = izq;
			}
			arbol->padre = padre_del_arbol_a_borrar;
			return dato;
		}//si llega hasta aca tiene dos hijos
		abb_t* siguiente_max = arbol->derecha;
		while (siguiente_max->izquierda != NULL){
			siguiente_max = siguiente_max->izquierda;
		}//uno a la derecha todo a la izquierda
		const char* clave_siguiente_max = siguiente_max->clave;
		void* valor_siguiente_max = abb_borrar(siguiente_max,clave_siguiente_max);//borra el nodo
		//suplanta los valores del nodo, manteniendo su ubicacion
		arbol->valor = valor_siguiente_max;
		arbol->clave = clave_siguiente_max;
		return dato;
	}
	if (arbol->comparar(arbol->clave,clave) > 0){
		return abb_borrar(arbol->izquierda,clave);
	}
	return abb_borrar(arbol->derecha,clave);
}

/* recibe un arbol y una clave y devuelve el valor de dicha clave, caso de que no xista la clave en el arbol devuelve NULL */
void *abb_obtener(const abb_t *arbol, const char *clave){
	if (arbol->clave == NULL){
		return NULL;
	}
	if (arbol->comparar(arbol->clave,clave) > 0){
		return abb_obtener(arbol->izquierda,clave);
	}
	if (arbol->comparar(arbol->clave,clave) < 0){
		return abb_obtener(arbol->derecha,clave);
	}
	return arbol->valor;
}

/* recibe un arbol y una clave y devuelve el true si existe la clave recibida en el arbol o false si no se encontro */
bool abb_pertenece(const abb_t *arbol, const char *clave){
	if (arbol->clave == NULL){
		return false;
	}
	if (arbol->comparar(arbol->clave,clave) < 0){
		return abb_pertenece(arbol->izquierda,clave);
	}
	if (arbol->comparar(arbol->clave,clave) > 0){
		return abb_pertenece(arbol->derecha,clave);
	}
	return true;
}

/* recibe un arbol y cuenta la cantidad de elementos que tiene */
size_t abb_cantidad(abb_t *arbol){
	if (arbol->clave == NULL){
		return 0;
	}
	if (arbol->derecha == NULL && arbol->izquierda == NULL){
		return 1;
	}
	return (1 + abb_cantidad(arbol->derecha) + abb_cantidad(arbol->izquierda));
}

/* recibe un arbol y destruye todos sus nodos valores y a si mismo */
void abb_destruir(abb_t *arbol){
	if (arbol == NULL){
		return;
	}
	abb_destruir(arbol->izquierda);
	abb_destruir(arbol->derecha);
	if (arbol->destruir != NULL){
		arbol->destruir(arbol->valor);
	}
	arbol->padre = NULL;
	free(arbol);
	return;
}

/* ITERADOR */

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* abb_iter_nuevo = malloc(sizeof(abb_iter_t));
	if(abb_iter_nuevo == NULL){
		return NULL;
	}
	abb_iter_nuevo->actual = arbol;
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
		return true;
	}
	if (iter->actual != iter->actual->padre->derecha){
		iter->actual = iter->actual->padre;
		return true;
	}
	while(iter->actual == iter->actual->padre->derecha){
		iter->actual = iter->actual->padre;
		if (iter->actual->padre == NULL){
			return false;
		}
	}
	iter->actual = iter->actual->padre;
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	return iter->actual->valor;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return (iter->actual->padre == NULL && iter->actual->derecha == NULL);
}

void abb_iter_in_destruir(abb_iter_t* iter){
	free(iter);
	return;
}

/////////////////////////////////////////////////
