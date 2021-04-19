#ifndef COMANDOS_H
#define COMANDOS_H

#include <stdbool.h>

int comparar_ip(const char* ip1, const char* ip2);

bool agregar_archivo(char* nombre_archivo, abb_t* abb);

bool ver_visitados(heap_t* heap_visitados, size_t cant);

bool ver_ips(abb_t*abb, char* ip_inicial, char* ip_final);

bool ordenar_archivo(char* archivo_entrante, char* archivo_saliente, size_t numero);//verificar

void wrapper_log(void* dato);

#endif // COMANDOS_H
