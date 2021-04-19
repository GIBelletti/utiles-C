import os
import grafo_tda as GRAFO

def print_test(men,b):
	if b:
		print("ok... {}".format(men))
	else:
		print("ERROR... {}".format(men))


def pru_grafo():
	grafo = GRAFO.Grafo()
	print_test("Prueba grafo crear grafo", grafo)
	print_test("Prueba grafo es grafo", type(grafo) == GRAFO.Grafo)
	print_test("Prueba grafo ver vertice", grafo.ver_vertices() == [])
	print_test("Prueba grafo agregar vertice", grafo.agregar_vertice("v1",100))
	print_test("Prueba grafo ver vertice clase", type(grafo.ver_vertices()) == list)
	print_test("Prueba grafo ver vertice cantidad", len(grafo.ver_vertices()) == 1)
	vertice = grafo.obtener_vertice_aleatorio()
	print_test("Prueba grafo ver vertice aleatorio", not vertice == None)
	print_test("Prueba grafo ver adyacente tipo", type(grafo.adyacentes(vertice)) == list)
	print_test("Prueba grafo ver adyacente sin adyacentes", len(grafo.adyacentes(vertice)) == 0)
	print_test("Prueba grafo ver info adicional del vertice", grafo.info_vertice(vertice) == 100)
	print_test("Prueba grafo agregar vertice 2", grafo.agregar_vertice("v2",50))
	print_test("Prueba grafo cantidad de vertices 2", len(grafo.ver_vertices()) == 2)
	print_test("Prueba grafo agregar vertice 3", grafo.agregar_vertice("v3",10))
	print_test("Prueba grafo agregar vertice 4", grafo.agregar_vertice("v4",80))
	print_test("Prueba grafo agregar arista no dirigida con info", grafo.agregar_arista("v1","v2",50,"hola"))
	print_test("Prueba grafo ver adyacente con adyacentes", len(grafo.adyacentes(vertice)) == 1)
	print_test("Prueba grafo ver peso tipo", type(grafo.peso_arista("v1","v2")) == int)
	print_test("Prueba grafo ver peso arista de 1 a 2", grafo.peso_arista("v1","v2") == 50)
	print_test("Prueba grafo ver peso arista de 2 a 1", grafo.peso_arista("v2","v1") == 50)
	print_test("Prueba grafo ver info arista", grafo.info_arista("v1","v2") == "hola")
	vertice2 = grafo.adyacentes(vertice)[0]
	print_test("Prueba grafo agregar arista dirigida con info", grafo.agregar_arista_dirigida("v1","v3",10,"l"))
	print_test("Prueba grafo ver adyacente con adyacentes", len(grafo.adyacentes(vertice)) == 2)
	print_test("Prueba grafo borrar vertice", grafo.borrar_vertice(vertice2))
	print_test("Prueba grafo ver adyacente con adyacentes", len(grafo.adyacentes(vertice)) == 1)
	print_test("Prueba grafo ver adyacente sin adyacentes", len(grafo.adyacentes("v3")) == 0)
	print_test("Prueba grafo ver adyacente sin adyacentes", len(grafo.adyacentes("v4")) == 0)
	print_test("Prueba grafo agregar arista dirigida con info", grafo.agregar_arista_dirigida("v3","v1",40,"s"))
	print_test("Prueba grafo ver peso arista dirigida de 1 a 3", grafo.peso_arista("v1","v3") == 10)
	print_test("Prueba grafo ver peso arista dirigida de 3 a 1", grafo.peso_arista("v3","v1") == 40)
	print_test("Prueba grafo ver info arista dirigida de 1 a 3", grafo.info_arista("v1","v3") == "l")
	print_test("Prueba grafo ver info arista dirigida de 3 a 1", grafo.info_arista("v3","v1") == "s")
	print_test("Prueba grafo ver adyacente 1", len(grafo.adyacentes("v1")) == 1)
	print_test("Prueba grafo ver adyacente 3", len(grafo.adyacentes("v3")) == 1)
	print_test("Prueba grafo borrar arista dirijida", grafo.borrar_arista_dirigida("v1","v3"))
	print_test("Prueba grafo ver adyacente 1", len(grafo.adyacentes("v1")) == 0)
	print_test("Prueba grafo ver adyacente 3", len(grafo.adyacentes("v3")) == 1)
	print_test("Prueba grafo vertice esta en el grafo", grafo.vertice_esta_en_grafo("v4"))

pru_grafo()
