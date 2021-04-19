#ifndef USUARIOS_H
#define USUARIOS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*agregará a la cola de procesamiento la transacción. El código se manifiesta en una sola línea, separando las distintas instrucciones con el caracter punto y coma (;). El monto es siempre positivo. (caso de ser negatvo no agregara nada a la cola de procesaeinto)*/
bool agregar_pago (pila_t* pila,lista_t* usuario,lista_t* pagos,cola_t* nada);

/* imprime (stdout) la cantidad y monto total de los pagos sin procesar. */
bool pagos_pendientes(pila_t* nada,lista_t* usuario,lista_t* pagos,cola_t* nada);

/* procesa hasta un número no negativo de pagos pendientes. (si el numero es negativo o cero no procesa ningun pago pendiente)*/
bool procesar(pila_t* pila,lista_t* usuario,lista_t* pagos,cola_t* pendientes);

/* guarda en un archivo el estado actual de las cuentas. Si no existe lo debe crear, y si existe lo sobreescribe. */
bool guardar_cuentas (pila_t* pila,lista_t* usuario,lista_t* pagos,cola_t* nada);

/* finaliza el programa (si quedan pagos sin procesar, no se procesan).*/
void finalizar();

/* FUNCIONES DEL PROGRAMA */
/* desapila dos elementos y revisa que las coordenadas sean apropiadas para el usuario. */
bool validar_usuario(pila_t* pila,lista_t* usuarios,lista_t* pagos,cola_t* nada);

/* desapila dos elementos y revisa que el usuario tenga fondos suficientes para hacer la operación. */
bool validar_pago(pila_t* pila,lista_t* usuarios,lista_t* pagos,cola_t* nada);

/* desapila tres elementos, el movimiento de fondos lo hace el procesar. */
bool pagar(pila_t* pila,lista_t* usuarios,lista_t* pagos,cola_t* pendientes);

/* crea un texto con los datos de los usuarios (se debe liberar memoria despues de usar) */
char* crear_datos(lista_t* lista);

/* remplaza el \n final del fgetline por el \0 de fin de linea */
void remplazar_chr13(char* cadena);

/* recibe un arreglo (cuyo ultimo elemento es NULL) y un numero (mayor que 0) y devuelve true si la cantidad de elementos (sin incluir el NULL) es igual al numero, caso contrario da false */
bool verificacion_de_tamaño(void* arreglo, size_t tam);

/* recive un puntero generico y un mensaje si el puntero es NULL imprime en error el mensaje y devuleve false, caso de que no sea */
bool verificacion_de_existencia(void* existe, char* mensaje);

/* recibe el comando a ejecutar la pila de procesos,un dato auxiliar que puede o no ser NULL (solo se usa en guardar_cuentas y procesar devido a que sus valores estan "invertidos" en la pila) y la luista de usuarios, devuelve false si hubo algun error */
bool divicion_de_procesos(pila_t* proceso,size_t* comando,pila_t* auxiliar,lista_t* usuarios);

/* recive la cola (y la lista) con los procesos a realizar y ejecuta los comandos */
bool ejecutar(cola_t* procesamientos,lista_t* usuarios);

/* recibe la cola y lista y ejecuta la interfaz, devuelve true si no hay problema false si hubo error */
bool interfaz(cola_t* procesamientos,lista_t* usuarios);

/* recibe un archivo abierto y una lista y pasa los datos del archivo a la lista en caso de error cierra el archivo */
bool archivo(FILE* usuarios,lista_t* lista_de_usuarios);


#endif
