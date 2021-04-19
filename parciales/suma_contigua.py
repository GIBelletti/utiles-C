import os
"""Dado un arreglo de n números reales, se desea encontrar el valor máximo posible para la suma de una ventana
contigua. Por ejemplo, para el arreglo [3 -1 -5 6 7 -9], el valor máximo es 13, que se obtiene sumando 6 + 7.
Para resolverlo usando programación dinámica, se define M[i] como el valor máximo de ventana que se puede
obtener utilizando los primeros i elementos del arreglo. Para cada valor de i, el valor de M[i] se puede formar
extendiendo la ventana que termina en i - 1, o bien comenzando una ventana nueva en i. a) Mostrar la relación
de recurrencia. b) Mostrar paso por paso la resolución para el arreglo [-15 29 -36 3 -22 11 19 -5 0 4]. c)
¿De qué orden es el algoritmo en tiempo y memoria?"""
def maximo_suma_contigua(arr,n):
	""""""
	matriz = [0]
	matriz += arr
	maximo = arr[0]
	for i in range(1,n+1):
		if matriz[i] <= (matriz[i] + matriz[i-1]):
			matriz[i] = matriz[i] + matriz[i-1]
		maximo = max(maximo,matriz[i])
	return maximo

def imprecion():
	""""""
	#arr = [3,-1,-5,6,7,-9]
	arr = [-15,29,-36,3,-22,11,19,-5,0,4]
	n = len(arr)
	print("{}".format(maximo_suma_contigua(arr,n)))
	if -9 > 4:
		print("no")
imprecion()


