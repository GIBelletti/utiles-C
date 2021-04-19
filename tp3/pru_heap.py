import os
import heap as HEAP

def DEFAULT(a, b):
	if(a > b):
		return 1
	if(a == b):
		return 0
	return -1

def print_test(men,b):
	if b:
		print("ok... {}".format(men))
	else:
		print("no ok... {}".format(men))

def pru_heap():
	valor1 = 3
	valor2 = 2
	valor3 = 20
	valor4 = 6
	valor5 = -21
	valor6 = 99
	valor7 = 76
	valor8 = 34
	valor9 = -31
	valor10 = 0;
	heap = HEAP.Heap(DEFAULT)
	print_test("Prueba heap crear heap", heap)
	print_test("Prueba heap es heap", type(heap) == HEAP.Heap)
	print_test("Prueba heap desencolar es vacio", heap.heap_desencolar() == None)
	print_test("Prueba heap esta vacio es true", heap.heap_esta_vacio())
	print_test("Prueba heap la cantidad de elementos es 0", heap.heap_cantidad() == 0)
	print_test("Prueba heap ver maximo es NULL", heap.heap_ver_max() == None)
	print_test("Prueba heap desencolar es NULL", heap.heap_desencolar() == None)
	print_test("Prueba heap insertar valor1", heap.heap_encolar(valor1))
	print_test("Prueba heap insertar valor2", heap.heap_encolar(valor2))
	print_test("Prueba heap insertar valor3", heap.heap_encolar(valor3))
	print_test("Prueba heap insertar valor4", heap.heap_encolar(valor4))
	print_test("Prueba heap insertar valor5", heap.heap_encolar(valor5))
	print_test("Prueba heap insertar valor6", heap.heap_encolar(valor6))
	print_test("Prueba heap insertar valor7", heap.heap_encolar(valor7))
	print_test("Prueba heap insertar valor8", heap.heap_encolar(valor8))
	print_test("Prueba heap insertar valor9", heap.heap_encolar(valor9))
	print_test("Prueba heap insertar valor10", heap.heap_encolar(valor10))
	for a in heap.arreglo:
		print("{}".format(a))
	print_test("Prueba heap la cantidad de elementos es 10", heap.heap_cantidad() == 10)
	print_test("Prueba heap ver maximo es valor6", heap.heap_ver_max() == valor6)
	print_test("Prueba heap desencolar valor6", heap.heap_desencolar() == valor6)
	print_test("Prueba heap desencolar valor7", heap.heap_desencolar() == valor7)
	print_test("Prueba heap arreglo", len(heap.arreglo) == 8)
	for a in heap.arreglo:
		print("{}".format(a))

pru_heap()
