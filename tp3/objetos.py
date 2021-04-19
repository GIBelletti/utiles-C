class _Nodo:
	"""clase privada Nodo en la cual se almacena informacion, para usar en otras clases"""
	def __init__(self,dato,proximo = None):
		"""constructor de la clase nodo la cual recibe un valor y puede recibir el proximo nodo al que se conectara (en caso de no recibir este ultimo dato se tomara como 'None')"""
		self.dato = dato
		self.prox = proximo
	def __str__(self):
		"""devuleve el valor en cadena"""
		return str(self.dato)

class ListaEnlazada:
	"""clase de lista enlazada en la que los valores estan en nodos"""
	def __init__(self):
		"""constructor de la lista enlazada la cual deve iniciar vacia, tiene una referencia al primer y al ultimo nodo de la lista y un valor adicional de longitud de la lista con el fin de acelerar el proceso de verificar su longitud"""
		self.primero = None
		self.longitud = 0
	def __str__(self):
		"""devuelve en cadena los valores de la lista"""
		return "{}".format(list(self))
	def append(self,valor):
		"""inserta un nodo con el valor que recibe al final de la lista"""
		posicion = self.longitud
		self.insertar(valor,posicion)
	def insertar(self,valor,posicion):
		"""inserta un nodo con el valor que recibe en la posicion que recibe, levanta una exepcion si la ubicacion es invalida"""
		if posicion < 0 or posicion > self.longitud:
			raise IndexError("La posicion esta fuera de rango")
		nodo_nuevo = _Nodo(valor)
		if posicion == 0:
			nodo_nuevo.prox = self.primero
			self.primero = nodo_nuevo
		else:
			anterior = self.primero
			for pos in range(1,posicion):
				anterior = anterior.prox
			nodo_nuevo.prox = anterior.prox
			anterior.prox = nodo_nuevo
		self.longitud += 1
	def remover(self,valor):
		"""borra la primera unbicacion de valor en la lista si no se encuentra lanza una exepcion"""
		if self.longitud == 0:
			raise ValueError("El valor no se encuentra por que la lista esta vacia")
		if self.primero.dato == valor:
			self.primero = self.primero.prox
		else:
			anterior = self.primero
			actual = anterior.prox
			while actual is not None and actual.dato != valor:
				anterior = actual
				actual = anterior.prox
			if actual is None:
				raise ValueError("El valor no se encuentra en la lista")
			anterior.prox = actual.prox
		self.longitud -= 1
	def remover_repetidos(self):
		"""remueve los valores repetidos que sean continuos"""
		if self.longitud == 0:
			raise ValueError("El valor no se encuentra por que la lista esta vacia")
		anterior = self.primero
		valor = anterior.dato
		actual = anterior.prox
		while valor == actual.dato:
			anterior.prox = actual.prox
			actual = anterior.prox
		while actual.prox is not None:
			anterior = actual
			actual = anterior.prox
			valor = anterior.dato
			while valor == actual.dato:
				anterior.prox = actual.prox
				if actual.prox is None:
					break
				actual = anterior.prox
	def pop(self,posicion):
		"""elimina lo que haya en la posicion que recibe y lo devuelve, lanza exepcion si la posicion es invalida"""
		if self.longitud == 0:
			raise ValueError("El valor no se encuentra por que la lista esta vacia")
		if posicion >= self.longitud or posicion < 0:
			raise IndexError("La posicion esta fuera de rango")
		if posicion == 0:
			dato = self.primero.dato
			self.primero = self.primero.prox
		else:
			anterior = self.primero
			actual = anterior.prox
			for pos in range (1,posicion):
				anterior = actual
				actual = anterior.prox
			anterior.prox = actual.prox
			dato = actual.dato
		self.longitud -= 1
		return dato
	def index(self,valor):
		"""busca el nodo de dato igual al valor recivido y devuelve su ubicacion pero si ne se encuantra lanza exepcion"""
		if self.longitud == 0:
			raise ValueError("El valor no se encuentra por que la lista esta vacia")
		for posicion in range(1,self.longitud):
			ubicacion = posicion
			actual = self.prim
			if actual.dato == valor:
				return ubicacion
		raise IndexError("El valor no se encuentra en la lista")
	def esta_en_la_lista(self,valor):
		"""busca el nodo de dato igual al valor recivido y devuelve un booleano True si esta False si no se encuentra en la lista"""
		try:
			index(valor)
			return True
		except:
			return False
	def __iter__(self):
		"""devuelve un iterador de la lista"""
		return _IteradorLE(self.primero)
	def invertir(self):
		"""invierte las posiciones de la lista"""
		pila = Pila()
		contador = 0
		while self.longitud != 0:
			contador += 1
			pila.apilar(self.pop(0))
		for algo in range (contador):
			valor = pila.desapilar()
			self.append(valor)
	def suma_acumulativa(self):
		"""a cada valor de cada nodo de la lista le suma los valores anteriores lanza excepcion con listas sin o con 1 solo elemento"""
		if self.longitud == 0 or self.longitud == 1:
			raise ValueError("No se puede operar con una lista sin elementos o con un solo elemento")
		anterior = self.primero
		actual = anterior.prox
		actual.dato += anterior.dato
		while actual.prox is not None:
			anterior = actual
			actual = anterior.prox
			actual.dato += anterior.dato
	def obtener_impares(self):
		"""devuelve la lista solo con las posiciones impares"""
		if self.longitud == 0:
			raise ValueError("No se puede operar con una lista sin elementos")
		impares = ListaEnlazada()
		anterior = self.primero
		actual = anterior.prox
		while actual is not None:
			impares.append(actual.dato)
			try:
				anterior = actual.prox
				actual = anterior.prox
			except:
				break
		return impares
