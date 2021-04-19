import grafo_tda as GRAFO
import heap as HEAP
import objetos as OBJ

#####CONSTANTES#####
INFINITO = -1#usado para la distancia
#####CONSTANTES#####
#####COMPARAR#####
def comparar_peso(a1,a2):
	v = a1[1] - a2[1]
	if v == 0:
		return 0
	if v < 0:
		return -1
	return 1

def comparar_peso_invertido(a1,a2):
	return comparar_peso(a2,a1)
#####COMPARAR#####
#####FUNCIONES AUXILIARES#####
def adyacente_de_menor_coste(grafo,vertice,excluidos,visitados):
	menor = None
	for w in grafo.adyacentes(vertice):
		if visitados[w] or w in excluidos:
			continue
		if menor == None:
			menor = w
		elif grafo.peso_arista(vertice,w) < grafo.peso_arista(vertice,menor):
			menor = w
	return menor

def seguir_camino(desde,hasta,padre):
	#print("{}".format(list(padre)))#debug
	resultado_invertido = []
	resultado = []
	i = hasta
	while not i == desde:
		resultado_invertido.append(i)
		i = padre[i]
		#print("{}".format(i))#debug
	resultado_invertido.append(desde)
	for a in range(len(resultado_invertido)-1,0-1,-1):
		resultado.append(resultado_invertido[a])
	return resultado

def peso_total(grafo):
	peso = 0
	for vertice in grafo.ver_vertices():
		for ady in grafo.adyacentes(vertice):
			peso += grafo.peso_arista(vertice,ady)
	return peso

def viajante_base(grafo, origen, optimo):
	""""""
	camino = [origen]
	coste = 0
	visitados = {}
	vertices = grafo.ver_vertices()
	for v in vertices:
		visitados[v] = False
	visitados[origen] = True
	resultado = [[origen],coste]
	if optimo:
		viajante_recursivo(grafo, origen, origen, visitados, camino, coste, resultado, optimo, len(vertices))
	else:
		viajante_recursivo_no_optimo(grafo, origen, origen, visitados, camino, coste, resultado, optimo, len(vertices))
	resultado[0].append(origen)
	return resultado

def viajante_recursivo_no_optimo(grafo, origen, ultimo, visitados, camino, peso, resultado, optimo, tam):
	if len(camino) >= tam:#condicion de corte (fuerza bruta)
		if not origen in grafo.adyacentes(ultimo):
			return False
		peso_utimo = grafo.peso_arista(camino[-1],camino[0])
		resultado[0] = camino
		resultado[1] = (peso + peso_utimo)
		return True
	excluido = []
	for i in range(len(grafo.adyacentes(ultimo))):
		ady = adyacente_de_menor_coste(grafo,ultimo,excluido,visitados)
		if ady == None:
			print("Falla")
			continue
	#	print("Ok")
		peso_aux = grafo.peso_arista(ultimo,ady)
		camino.append(ady)
	#	print("{}".format(peso_aux))
		peso += peso_aux
	#	print("{}".format(camino))
		if viajante_recursivo(grafo, origen, ady, visitados, camino, peso, resultado, optimo, tam):
			return True
		peso -= peso_aux
		camino.pop()
		excluido.append(ady)
	#	print("{}".format(excluido))
	return False

def viajante_recursivo(grafo, origen, ultimo, visitados, camino, peso, resultado, optimo, tam):
	if len(camino) >= tam:#condicion de corte (fuerza bruta)
		if not origen in grafo.adyacentes(ultimo):
			return False
		peso_utimo = grafo.peso_arista(camino[-1],camino[0])
	#	if not optimo:
	#		resultado[0] = camino
	#		resultado[1] = (peso + peso_utimo)
	#		return True
		if (peso + peso_utimo) < resultado[1] or resultado[1] == 0:
			resultado[0] = camino[:]
			resultado[1] = (peso + peso_utimo)
	#		print("resul0 = {} resul1 = {}".format(resultado[0],resultado[1]))
	#	print("peso = {} resul = {}".format(peso,resultado[1]))
		return False
#	if not optimo:
#		excluido = []
#		for i in range(len(grafo.adyacentes(ultimo))):
#			ady = adyacente_de_menor_coste(grafo,ultimo,excluido,visitados)
#			if ady == None:
#				return False
#			camino.append(ady)
#			peso_aux = grafo.peso_arista(ultimo,ady)
#			print("{}".format(peso_aux))
#			peso += peso_aux
#			print("{}".format(camino))
#			if viajante_recursivo(grafo, origen, ady, visitados, camino, peso, resultado, optimo, tam):
#				return True
#			peso -= peso_aux
#			camino.pop()
#			excluido.append(ady)
#			print("{}".format(excluido))
#		return False
	###
	if peso >= resultado[1] and not resultado[1] == 0:#condicion de corte (backtracking)
		return False
	visitados[ultimo] = True
	for w in grafo.adyacentes(ultimo):
	#	print("w = {} ult ={}".format(w,ultimo))
		if not visitados[w]:
			camino.append(w)
			peso += grafo.peso_arista(ultimo,w)
			if viajante_recursivo(grafo, origen, w, visitados, camino, peso, resultado, optimo, tam):
				return True
			peso -= grafo.peso_arista(ultimo,w)
			visitados[w] = False
			camino.pop()
		else:
			continue
	#print("resul0 = {} resul1 = {}".format(resultado[0],resultado[1]))
	return False

