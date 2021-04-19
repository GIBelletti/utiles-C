/* CONSTANTES */
#include <stdlib.h>
const char* COM_AGREGAR_ARCHIVO = "agregar_archivo";
const char* COM_VER_VISIT = "ver_mas_visitados";
const char* COM_VER_IPS = "ver_visitantes";
const char* COM_ORDENEAR_ARCHIVO = "ordenar_archivo";
const size_t CANT_COMANDOS = 4;
const size_t MAX_TAM_PARAMETROS = 3;
const size_t MIN_TAM_PARAMETROS = 2;
const size_t CANT_IP = 4;
const size_t CANT_CHAR_IP = 16;//4 numeros de 3 char + 3 separadores + \0
const size_t TAM_APROX_LINEA = 200;//cantidad de bytes aprox que tiene la linea maxima del accesslog
int MAX_TAM = 250;
const double T_DOS = 2;
const int CANT_T_DOS = 5;
