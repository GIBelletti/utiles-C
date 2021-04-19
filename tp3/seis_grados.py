import objetos.py
import grafo.py
import csv
#Constantes
DISTANCIA_POPULAR = 2
#Constantes
#recibe un grafo y dos vertices del grafo y da la distancia que hay entre ellos (si es el mismo da [origen] y si no se conectan [])
def agregar_base_datos(nombre_archivo):
	grafo = grafo_crear()
	from info_peli import defaultdict
	peliculas = defaultdict(list)
	archivo = csv.reader(open(nombre_archivo, "rb"))
	for row in archivo:
		grafo.agregar_vertice(row[0])
		for info in row:
			if info == row[0]:
				continue
			peliculas[info].append(row[0])
	for peli in peliculas:
		for actor in peli:
			for actor_destino in peli:
				if actor_destino == actor:
					continue
				grado.agregar_arista(actor, peli, peso_arista, actor_destino)
	archivo.close
	return grafo

def camino(grafo,origen,destino):
	if origen == destino:
		return [(origen,None,None)]
	camino_invertido = []
	(padre,orden,aristas) = BFS(grafo, origen)
	if destino in orden:
		actual = destino
		while not actual == origen:
			tupla = (padre[actual],actual,aristas[actual])#vertice padre (actor anterior), vertice actual (actor), arista que los une (pelicula)
			camino_invertido.append(tupla)
			actual = padre[actual]
		return cadena_invertida.reverse()
	return []

#recibe un grafo un veritice y una distancia > 0 y devuelve una lista con los veritcers a esa distancia si distancia < 0 da [] si distancia = 0 da [vertice]
def Actores_a_distancia(grafo, origen, distancia, comparacion_de_nodos = None):
	if distancia == 0:
		return [origen]
	actores = []
	(padre,orden,aristas) = BFS(grafo, origen)
	if distancia < 0:
		for vertice in grafo.obtener_vertices():
			if vertice in orden:
				actores.append(vertice)
		return []
	for elemento in orden:
		if orden[elemento] == distancia:
			actores.append(elemento)
	return actores.sort(comparacion_de_nodos)#verificar orden

#recive un grafo y uno de sus vertices devuelve un valor numerio de suma de vertices a distancia 2 (personas con la que trabajó alguien que trabajó con él) * cantidad de vertices (cantidad de películas en las que participó)
def popularidad(grafo,actor):
	personas = Actores_a_distancia(grafo,actor,DISTANCIA_POPULAR)
	cant_de_personas = len(personas)
	peliculas = grafo.obtener_aristas(actor)
	cant_de_peliculas = len(peliculas)
	return cant_de_personas * cant_de_peliculas

#recibe un grafo uno de sus vertices y una cantidad, devuelve esa cantidad de vertices que esta a distancia 2 (si la distancia es mayor no existen vertices en comun) y que tengan la mayor cantidad de vertices en comun (caso de que la cantidad sea mayor o igual a los posibles similares los devolvera a todos, caso de que sea 0 o menor da [])
def similares(grafo,actor,cantidad):
	if cantidad <= 0:
		return []
	posibles_similares = Actores_a_distancia(grafo,actor,DISTANCIA_POPULAR)
	cant_posibles = len(posibles_similares)
	if cant_posibles >= cantidad:
		return posibles_similares
	similares = []
	auxiliar = []
	for actores in posibles_similares:
		iguales = 0
		for adyacente in grafo.adyacentes(actores):
			if adyacente == actor:
				iguales += 1
		tupla = (actores,iguales)
		auxiliar.append(tupla)
	auxiliar.sort(key=lambda auxiliar: auxiliar[1], reverse=True)
	similares = auxiliar[:cantidad]
	return similares

#FUNCIONES AUXILIARES
def BFS(grafo, origen):
	visitados = {}
	padre = {}
	orden = {}
	aristas = {}
	q = cola()
	q.encolar(origen)
	visitados[origen] = True
	orden[origen] = 0
	padre[origen] = None
	while not cola.esta_vacia():
		V = q.desencolar()
		for W in grafo.adyacentes(V):
			if W in visitados:
				continue
			visitados[W] = True
			padre[W] = V
			for aristas in grafo.adyacentes(W):
				if arista.destino == V:
					aristas[W] = aristas
					break
			orden[W] = orden[V] + 1
			q.encolar(W)
	return (padre,orden,aristas)