def grafo_copiar_vertices(grafo_original):
	grafo = GRAFO.Grafo()
	for i in grafo_original.ver_vertices():
	#	identidad = grafo.idem_vertice(i)
	#	grafo.agregar_vertice(identidad)
		grafo.agregar_vertice(i)
	return grafo

def calcular_peso(grafo,viaje):
	peso = 0
	for i in range(len(viaje) - 1):
		if not viaje[i + 1] in grafo.adyacentes(viaje[i]):
			return -1
		peso += grafo.peso_arista(viaje[i],viaje[i + 1])
	return peso

def cant_aristas(grafo):
	cant = 0
	visitados = {}
	for v in grafo.ver_vertices():
		visitados[v] = False
	for V in grafo.ver_vertices():
		visitados[V] = True
		for W in grafo.adyacentes(V):
			if not visitados[W]:
				cant += 1
	return cant

def peso_total(grafo,dirigido = False):
	peso = 0
	for vertice in grafo.ver_vertices():
		for ady in grafo.adyacentes(vertice):
			peso += grafo.peso_arista(vertice,ady)
	if dirigido:
		peso /= 2
	return peso
#####FUNCIONES AUXILIARES#####
#####COMANDOS#####

def camino_minimo(grafo, desde, hasta):
	"""que nos devuelva una lista con el camino mínimo entre ese par de sedes. Ejemplo: `camino_minimo(rusia, ‘Moscu’, ‘Saransk’) -> [‘Moscu’, ‘Samara’, ‘Saransk’]"""
	distancia = {}
	padre = {}
	visitado = {}
	for V in grafo.ver_vertices():
		distancia[V] = INFINITO
	distancia[desde] = 0
	padre[desde] = None
	q = HEAP.Heap(comparar_peso_invertido)
	q.heap_encolar([desde,distancia[desde]])
	visitado[desde] = True
	while not q.heap_esta_vacio():
		v,p = q.heap_desencolar()
		for w in grafo.adyacentes(v):
			if w in visitado:
				continue
			peso = grafo.peso_arista(v,w)
			if (distancia[v] + peso < distancia[w] or distancia[w] == INFINITO):
				padre[w] = v
				distancia[w] = distancia[v] + peso
				q.heap_encolar([w,distancia[w]])
		#		if w == hasta:
		#			return seguir_camino(desde,hasta,padre)
	return seguir_camino(desde,hasta,padre)
	

def viajante(grafo, origen):
	"""que nos devuelva una lista con el recorrido a hacer para resolver de forma óptima el problema del viajante. La lista debe tener el mismo formato que camino_minimo."""
	return viajante_base(grafo, origen, True)

def viajante_aproximado(grafo, origen):
	"""idem anterior, pero de forma aproximada, siendo este mucho más rápido."""
	return viajante_base(grafo, origen, False)

def orden_topologico(grafo):
	"""que nos devuelva una lista con un orden topológico del grafo. En caso de no existir, devolver None, NULL, o el equivalente en el lenguaje elegido. Notar que para esta función, el grafo recibido debe ser dirigido."""
	orden = []
	q = OBJ.Cola()
	nesesarios = {}
	for vertice in grafo.ver_vertices():
		nesesarios[vertice] = 0
	for v in grafo.ver_vertices():
		for w in grafo.adyacentes(v):
			nesesarios[w] += 1
	for vertice in grafo.ver_vertices():
		if nesesarios[vertice] == 0:
			q.encolar(vertice)
	while not q.esta_vacia():
		V = q.desencolar()
		for W in grafo.adyacentes(V):
			nesesarios[W] -= 1
			if nesesarios[W] == 0:
				q.encolar(W)
		orden.append(V)
	return orden

def arbol_tendido_minimo(grafo):
	"""recibe un grafo (que se puede asumir conexo) y devuelve un nuevo grafo, que representa un árbol de tendido mínimo del original."""
	inicio = grafo.obtener_vertice_aleatorio()
	visitados = {}
	grafo_nuevo = grafo_copiar_vertices(grafo)
	q = HEAP.Heap(comparar_peso_invertido)#crear heap
	for w in grafo.adyacentes(inicio):
		peso = grafo.peso_arista(inicio,w)
		arr = [inicio,peso,w]
		q.heap_encolar(arr)#(vertice,valor que compara,vertice)
	visitados[inicio] = True
	while (not q.heap_esta_vacio()) and (not len(visitados) == len(grafo.ver_vertices())):
		V,P,W = q.heap_desencolar()
		if W in visitados:
			continue
		visitados[W] = True
		for U in grafo.adyacentes(W):
			peso = grafo.peso_arista(W,U)
			q.heap_encolar([W,peso,U])
		grafo_nuevo.agregar_arista(V,W,P)
	return grafo_nuevo

#####COMANDOS#####
