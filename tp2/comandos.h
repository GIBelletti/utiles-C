#ifndef COMANDOS_H
#define COMANDOS_H

#include <stdbool.h>

int comparar_ip(const char* ip1, const char* ip2);

bool agregar_archivo(char* nombre_archivo, hash_t* logs, hash_t* hash_visitados , heap_t* heap_visitados, abb_t* abb, pila_t* basurero_de_split, pila_t* basurero_de_logs);

bool ver_visitados(heap_t* heap_visitados, size_t cant);

bool ver_ips(abb_t*abb, char* ip_inicial, char* ip_final);

void wrapper_log(void* dato);

#endif // COMANDOS_H
