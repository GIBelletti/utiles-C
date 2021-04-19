/* CONSTANTES */
const size_t TAM_BASE = 20;

struct heap{
	size_t cantidad;
	size_t cant_max;
	void* arreglo[];
	cmp_func_t comparar;//???
}

/*typedef int (*cmp_func_t) (const void *a, const void *b);

menor a 0 si a < b
0 si a == b
mayor a 0 si a > b

*/

/* cambia las pociciones de dos punteros en un arreglo */
void swap_vector(void* vector,size_t pos_1, size_t pos_2){
	void* algo = vector[pos_1];
	vector[pos_1] = vector[pos_2];
	vector[pos_2] = algo;
	return;
}

size_t posicion_padre(size_t posicion_hijo){
	if (posicion_hijo == 0){return NULL;}
	return ((posicion_hijo - 1) / 2)
}

size_t posicion_hijo_der(size_t posicion_padre){
	return ((posicion_padre * 2) + 2);
}

size_t posicion_hijo_izq(size_t posicion_padre){
	return ((posicion_padre * 2) + 1);
}

bool upheap(heap_t* heap,size_t pos){
}

bool downheap(heap_t* heap,size_t pos){
}

heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap_nuevo = malloc(sizeof(heap_t));
	if (heap_nuevo == NULL){return NULL;}
	heap_nuevo->arreglo = malloc(sizeof(void*) * TAM_BASE);
	if (heap_nuevo->arreglo == NULL){return NULL;}
	heap_nuevo->cantidad = 0;
	heap_nuevo->comparar = cmp;
	heap_nuevo->cant_max = TAM_BASE;
	return heap_nuevo;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap_nuevo = malloc(sizeof(heap_t));
	if (heap_nuevo == NULL){return NULL;}
	heap_nuevo->arreglo = arreglo;
	heap_nuevo->cantidad = n;
	heap_nuevo->cant_max = n;
	heap_nuevo->comparar = cmp;
	size_t pos = posicion_padre(n);//toma +- desde el ultimo "nodo" con al menos un hijo
	while (pos > 0){
		downheap(heap_nuevo,pos);
		pos--;
	}
	downheap(heap_nuevo->arreglo[0];
	return heap_nuevo;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return (heap->cantidad == 0);
}

bool heap_encolar(heap_t *heap, void *elem){
	heap->arreglo[heap->cantidad] = elem;
	upheap(heap,heap->cantidad);
	heap->cantidad++;
	return true;
}

void *heap_ver_max(const heap_t *heap){
	return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap){
	if (heap->cantidad == 0){return NULL;}
	hash->cantidad--;
	swap_vector(heap->arreglo,0,heap->cantidad);//ya se le resto uno antes
	void* devolver = heap->arreglo[heap->cantidad];
	heap->arreglo[heap->cantidad] = NULL;
	downheap(hash,0);
	return devolver;
}

/* FUNCION DE ORDENAMIENTO */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
}
//////////////////////////////////////////////////
