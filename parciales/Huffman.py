import heap as HEAP

def comparar(a,b):
	v = a[1] - b[1]
	if v == 0:
		return 0
	if v < 0:
		return -1
	return 1

def Huffman(cadena):
	caracteres = {}
	for caracter in cadena:
		if caracter not in caracteres:
			caracteres[caracter] = 1
		else:
			caracteres[caracter] += 1
	q = HEAP.Heap(comparar)
	for c in caracteres:
		q.heap_encolar([c,caracteres[c]])
		#print("{}  {}".format(c,caracteres[c]))
	while q.heap_cantidad() > 1:
		menor_1 = q.heap_desencolar()
		menor_2 = q.heap_desencolar()
		lista = [menor_1,menor_2]
		q.heap_encolar([lista,menor_1[1] + menor_2[1]])
	return q.heap_desencolar()

def imprimir(lista):
	if len(lista[0]) < 2:
		return
	imprimir(lista[0][0])
	if len(lista[0]) == 2:
		print(lista[0][0])
		print(lista[0][1])
	imprimir(lista[0][1])
	return

def menu():
	cad = "algoritmosii"
	l = Huffman(cad)
	imprimir(l)
menu()
