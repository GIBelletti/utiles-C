#include "tp0.h"

/* ******************************************************************
 *                     FUNCIONES A COMPLETAR
 * *****************************************************************/

// Alumno: Belletti Gabriel Ignacio. Padron: 100053.

/* swap() intercambia dos valores enteros.
 */
void swap (int* x, int* y) {
	int auxiliar = *x;
	*x = *y;
	*y = auxiliar;
	return;
}

/* maximo() busca el mayor elemento del arreglo y devuelve su posicion.
 * Si el vector es de largo 0, devuelve -1.
 */
int maximo(int vector[], int n) {
	if (n == 0){
		return -1;
	}
	int pos_maximo = 0;
	//la posicion del maximo empieza desde 0 asi que comparo a partir de 1
	for (int i = 1; i < n; i++){
		if (vector[i] > vector[pos_maximo]){
			pos_maximo = i;
		}
	}
	return pos_maximo;
}

/* La función comparar recibe dos vectores y sus respectivas longitudes
 * y devuelve -1 si el primer vector es menor que el segundo; 0 si son
 * iguales; y 1 si el segundo es menor.
 *
 * Un vector es menor a otro cuando al compararlos elemento a elemento,
 * el primer elemento en el que difieren no existe o es menor.
 */
int comparar(int vector1[], int n1, int vector2[], int n2) {
	int minimo = n1<=n2? n1:n2;
	for (int i = 0;i < minimo;i++){
		if (vector1[i] != vector2[i]){
			if (vector1[i] < vector2[i]){
				return -1;
			}
			return 1;
		}
	}
	if (n1 != n2){
		if (n1 < n2){
			return -1;
		}
		return 1;
	}
	return 0;
}

/* selection_sort() ordena el arreglo recibido mediante el algoritmo de
 * selección.
 */
void seleccion(int vector[], int n) {
	//vector de 0 o 1 elemento ya estan ordenados
	for (int i = n-1; i > 0; i--){
		int pos_max = maximo(vector,i+1);
		swap(&vector[i],&vector[pos_max]);
	}
	return;
}
