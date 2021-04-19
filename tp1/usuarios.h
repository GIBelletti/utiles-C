#ifndef USUARIOS_H
#define USUARIOS_H

#include <stdbool.h>
#include <stdlib.h>

struct usuario;
typedef struct usuario usuario_t;
struct pago;
typedef struct pago pago_t;
/* crea un usuario nuevo recive su id y coordenadas empieza con saldo 0, y lo devuelve, en caso de falla devuelve NULL */
usuario_t* crear_usuario(size_t id, char* coordenadas);


/* agrega saldo al usuario, solo numeros positivos devuelve true si se pudo realizar la operacion y false si fallo */
bool pagar_saldo(usuario_t* usuario, double monto);

/* quita saldo al usuario, solo numeros positivos devuelve true si se pudo realizar la operacion y false si no hay saldo que extraer o el monto a extraer es negativo */
bool extraer_saldo(usuario_t* usuario, double monto);

/* elimina el usuario */
void eliminar_usuario(usuario_t* usuario);

/* mestra el saldo del usuario */
double ver_saldo(usuario_t* usuario);

/* devuelve la coordenada del usuario */
char* usuario_coordenada(usuario_t* usuario);

/* verifica que la coordenada del usuario sea correcta (true) en caso de no serlo devuleve false */
bool verificacion_de_usuario(usuario_t* usuario,char* coordenada);

/* crea un pago nuevo recibe su id, monto y codigo */
pago_t* crear_pago(size_t id, char* codigo, double monto);

/* agrega saldo al usuario, solo numeros positivos devuelve true si se pudo realizar la operacion y false si fallo */
bool pagar_saldo(usuario_t* usuario, double monto);

/* destruye el pago que recibe */
void destruir_pago(pago_t* pago);

/* verifica que el pago pertenesca al usuario, recibe el pago y el usuario devuelve true si este pago le pertenece y false si no */
bool verificar_pago(pago_t* pago, usuario_t* usuario);

/* da el pago que deve procesar */
double saldo_pago(pago_t* pago);

/* da el id del pago */
size_t identidad_del_pago (pago_t* pago);

bool aumentar_pago(pago_t* pago, double valor);

/* quita saldo del pago sin pagar */
bool pagar_pagos(pago_t* pago, double valor);

void pago_codigo(pago_t* pago,char* cod);

char* pago_ver_codigo(pago_t* pago);
#endif
