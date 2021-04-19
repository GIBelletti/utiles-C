import os
import grafo_tda as GRAFO
import funciones_grafo as FUNG

def print_test(men,b):
	if b:
		print("ok... {}".format(men))
	else:
		print("ERROR... {}".format(men))

def peso_total(grafo):
	peso = 0
	for vertice in grafo.ver_vertices():
		for ady in grafo.adyacentes(vertice):
			peso += grafo.peso_arista(vertice,ady)
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

def pruevas_grafo_no_dirigido():
	grafo = GRAFO.Grafo()
	for i in range(1,10+1):
		grafo.agregar_vertice("v{}".format(i))
	for j in range(1,9+1):
		grafo.agregar_arista("v{}".format(j),"v{}".format(j+1),5)
	grafo.agregar_arista("v10","v1",5)
	grafo.agregar_arista("v5","v1",3)
	grafo.agregar_arista("v7","v3",8)
	grafo.agregar_arista("v8","v2",4)
	grafo_minimo = FUNG.arbol_tendido_minimo(grafo)
	print_test("Prueba arbol de tendido minimo", not grafo_minimo == None)
	print_test("Prueba arbol de tendido minimo cantidad de vertices", len(grafo_minimo.ver_vertices()) == len(grafo.ver_vertices()))
	print_test("Prueba arbol de tendido minimo menor peso total", peso_total(grafo_minimo) < peso_total(grafo))
	print_test("Prueba arbol de tendido minimo cantidad de aristas |V| - 1", cant_aristas(grafo_minimo) == (len(grafo_minimo.ver_vertices()) - 1))
	print_test("Prueba arbol de tendido minimo pertenece arista menor", grafo_minimo.peso_arista("v5","v1") == 3)
	print_test("Prueba camino minimo de 8 a 3", FUNG.camino_minimo(grafo,"v8","v3") == ["v8","v2","v3"])
	print_test("Prueba camino minimo de 1 a 4", FUNG.camino_minimo(grafo,"v1","v4") == ["v1","v5","v4"])
	print_test("Prueba camino minimo de 1 a 8", FUNG.camino_minimo(grafo,"v1","v8") == ["v1","v2","v8"])
	print_test("Prueba camino minimo de 10 a 8", FUNG.camino_minimo(grafo,"v10","v8") == ["v10","v9","v8"])
	grafo.agregar_arista("v6","v9",0)
	grafo.agregar_arista("v6","v10",0)
	viaje_aprox = FUNG.viajante_aproximado(grafo,"v1")
	print_test("Prueba grafo problema de viajero aprox tamaño", len(viaje_aprox[0]) == len(grafo.ver_vertices()) + 1)
	print("aprox = {}, peso = {}".format(viaje_aprox[0],viaje_aprox[1]))
	viaje_completo = FUNG.viajante(grafo,"v1")
	print_test("Prueba grafo problema de viajero completo tamaño", len(viaje_completo[0]) == len(grafo.ver_vertices()) + 1)
	print("compl = {}, peso = {}".format(viaje_completo[0],viaje_completo[1]))

def pruevas_grafo_dirigido():
	grafo = GRAFO.Grafo()
	for i in range(1,10+1):
		grafo.agregar_vertice("v{}".format(i))
	for j in range(1,4+1):
		grafo.agregar_arista_dirigida("v{}".format(j),"v{}".format(j+1),5)
		grafo.agregar_arista_dirigida("v{}".format(j+5),"v{}".format(j+6),3)
	grafo.agregar_arista_dirigida("v8","v5",4)
	grafo.agregar_arista_dirigida("v4","v10",2)
	grafo.agregar_arista_dirigida("v1","v6",1)
	grafo.agregar_arista_dirigida("v10","v5",8)
	orden = FUNG.orden_topologico(grafo)
	print_test("Prueba orden topologico primero", orden[0] == "v1")
	print_test("Prueba orden topologico ultimo", orden[-1] == "v5")

def pruevas():
	pruevas_grafo_dirigido()
	pruevas_grafo_no_dirigido()

pruevas()
