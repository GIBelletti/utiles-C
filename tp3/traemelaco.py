import grafo_tda as GRAFO
#import heap as HEAP
#import objetos as OBJ
import funciones_grafo as FUNG
import csv
from os.path import exists as comprobar_si_existe
import sys

#CONSTANTES
CANT_DE_GRUPOS = 2
CANT_DE_COLUMNAS = 2
COMANDOS = ["ir","viaje","itinerario","reducir_caminos"]
OPT_APROX = ["optimo","aproximado"]
EXTENCION_KML = ".kml"
#CONSTANTES
#FUNCIONES AUXILIARES
def atoi(s):
	n, notnegative = 0, 1
	if s[0]=="-":
		notnegative = -1
		s = s[1:]
	for i in s:
		n = n*10 + ord(i)-ord("0")
	return notnegative*n

def procesar_csv(nombre):
	"""recibe un nombre de un archivo *.csv lo abre y lo convierte en un grafo que devuelve"""
	if not comprobar_si_existe(nombre):
		return None
	grafo = GRAFO.Grafo()
	with open(nombre, 'r') as csvfile:
		texto = csv.reader(csvfile,delimiter=",")
		contador = 0
		cambio = 0
		for linea in texto:
			if contador == 0:
				contador += atoi(linea[0])
				cambio += 1
			elif cambio == 1:
				nombre = linea[0]
				latitud = linea[2]#esta invertido
				longitud = linea[1]
				extra = [latitud,longitud]
				grafo.agregar_vertice(nombre,extra)
				contador -= 1
			else:
				vertice1 = linea[0]
				vertice2 = linea[1]
				peso = atoi(linea[2])
				grafo.agregar_arista(vertice1,vertice2,peso)
				contador -= 1
	return grafo

def ingresar(mensaje = None):
	"""recibe dos cadenas una por stdin y devuelve la cadena"""
	if not mensaje == None:
		print("{}".format(mensaje))
	try:
		palabras = input()
	except:
		return None
	return palabras

def agregar_aristas_dirigidas(grafo,recomendaciones):
	if not comprobar_si_existe(recomendaciones):
		print("DEBUG, No existe archivo")
		return False
	with open(recomendaciones, 'r') as csvfile:
		texto = csv.reader(csvfile,delimiter=",")
		for linea in texto:
			origen = linea[0]
			destino = linea[1]
			grafo.agregar_arista_dirigida(origen,destino)#no tomo en cuenta el peso
	return True

def escribir_kml(comando,info,grafo,camino):
	if comando == COMANDOS[2]:
		nombre = "{} {}".format(comando,info[0])
	else:
		nombre = "{} {}, {}".format(comando,info[0],info[1])
	with open(nombre + EXTENCION_KML, 'w') as archivo:
		#inicio
		archivo.write('<?xml version="1.0" encoding="UTF-8"?>\n<kml xmlns="http://earth.google.com/kml/2.1">\n	<Document>\n')
		#nombre/descripcion
		archivo.write('		<name>{}</name>\n'.format(nombre))
		#archivo.write('		<description>{}.</description>\n'.format(nombre))
		#vertices
		vertices = grafo.ver_vertices()
		archivo.write('\n')
		#for v in vertices:
		for v in camino:
			info = grafo.info_vertice(v)#[lat,lon]
			archivo.write('		<Placemark>\n')
			archivo.write('			<name>{}</name>\n'.format(v))
			#archivo.write('			<description>{}</description>\n'.format(v))
			archivo.write('			<Point>\n')
			archivo.write('				<coordinates>{}, {}</coordinates>\n'.format(info[0],info[1]))
			archivo.write('			</Point>\n')
			archivo.write('		</Placemark>\n')
		#aristas
		archivo.write('\n')
		#for V in vertices:
		longitud = len(camino)
		for V in range(longitud - 1):
			info_V = grafo.info_vertice(camino[V])#[lat,lon]
			info_W = grafo.info_vertice(camino[V+1])#[lat,lon]
			archivo.write('		<Placemark>\n')
			archivo.write('			<LineString>\n')
			archivo.write('				<coordinates>{},  {} {}, {}</coordinates>\n'.format(info_V[0],info_V[1],info_W[0],info_W[1]))
			archivo.write('			</LineString>\n')
			archivo.write('		</Placemark>\n')
		#final
		archivo.write('	</Document>\n</kml>'.format(nombre))
