# Trabajo de la asignatura

# Importar librerias

import requests
import json
from time import sleep
import threading
import Tkinter as tk
import serial

# Declaracion de variables globales

# Variables meteorologia
temperatura_actual = -100
prevision_maxima = -100
prevision_minima = -100
prevision_lluvia = -1

# Variables ascensor
planta = -100
destino_deseado = -100
estado = -1
mensaje_estado = "nd"
flag_boton_0 = False
flag_boton_1 = False
flag_boton_2 = False
flag_boton_3 = False
flag_boton_4 = False

#Mensajes para la pantalla del ascensor
mensajes_estados = [
	"No operativo",
	"En mantenimiento",
	"ETSIDI",
	"Planta solicitada",
	"Subiendo",
	"Subiendo",
	"Subiendo",
	"Bajando",
	"Bajando",
	"Bajando",
	"Abriendo puertas",
	"Puerta abierta",
	"Cerrando puertas"]

# Otras variables
terminar = False

#Temporizacion hilos
tiempo_actualizacion = 1800 	# Cada 30 min
tiempo_mostrar_datos = 20		# Cada 20 segundos
tiempo_refrescar_interfaz = 1	# Cada segundo

# URL AEMET para consulta de la temperatura actual
# Ind. climatologico de estaciones varias para test
# Madrid - Retiro: 3195
# Madrid - Aeropuerto: 3129
# Puerto de Navacerrada: 2462
# Madrid - Ciudad Universitaria: 3194U
# Colmenar Viejo: 3191E
# Somosierra: 3111D
# Alcala de Henares: 3170Y
# Madrid - El Goloso: 3126Y
# San Sebastian de los Reyes: 3125Y
url_aemet_1 = "https://opendata.aemet.es/opendata/api/observacion/convencional/datos/estacion/2462"

# URL AEMET para consulta de la prevision del tiempo
url_aemet_2 = "https://opendata.aemet.es/opendata/api/prediccion/especifica/municipio/diaria/28079"

# Configuraciones varias solicitudes AEMET
querystring_aemet = {"api_key": "**************************************"}
headers_aemet = {'cache-control': 'no-cache'}

# Configuracion del puerto serie
puerto_serie = serial.Serial('/dev/ttyACM0', 115200)

# Funciones ############################################################

def finalizacion():
	global terminar
	#Todavia no se ha definido el evento de finalizacion

def getInfoMeteo():
	global terminar
	global temperatura_actual
	global temperatura_minima
	global temperatura_maxima
	global tiempo_actualizacion
	global prevision_lluvia
	global prevision_minima
	global prevision_maxima
	while terminar == False:
		#Realizar la solicitud de informacion sobre la temperatura actual
		response = requests.request("GET", url_aemet_1, headers=headers_aemet, params=querystring_aemet)
		data = response.json()
		print(json.dumps(data, indent = 3, ensure_ascii=False))
		url_datos = data.get('datos')
		if url_datos:
			datos_response = requests.get(url_datos)
			if datos_response.status_code != 200:
				print("Error en la solicitud de la temperatura actual. Codigo de estado HTML: ", datos_response.status_code)
			datos_json = datos_response.json()
			print(json.dumps(datos_json, indent = 3, ensure_ascii=False))
	
			#Ordenar los datos recibidos por fecha y hora para obtener solo los mas recientes
			observaciones_retiro = [item for item in datos_json if item.get("idema") == "3195"]
			observaciones_retiro.sort(key=lambda x: x.get("fint"), reverse = True)
			ultima_observacion = observaciones_retiro[0] if observaciones_retiro else None
	
			if ultima_observacion:
				temperatura_actual = ultima_observacion.get("ta")
				hora_lectura = ultima_observacion.get("fint")

		else:
			print("Error obteniendo la URL de AEMET con las observaciones actuales de Madrid-Retiro")
			
		#Realizar la solicitud de informacion sobre la prevision del tiempo
		response = requests.request("GET", url_aemet_2, headers=headers_aemet, params=querystring_aemet)
		data = response.json()
		url_datos = data.get('datos')
		

		if url_datos:
			datos_response = requests.get(url_datos)
			if datos_response.status_code != 200:
				print("Error en la solicitud de las previsiones a AEMET. Codigo estado HTML: ", datos_response.status_code)
			datos_json = datos_response.json()
			print(json.dumps(datos_json, indent = 3, ensure_ascii=False))
			
			# Buscar el dia actual
			for dia in datos_json[0]["prediccion"]["dia"]:
				prediccion_hoy = dia
				break
				
			if prediccion_hoy:
				prevision_maxima = prediccion_hoy["temperatura"]["maxima"]
				prevision_minima = prediccion_hoy["temperatura"]["minima"]
				probabilidad_precipitacion = prediccion_hoy.get("probPrecipitacion", [])
				probabilidad_precipitacion_int = [int(p["value"]) for p in probabilidad_precipitacion if p.get("value") not in [None, ""]]
				if probabilidad_precipitacion_int:
					prevision_lluvia = max(probabilidad_precipitacion_int)
		else:
			print("Error obteniendo la URL para el acceso a los datos de previsiones meteorologicas de AEMET")
		sleep(tiempo_actualizacion*1000)
		
		
