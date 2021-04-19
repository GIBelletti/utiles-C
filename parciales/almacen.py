class _Nodo:
	"""clase privada Nodo en la cual se almacena informacion, para usar en otras clases"""
	def __init__(self,dato,proximo = None):
		"""constructor de la clase nodo la cual recibe un valor y puede recibir el proximo nodo al que se conectara (en caso de no recibir este ultimo dato se tomara como 'None')"""
		self.dato = dato
		self.prox = proximo
	def __str__(self):
		"""devuleve el valor en cadena"""
		return str(self.dato)

class Pedidos:
	def __init__(self):
		self.primero = None
		self.ultimo = None
		self.pedido = {}
	def encolar(self,r):
		nodo = _Nodo(r,self.ultimo)
		#r = [producto, cantidad, nombre]
		if self.ultimo == None:
			self.primero = nodo
			self.ultimo = nodo
		else:
			self.ultimo = nodo
		#####
		if r[0] in self.pedido:
			self.pedido[r[0]] += r[1]
		else:
			self.pedido[r[0]] = r[1]
	def atender(self):
		if self.primero == None:
			return None
		valor = self.primero.dato
		self.primero = self.primero.prox
		if self.primero == None:
			self.ultimo = None
		#####
		self.pedido[valor[0]] -= valor[1]
		if self.pedido[valor[0]] == 0:
			self.pedido.pop(valor[0])
		return valor
	def cuanto(self,p):
		if not p in self.pedido:
			return 0
		return self.pedido[p]
