#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "usuarios.h"

struct usuario {
	size_t id;
	char* coordenadas;
	double saldo;
};

struct pago {
	size_t id;
	char* codigo;
	double monto;
};

/* primitivas de usuario */
/* crea un usuario nuevo recive su id y coordenadas empieza con saldo 0, y lo devuelve, en caso de falla devuelve NULL */
usuario_t* crear_usuario(size_t id, char* coordenadas){
	usuario_t* usuario_nuevo = malloc(sizeof(usuario_t));
	if (usuario_nuevo == NULL){
		return NULL;
	}
	usuario_nuevo->id = id;
	usuario_nuevo->coordenadas = coordenadas;
	usuario_nuevo->saldo = 0;
	return usuario_nuevo;
}

/* agrega saldo al usuario, solo numeros positivos devuelve true si se pudo realizar la operacion y false si fallo */
bool pagar_saldo(usuario_t* usuario, double monto){
	if (monto < 0){
		return false;
	}
	usuario->saldo += monto;
	return true;
}

/* quita saldo al usuario, solo numeros positivos devuelve true si se pudo realizar la operacion y false si no hay saldo que extraer o el monto a extraer es negativo */
bool extraer_saldo(usuario_t* usuario, double monto){
	if (usuario->saldo < monto || monto < 0){
		return false;
	}
	usuario->saldo -= monto;
	return true;
}

/* elimina el usuario */
void eliminar_usuario(usuario_t* usuario){
	free(usuario);
	return;
}

/* mestra el saldo del usuario */
double ver_saldo(usuario_t* usuario){
	return usuario->saldo;
}

/* devuelve la coordenada del usuario */
char* usuario_coordenada(usuario_t* usuario){
	return usuario->coordenadas;
}

/* verifica que la coordenada del usuario sea correcta (true) en caso de no serlo devuleve false */
bool verificacion_de_usuario(usuario_t* usuario,char* coordenada){
	return (usuario->coordenadas == coordenada);
}

/* primitivas de pagos */
/* crea un pago nuevo recibe su id, monto y codigo */
pago_t* crear_pago(size_t id, char* codigo, double monto){
	pago_t* pago_nuevo = malloc(sizeof(pago_t));
	if (pago_nuevo == NULL){
		return NULL;
	}
	pago_nuevo->id = id;
	//pago_nuevo->codigo = codigo;
	pago_nuevo->codigo = NULL;
	pago_nuevo->monto = monto;
	return pago_nuevo;
}

/* destruye el pago que recibe */
void destruir_pago(pago_t* pago){
	free(pago);
	return;
}

/* verifica que el pago pertenesca al usuario, recibe el pago y el usuario devuelve true si este pago le pertenece y false si no */
bool verificar_pago(pago_t* pago, usuario_t* usuario){
	return (pago->id == usuario->id/* && pago->codigo == usuario->coordenadas*/);
}

void pago_codigo(pago_t* pago,char* cod){
	pago->codigo = cod;
	return;
}
char* pago_ver_codigo(pago_t* pago){
	return pago->codigo;
}

/* da el pago que deve procesar */
double saldo_pago(pago_t* pago){
	return pago->monto;
}

/* da el id del pago */
size_t identidad_del_pago (pago_t* pago){
	return pago->id;
}

bool aumentar_pago(pago_t* pago, double valor){
	if (valor > pago->monto || valor < 0){return false;}
	pago->monto += valor;
	return true;
}

/* quita saldo del pago sin pagar */
bool pagar_pagos(pago_t* pago, double valor){
	if (valor > pago->monto || valor < 0){return false;}
	pago->monto -= valor;
	return true;
}
////////////////
