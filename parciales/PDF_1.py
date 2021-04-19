def crar_matriz(n,m,nulo):
	matriz = []
	for i in range(n):
		fila = []
		for j in range(m):
			fila.append(nulo)
		matriz.append(fila)
	return matriz
			

def optimo(total,lista):
	#borde len(lsita) == 0, total 0, len(lista) == 1
	#elem = [productos,costes,ganacia]
	nulo = [[],0,0]
	n = len(lista)

	matriz = crar_matriz(total + 1,n,nulo)
	for i in range(1,total + 1):
		for j in range(n):
			coste = lista[j][1] + matriz[i - 1][j][1]
		#	print("i {} cos {}".format(coste ,i))
			if coste > i:
				matriz[i][j] = matriz[i-1][j]
				continue
			ganancia = lista[j][2] + matriz[i-1][j-1][2]
			if ganancia > matriz[i-1][j][2]:
				productos = lista[j][0][:] + matriz[coste][j][0][:]
				elemento = [productos,coste,ganancia]
				matriz[i][j] = elemento
			else:
				matriz[i][j] = matriz[i][j-1]
	for i in matriz:
		print(i)
	return matriz[-1][-1]
			
def menu():
	elem = [[["a"],1,3],[["b"],3,23],[["c"],1,9]]
	tot = 4
	print(optimo(tot,elem))

menu()
