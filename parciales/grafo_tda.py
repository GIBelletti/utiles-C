class _Vertice:
	def __init__(self,identificacion ,dato = None):
		self.aristas = {}
		self.identificacion = identificacion
		self.dato = dato

	def agregar_arista(self, arista):
		self.aristas[arista.destino] = arista#solo permite una unica arista por conexion con otro vertice

	def remover_arista(self, arista):
		return self.aristas.pop(arista)

	def ver_aristas(self):
		return list(self.aristas)

	def ver_info(self):
		return self.dato

	def dar_arista(self,destino):
		return self.aristas[destino]

	def dar_nombre(self):
		return self.identificacion

	def __str__(self):
		if self.dato == None:
			return "{}".format(self.identificacion)
		return "{}:{}".format(self.identificacion,self.dato)

class _Arista:
	def __init__(self, origen, destino, peso = 1, info = None):
		self.origen = origen
		self.destino = destino
		self.peso = peso
		self.info = info

	def destino(self):
		return self.destino

	def dar_peso(self):
		return self.peso

	def dar_info(self):
		return self.info

class Grafo:
	"""
- Crear
- Destruir (si corresponde)
- Agregar/Borrar un vértice
- Agregar/Borrar una arista
- Ver si un vértice está en el grafo. 
- Si los vértices pudieran tener más información asociada, ver obtener dicha información (eso depende de la implementación, porque podría dejarse eso como algo externo). 
- Ver si dos vértices están conectados. 
- Obtener el peso de la unión de dos vértices (si fuera no pesado, podría ser 1). 
- Obtener todos los vértices
- Obtener un vértice aleatorio 
- Obtener los adyacentes a un vértice. 
- Obtener la cantidad de vértices
- Tener alguna forma de iterarlo. """
	def __init__(self):
		self.vertices = {}
#agregar
	def agregar_vertice(self,nombre,extra = None):
		self.vertices[nombre] = _Vertice(nombre,extra)
		return not self.vertices[nombre] == None

	def agregar_arista_dirigida(self,origen, destino, peso = 1,info = None):
		arista = _Arista(origen,destino,peso,info)
		self.vertices[origen].agregar_arista(arista)
		return not arista == None

	def agregar_arista(self,vertice_1, vertice_2, peso = 1,info = None):
		arista12 = _Arista(vertice_1,vertice_2,peso,info)
		arista21 = _Arista(vertice_2,vertice_1,peso,info)
		self.vertices[vertice_1].agregar_arista(arista12)
		self.vertices[vertice_2].agregar_arista(arista21)
		return (not arista12 == None) and (not arista21 == None)
#obtener
	def peso_arista(self,origen,destino):
		vertice = self.vertices[origen]
		arista = vertice.dar_arista(destino)
		return arista.dar_peso()

	def info_arista(self,origen,destino):
		vertice = self.vertices[origen]
		arista = vertice.dar_arista(destino)
		return arista.dar_info()

	def info_vertice(self,vertice):
		return self.vertices[vertice].ver_info()
#
#	def idem_vertice(self,vertice):
#		return self.vertices[vertice].dar_nombre()

	def ver_vertices(self):
		return list(self.vertices)

	def obtener_vertice_aleatorio(self):
		for a in self.vertices:
			return a#termina en la primer ejecucion haciendolo O(1)

	def adyacentes(self, vertice):
		return self.vertices[vertice].ver_aristas()
#borrar
	def borrar_arista_dirigida(self, origen, destino):
		return self.vertices[origen].remover_arista(destino)

	def borrar_arista_no_dirigida(self, vertice_1, vertice_2):
		a = self.borrar_arista_dirigida(vertice_1, vertice_2)
		b = self.borrar_arista_dirigida(vertice_2, vertice_1)
		return (not a == None and not b == None)

	def borrar_arista(self, vertice_1, vertice_2):
		if (vertice_1 in self.adyacentes(vertice_2) and vertice_2 in self.adyacentes(vertice_1)):
			return self.borrar_arista_no_dirigida(vertice_1, vertice_2)
		if vertice_1 in self.adyacentes(vertice_2):
			return (self.borrar_arista_dirigida(vertice_2, vertice_1) != None)
		return (self.borrar_arista_dirigida(vertice_1, vertice_2) != None)

	def borrar_vertice(self,vertice):
		adyacente = self.adyacentes(vertice)
		for v in adyacente:
			self.borrar_arista(v,vertice)
		self.vertices.pop(vertice)
		return True
#verificacione
	def vertice_esta_en_grafo(self,vertice):
		return vertice in self.vertices