def mostrarDatosMeteo():
	global terminar
	global temperatura_actual
	global prevision_minima
	global prevision_maxima
	global prevision_lluvia
	global tiempo_mostrar_datos
	while terminar == False:
		print("Temperatura maxima prevista para hoy: ", prevision_maxima)
		print("Temperatura minima prevista para hoy: ", prevision_minima)
		print("Probabilidad de precipitacion: ", prevision_lluvia)
		print("Temperatura actual en Madrid-Retiro: ", temperatura_actual)
		sleep(tiempo_mostrar_datos)
		
def interfazGrafica():
	global temperatura_actual
	global prevision_minima
	global prevision_maxima
	global prevision_lluvia
	global planta
	global root, lbl_planta, lbl_max, lbl_min, lbl_precip, lbl_temp_actual, lbl_estado
	global mensaje_estado
	
	root = tk.Tk()
	root.title("Panel Ascensor")
	root.geometry("480x320")
	root.configure(bg = "black")
	font_planta = ("Clearview", 100, "bold")
	font_meteo = ("Helvetica", 14)
	font_estado = ("Clearview", 14)
	
	
	# Numero de planta grande en el centro
	lbl_planta = tk.Label(root, text=str(planta), font=font_planta, fg="white", bg="black")
	lbl_planta.place(relx=0.1, rely=0.3)
	
	# Estado del ascensor
	lbl_estado = tk.Label(root, text=str(mensaje_estado), font=font_estado, fg="white", bg="black")
	lbl_estado.place(relx=0.05, rely=0.8) 
	
	# Marco lateral para datos meteorologicos
	meteo_frame = tk.Frame(root, bg="black")
	meteo_frame.place(relx=0.5, rely=0.1)
	
	# Info meteo en el lateral con colores
	mensaje_temp = str(temperatura_actual) + "*C"
	lbl_temp_actual = tk.Label(meteo_frame, text=mensaje_temp,
	font=("Helvetica", 24), fg="skyblue", bg="black")
	
	lbl_temp_actual.pack(anchor="w", pady=5)
	
	lbl_max = tk.Label(meteo_frame, text=["Max: ", {prevision_maxima}, "*C"],
	font=font_meteo, fg="red", bg="black")
	
	lbl_max.pack(anchor="w", pady=2)
	
	lbl_min = tk.Label(meteo_frame, text=["Min: ",{prevision_minima},"*C"],
	font=font_meteo, fg="blue", bg="black")
	
	lbl_min.pack(anchor="w", pady=2)
	
	lbl_precip = tk.Label(meteo_frame, text=["Prob. Lluvia: ",{prevision_lluvia},"%"], font=font_meteo, fg="lightgreen", bg="black")
	lbl_precip.pack(anchor="w", pady=2)
		
	# Iniciar bucle principal
	actualizarInterfazGrafica()
	root.mainloop()

def actualizarInterfazGrafica():
		global terminar
		global prevision_lluvia
		global prevision_maxima
		global prevision_minima
		global temperatura_actual
		global tiempo_refrescar_interfaz
		global mensaje_estado
		
		
		lbl_planta.config(text=str(planta))
		lbl_estado.config(text=str(mensaje_estado))
		mensaje_temp = str(temperatura_actual) + "*C"
		lbl_temp_actual.config(text=mensaje_temp)
		mensaje_temp = "Max: " + str(prevision_maxima) + "*C"
		lbl_max.config(text=mensaje_temp)
		mensaje_temp = "Min: " + str(prevision_minima) + "*C"
		lbl_min.config(text=mensaje_temp)
		mensaje_temp = "Prob. lluvia: " + str(prevision_lluvia) + "%"
		lbl_precip.config(text=mensaje_temp)
		
		root.after(tiempo_refrescar_interfaz, actualizarInterfazGrafica)
		
		
		
def serialRX():
	global planta
	global estado
	global mensajes_estados
	global mensaje_estado
	global terminar
	while !terminar:
		cadena=puerto_serie.readline().decode('utf-8').strip()
		print(cadena)
		valores = cadena.split(';')
		estado = int(valores[0])
		planta = int(valores[1])
		mensaje_estado = mensajes_estados[estado]
		
		
def publicarDatos():
	global planta
	global estado
	global mensaje_estado
	global terminar
	while !terminar:
		# Codigo relacionado con subir el estado del ascensor a Thingspeak
		
# Programa principal ###################################################

print("BIENVENIDO")

# Configuraciones iniciales


# Creacion de hilos
finalizacion = threading.Thread(target = finalizacion)
actualizacion_meteo = threading.Thread(target = getInfoMeteo)
mostrar_datos_meteo = threading.Thread(target = mostrarDatosMeteo)
interfaz_grafica = threading.Thread(target = interfazGrafica)
RX_puerto_serie = threading.Thread(target = serialRX)
# Lanzamiento de los hilos
finalizacion.start()
actualizacion_meteo.start()
mostrar_datos_meteo.start()
interfaz_grafica.start()
RX_puerto_serie.start()
# Finalizacion del programa
# Join de hilos si fuese necesario