class _IteradorLE:
	"""clase privada Iterador de la las listas enlazadas"""
	def __init__(self,primero):
		"""constructor del iterador de las listas enlazadas recibe un valor referenciando al primer nodo que la lista da como dato"""
		self.actual = primero
	def __next__(self):
		"""devuelve el valor de la posicion actual y avanza hasta el siguiente"""
		if self.actual is None:
			raise StopIteration()
		dato = self.actual.dato
		self.actual = self.actual.prox
		return dato

class Pila:
	"""clase Pila que permite apilar y desapilar(solo ultimo elemento agregado)"""
	def __init__(self):
		"""constructor de pila crea una pila inicialmente vacia"""
		self.item = []
	def esta_vacia(self):
		"""devuelve un booleano si la pila esta vacia True si no lo esta False"""
		return len(self.item) == 0
	def apilar(self,valor):
		"""agrega algo al final de la pila"""
		self.item.append(valor)
	def desapilar(self):
		"""desapila lo ultimo de la lsta y lo devuelve"""
		if self.esta_vacia():
			raise IndexError("La pila esta vacia")
		return self.item.pop()
	def __str__(self):
		""""""
		return str(self.item)

class Cola:
	"""primero que entra primero que sale"""
	def __init__(self):
		"""constructor"""
		self.primero = None
		self.ultimo = None
	def encolar(self,valor):
		"""agregar el valor que recibe en un nodo y lo coloca al final de la cola"""
		nuevo = _Nodo(valor)
		if self.ultimo is not None:
			self.ultimo.prox = nuevo
			self.ultimo = nuevo
		else:
			self.primero = nuevo
			self.ultimo = nuevo
	def desencolar(self):
		"""quita el ultimo elemnto de la lista y devuelve se valor si la cola esta vacia levanta una exepcion"""
		if self.primero is None:
			raise ValueError("La cola esta vacia")
		valor = self.primero.dato
		self.primero = self.primero.prox
		if self.primero is None:
			self.ultimo = None
		return valor
	def esta_vacia(self):
		"""devuelve un booleano duiciendo True si esta vacia o False si no lo esta"""
		return self.primero is None
	def __iter__(self):
		"""devuelve un iterador de la cola"""
		return _IteradorLE(self.primero)
	def __str__(self):
		""""""
		return "{}".format(list(self))
