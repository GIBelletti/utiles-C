/* Dado un arreglo ordenado que tiene todos los elementos repetidos una vez
excepto uno que aparece una sola vez
devuelve el unico elemento que aparece sin repeticiones  */
int repetido(int* arreglo,size_t n){
	if(n == 1){
		return arreglo[1];
	}
	if(arreglo[0] != arreglo[1]){
		return arreglo[0];
	}
	return repetido(&arreglo[2],(n-2));
}
const int maximo_entero_positivo = 1073741824;
/*recibe un arreglo de n números enteros e informe los índices de dos números que estén entre sí a mínima distancia*/
int* indice_distancia(int* arreglo,size_t n){
	if (n <= 1){
		return NULL;
	}
	int distancia[3] = {maximo_entero_positivo,0,0};
	for (size_t i = (n-1), i > 0;){
		int* dist = distancia_minima(arreglo, i, arreglo[i]);
		if (dist[0] < distancia[0]){
			distancia = dist
		}
	}
	return distancia;
}
int* distancia_minima(int* arreglo,size_t n,int valor){
	int distancia[3] = {maximo_entero_positivo,0,0};
	for(size_t i = 0;i < n;i++){
		int relativo = modulo((arreglo[i] - valor));
		if(relativo < distancia[0]){
			distancia = {relativo,i,0};
		}
	}
	return distancia;
}
/* da el modulo de un entero */
int modulo(int* entero){
	if (entero < 1){
		return (entero * -1);
	}
	return entero;
}
/* recibe un arreglo de 1 y 0 y devuelve la pos del primer cero */
int pos_del_cero(int* arreglo,size_t n){
	if (n == 0){return -1;}
	if (n == 1){
		if (arreglo[0] == 0){
			return 0;
		}
		return -1;
	}
	size_t medio = n / 2;
	int izq = pos_del_cero(arreglo, medio);
	int der = pos_del_cero(&arreglo[medio], medio);
	if (izq > 0 || der > 0){
		if (izq > der){
			return izq;
		}
		return (der + n - 1);
	}
	return -1;
}
