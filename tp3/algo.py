def es_conexo_recursivo(grafo,visitados,v)
	visitados[v] = True
	for w in grafo.adyacentes(v)
		if visitados[w] == True
			continue
		es_conexo_recursivo(grafo,visitados,w)
	return

def es_conexo(grafo)
	visitados = {}
	for vertices in grafo.vertices()
		visitados[vertices] = False
	v = grafo.obtener_verice_aleatorio()
	es_conexo_recursivo(grafo,visitados,v)
	for visita in visitados
		if not visita
			return False
	return True
