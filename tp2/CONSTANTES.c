/* CONSTANTES */
#include <stdlib.h>
const char* COM_AGREGAR_ARCHIVO = "agregar_archivo";
const char* COM_VER_VISIT = "ver_mas_visitados";
const char* COM_VER_IPS = "ver_visitantes";
//const char* COMANDOS_VALIDOS[] = {COM_AGREGAR_ARCHIVO, COM_VER_VISIT, COM_VER_IPS, NULL};
//const char* COMANDOS_VALIDOS[] = {"agregar_archivo", "ver_mas_visitados", "ver_visitantes", NULL};
const size_t CANT_COMANDOS = 4;
const size_t MAX_TAM_PARAMETROS = 3;
const size_t MIN_TAM_PARAMETROS = 2;
const size_t CANT_IP = 4;
const size_t CANT_CHAR_IP = 16;//4 numeros de 3 char + 3 separadores + \0
