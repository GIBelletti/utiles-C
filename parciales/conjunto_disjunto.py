"""Se tiene un arreglo con n enteros positivos, y se desea separar todos sus elementos en dos conjuntos disjuntos,
tales que la diferencia entre la suma de los elementos de cada conjunto sea mínima.
Diseñar un algoritmo greedy para formar los dos conjuntos. Indicar el orden de la solución."""
def conjunto_disjunto(arr,n):
	if n < 2:
		return (arr,[])
	arr1 = []
	arr2 = []
	diferencia = 0
	for i in range(0,n,2):
		if (i + 1 >= n):
			if diferencia > 0:
				arr2.append(arr[i])
			else:
				arr1.append(arr[i])
			break
		diferencia += (arr[i] - arr[i+1])
		if diferencia > 0:
			arr1.append(arr[i])
			arr2.append(arr[i+1])
		else:
			arr2.append(arr[i])
			arr1.append(arr[i+1])
	return (arr1,arr2)

def imprecion():
	arr = [1,5,7,3,4,9,8]
	n = len(arr)
	a1,a2 = conjunto_disjunto(arr,n)
	print("{}\n{}".format(a1,a2))
imprecion()