#COMANDOS
def viaje(grafo, tipo, origen):
	if not tipo in OPT_APROX:
		return False
	if tipo == OPT_APROX[0]:
		viaje,peso = FUNG.viajante(grafo, origen)
	elif tipo == OPT_APROX[1]:
		viaje,peso = FUNG.viajante_aproximado(grafo, origen)
	else:
		return False
	print(" -> ".join(viaje))
	print("Costo total: {}".format(peso))
	#kml
	info = [tipo,origen]
	escribir_kml(COMANDOS[1],info,grafo,viaje)
	return True

def ir(grafo, origen, destino):
	viaje = FUNG.camino_minimo(grafo, origen, destino)
	peso = FUNG.calcular_peso(grafo,viaje)
	print(" -> ".join(viaje))
	print("Costo total: {}".format(peso))
	#kml
	info = [origen,destino]
	escribir_kml(COMANDOS[0],info,grafo,viaje)
	return True

def itinerario(grafo_original,recomendaciones):
	grafo_nuevo = FUNG.grafo_copiar_vertices(grafo_original)
	while True:
		if not agregar_aristas_dirigidas(grafo_nuevo,recomendaciones):
			return False
		recomendcion = FUNG.orden_topologico(grafo_nuevo)
		peso = FUNG.calcular_peso(grafo_original,recomendcion)
		if peso < 0:
			continue#si pasa esto corta la iteracion
		print(" -> ".join(recomendcion))
		print("Costo total: {}".format(peso))
		#kml
		info = [recomendaciones]
		escribir_kml(COMANDOS[2],info,grafo_original,recomendcion)
		return True

def reducir_caminos(grafo,nombre):
	grafo_nuevo = FUNG.arbol_tendido_minimo(grafo)
	vertices = grafo_nuevo.ver_vertices()
	cant_vertices = len(vertices)
	cant_aristas = FUNG.cant_aristas(grafo_nuevo)
	if (not(cant_aristas + 1 == cant_vertices)) and not (cant_vertices == len(grafo.ver_vertices())):
		return False
	with open(nombre, 'w') as csvfile:
		escritor = csv.writer(csvfile, delimiter=',')
		escritor.writerow([cant_vertices])
		for vertice in vertices:
			escritor.writerow([vertice] + grafo.info_vertice(vertice))
		escritor.writerow([cant_aristas])
		visitados = {}
		for v in vertices:
			visitados[v] = False
		for V in vertices:
			visitados[V] = True
			escribir = [V]
			for W in grafo_nuevo.adyacentes(V):
				while len(escribir) > 1:
					escribir.pop()
				if visitados[W]:
					continue
				escribir.append(W)
				escribir.append(grafo_nuevo.peso_arista(V,W))
				escritor.writerow(escribir)
	peso = FUNG.peso_total(grafo_nuevo,True)
	print("Peso total: {}".format(int(peso)))
	return True
#COMANDOS
#FUNCIONES AUXILIARES

def menu(argv):
	#arg = [*.py,*.csv,*.kml]
	#el argv[2] es la direccion donde escribir el *.kml
	if not len(argv) == 3:
		return False
	grafo = procesar_csv(argv[1])
	if grafo == None:
		return False
	comando = ingresar()
	terminar = False
	funciones = {COMANDOS[0]:ir,COMANDOS[1]:viaje,COMANDOS[2]:itinerario,COMANDOS[3]:reducir_caminos}
	while not terminar:
		valores = comando.split(' ')
		cant = len(valores)
		cont_prim = 0
		if cant > 3:
			for i in range(1,cant):
				if valores[i][-1] == ',':
					cont_prim = i
					break
			aux_prim = valores[1:cont_prim + 1]
			valores[1] = (" ".join(aux_prim))
			aux_segu = valores[cont_prim + 1:]
			valores[2] = (" ".join(aux_segu))
			valores = valores[:3]
			cant = len(valores);
		if valores[0] == COMANDOS[0] or valores[0] == COMANDOS[1]:
			if not cant == 3:#[comando,valor1,valor2]
				return False
			if valores[1][-1] == ',':
				valores[1] = (valores[1].split(','))[0]
			if not funciones[valores[0]](grafo,valores[1],valores[2]):
				return False
		elif valores[0] == COMANDOS[2] or valores[0] == COMANDOS[3]:
			if not cant == 2:#[comando,arch]
				return False
			if not funciones[valores[0]](grafo,valores[1]):
				#print("DEBUG, FALLA")
				return False
		else:
			terminar = True
		comando = ingresar()
		if (terminar or comando == None):
			break
menu(sys.argv)
