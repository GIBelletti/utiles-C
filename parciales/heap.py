def posicion_padre(posicion_hijo):
	if (posicion_hijo == 0):
		return -1;
	return (int)((posicion_hijo - 1) / 2)

def posicion_hijo_der(posicion_padre):
	return (int)((posicion_padre * 2) + 2)

def posicion_hijo_izq(posicion_padre):
	return (int)((posicion_padre * 2) + 1)

def swap_vector(arr,pos1,pos2):
	arr[pos1],arr[pos2] = arr[pos2],arr[pos1]

class Heap:
	def __init__(self,comparar):
		self.cantidad = 0
		self.comparar = comparar
		self.arreglo = []

	def dar_arreglo(self):
		"""debug"""
		return self.arreglo

	def upheap(self, pos):
		padre = posicion_padre(pos)
		if(padre == -1):
			return
		elif(self.comparar(self.arreglo[pos], self.arreglo[padre]) > 0):
			swap_vector(self.arreglo, pos, padre)
			self.upheap(padre)

	def downheap(self, pos):
		while(pos < self.cantidad):
			pos_max = pos
			izq = posicion_hijo_izq(pos)
			der = posicion_hijo_der(pos)
			if(izq < self.cantidad and self.comparar(self.arreglo[izq], self.arreglo[pos_max]) > 0):
				pos_max = izq
			if(der < self.cantidad and self.comparar(self.arreglo[der], self.arreglo[pos_max]) > 0):
				pos_max = der
			if(pos_max == pos):
				return
			swap_vector(self.arreglo, pos_max, pos)
			self.downheap(pos_max)
			pos = pos_max

	def heap_cantidad(self):
		return self.cantidad

	def heap_esta_vacio(self):
		return (self.cantidad == 0)

	def heap_encolar(self, elem):
		self.arreglo.append(elem)
		self.upheap(self.cantidad)
		self.cantidad += 1
		return True

	def heap_ver_max(self):
		if(self.heap_esta_vacio()):
			return None
		return self.arreglo[0]

	def heap_desencolar(self):
		if (self.cantidad == 0):
			return None
		self.cantidad -= 1
		swap_vector(self.arreglo,0,self.cantidad)
		devolver = self.arreglo[self.cantidad]
		self.arreglo[self.cantidad] = None
		self.arreglo = self.arreglo[:self.cantidad]
		self.downheap(0)
		return devolver

	def heap_crear_arr(arreglo, n, comp):
		heap_t* heap_nuevo = Heap(comp)
		for i in range(n):
			heap_nuevo->arreglo[i] = arreglo[i]
		heap_nuevo->cantidad = n
		heap_nuevo->comparar = comp
		for j in range(posicion_padre(n - 1),0,-1)
			downheap(heap_nuevo, (size_t)pos)
		return heap_nuevo
