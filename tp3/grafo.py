import objetos.py


class arista_t:
	destino = ""
	peso = 0.0
	info = ""

class grafo_t:
	nodos = {}
	cant = 0

	def agregar_vertice(self, nodo):
		grafo.nodos[nodo] = []
		grafo.cant = grafo.cant + 1

	def agregar_arista(self, nodo, info_arista, peso_arista = 0.0, destino_arista):
		arista = arista_t
		arista.destino = destino_arista
		arista.peso = peso_arista
		arista_info = info_arista
		grafo.nodos[nodo].append(arista)

	def borrar_arista(self, nodo, arista):
		if arista in grafo.nodos[nodo]:
			grafo.nodos[nodo].remove(arista)
			return true
		return false

	def borrar_vertice(self, nodo):
		for x in grafo.nodos[nodo]:
			for y in grado.nodos[x.destino]:
				if y.destino == nodo:
					grafo.nodos[x.destino].remove(y)
		del grafo.nodos[nodo]

	def adyacentes(self, nodo):
		res = []
		for a in grafo.nodos[nodo]:
			res.append(a.destino)
		return res

	def son_adyacentes(self, nodo1, nodo2):
		return nodo2 in adyacentes(self, nodo1)

	def obtener_aristas(self):
		res = []
		for x in grafo.nodos:
			res + grafo.nodos[x]
		return res

	def obtener_vertices(self):
		return grafo.nodos.keys()

	def cant_vertices(self):
		return 


def grafo_crear():
	grafo = grafo_t
	return grafo

