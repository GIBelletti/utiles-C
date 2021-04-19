import heap as HEAP

class Pila_extendida:
	def __init__(self,comp):
		self.arreglo = []
		self.comp = comp
		self.minimo = HEAP.Heap(comp)
		self.tam = 0
	def apilar(self,dato):
		self.arreglo.append(dato)
		self.tam += 1
		self.minimo.heap_encolar(dato)
	def desapilar(self):
		if self.tam == 0:
			return None
		dato = self.arreglo.pop()
		self.tam -= 1
		if self.minimo.heap_ver_max() == dato:
			self.minimo.heap_desencolar()
		else:
			self.minimo.heap_crear_arr(self.arreglo,self.tam,self.comp)#cambiar heap por abb
		return dato
	def minimo(self):
		return self.minimo.heap_ver_max()
