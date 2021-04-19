int pos_padre(int n){
	return (n - 1)/2;
}

int pos_hijo_izq(int n){
	return (n*2)+1;
}

int pos_hijo_der(int n){
	return (n*2)+2;
}

bool es_heap_max(int* arr, int n){
	for (int i; i < n/2; i++){
		if (arr[i] < arr[pos_hijo_der(i)] || arr[i] < arr[pos_hijo_izq(i)]){
			return false;
		}
	}
	return true;
}
