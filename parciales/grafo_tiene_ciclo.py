def visitar(grafo,visitados,vertice,antecesor = None):
	visitados[vertice] = True
	for w in grafo.adyacentes(vertice):
		if w == antecesor:
			continue
		if visitados[w]:
			return True
		return visitar(grafo,visitados,w,vertice)
	return False

def grafo_tiene_ciclo(grafo):
	vertices = grafo.ver_vertices()
	visitados = {}
	for v in vertices:
		visitados[v] = False
	for vertice in vertices:
		if visitados[vertice]:
			continue
		if visitar(grafo,visitados,vertice):
			return True
	return False
