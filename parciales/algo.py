"""En este barrio todos viven alrededor de la plaza y los vecinos contiguos se odian entre sí. Hay N vecinos y el
primer vecino es contiguo con el último. Cuando le preguntan a cada uno cuánto es lo máximo que va a donar
para arreglar la plaza la respuesta se registra en el array donar = [d0, d1, ..., d(N-1)], pero la condición es:
solo donaré si no dona ninguno de mis vecinos contiguos. Usando programación dinámica resolver cuánto es lo
máximo que se puede recolectar por donaciones.
Nota: Lo resolveremos considerando que el primer vecino y el último no son contiguos. En caso
de sobrar tiempo resolveremos el caso circular"""
def max_donacion(arr):
	n = len(arr)
	donaciones = [0]
	donaciones += arr
	donaciones += [0]
	maximo = 0
	for i in range(2,n+2):
		donaciones[i] = max(donaciones[i-1],donaciones[i] + donaciones[i-2])
		if maximo < donaciones[i]:
			maximo = donaciones[i]
	return maximo

def menu():
	arr = [2,4,3,1,8,4,1,2]
	print("{}".format(max_donacion(arr)))
menu()
