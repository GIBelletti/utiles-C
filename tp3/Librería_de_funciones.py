import objetos.py
#Constantes
DISTANCIA_POPULAR = 2
#Constantes
#recibe un grafo y dos vertices del grafo y da la distancia que hay entre ellos (si es el mismo da [origen] y si no se conectan [])
def camino(grafo,origen,destino):
	if origen == destino:
		return [origen]
	camino_invertido = []
	camino = []
	(padre,orden) = BFS(grafo, origen)
	if destino in orden:
		actual = destino
		while not actual == None:
			camino_invertido.append(padre[actual])
			actual = padre[actual]
		return cadena_invertida.reverse()
	return []

#recibe un grafo un veritice y una distancia > 0 y devuelve una lista con los veritcers a esa distancia si distancia < 0 da [] si distancia = 0 da [vertice]
def Actores_a_distancia(grafo, origen, distancia):
	if distancia < 0:
		return []
	if distancia == 0:
		return [origen]
	actores = []
	(padre,orden) = BFS(grafo, origen)
	for elemento in orden:
		if orden[elemento] == distancia:
			actores.append(elemento)
	return actores

#recive un grafo y uno de sus vertices devuelve un valor numerio de suma de vertices a distancia 2 (personas con la que trabajó alguien que trabajó con él) * cantidad de vertices (cantidad de películas en las que participó)
def popularidad(grafo,actor):
	personas = Actores_a_distancia(grafo,actor,DISTANCIA_POPULAR)
	cant_de_personas = len(personas)
	peliculas = grafo.obtener aristas(actor)
	cant_de_peliculas = len(peliculas)
	return cant_de_personas * cant_de_peliculas

#recibe un grafo uno de sus vertices y una cantidad, devuelve esa cantidad de vertices que esta a distancia 2 (si la distancia es mayor no existen vertices en comun) y que tengan la mayor cantidad de vertices en comun (caso de que la cantidad sea mayor o igual a los posibles similares los devolvera a todos, caso de que sea 0 o menor da [])
def similares(grafo,actor,cantidad):
	if cantidad <= 0:
		return []
	posibles_similares = Actores_a_distancia(grafo,actor,DISTANCIA_POPULAR)
	cant_posibles = len(posibles_similares)
	if cant_posibles <= cantidad:
		return posibles_similares

#FUNCIONES AUXILIARES
def BFS(grafo, origen);
	visitados = {}
	padre = {}
	orden = {}
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
			orden[W] = orden[V] + 1
			q.encolar(W)
	return (padre,orden)
