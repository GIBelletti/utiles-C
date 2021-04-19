import grafo_tda as GRAFO
import objetos as OBJ

def cantidad_de_dependientes(grafo,nombre):
	vertices = grafo.ver_vertices()
	if not nombre in vertices:
		return 0
	dependientes = {}
	for vertice in vertices:
		dependientes[vertice] = [0,[]]
	for v in vertices:
		for w in grafo.adyacentes(v):
			dependientes[w][0] += 1
			dependientes[w][1].append(v)
	valor = 0
	cola = OBJ.Cola()
	cola.encolar(nombre)
	visitados = []
	while not cola.esta_vacia():
		persona = cola.desencolar()
		if persona in visitados:
			continue
		visitados.append(persona)
		valor += dependientes[persona][0]
		for dep in dependientes[persona][1]:
			cola.encolar(dep)
	return valor

def main():
	grafo = GRAFO.Grafo()
	for i in range(1,10+1):
		grafo.agregar_vertice("v{}".format(i))
	for j in range(1,9+1):
		grafo.agregar_arista_dirigida("v{}".format(j),"v{}".format(j+1),5)
	print(cantidad_de_dependientes(grafo,"v10"))
	print(cantidad_de_dependientes(grafo,"v5"))
	
main()
