# Trabajo de la asignatura

# Importar librerias

import requests #REST - Thingspeak
import json
import time #Para delay y para obtener la fecha y hora
import threading
import Tkinter as tk #GUI
import serial
import subprocess
from time import sleep
import paho.mqtt.client as mqtt
from datetime import datetime
import os # Cerrar terminal al final


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
idema = "3195"
ubicacion = "nd"
url_aemet_1 = "https://opendata.aemet.es/opendata/api/observacion/convencional/datos/estacion/" + idema

# URL AEMET para consulta de la prevision del tiempo
url_aemet_2 = "https://opendata.aemet.es/opendata/api/prediccion/especifica/municipio/diaria/28079"

# Configuraciones varias solicitudes AEMET
querystring_aemet = {"api_key": "****************************"}
headers_aemet = {'cache-control': 'no-cache'}

# Configuracion de las comunicaciones con Thingspeak
api_key_write_thignspeak = "**************"
api_key_read_thigspeak = "***************"
channel_thingspeak = "2945298"
headers_thingspeak = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
url_thingspeak = "http://api.thingspeak.com:80/channels/" + channel_thingspeak + "/feeds"
url_write_channel = "GET https://api.thingspeak.com/update?api_key=**************"
url_read_channel = "GET https://api.thingspeak.com/channels/2945298/feeds.json?api_key=**************"
url_read_channel_field = "GET https://api.thingspeak.com/channels/2945298/fields/1.json?api_key=****************"
url_read_channel_status_updates = "GET https://api.thingspeak.com/channels/2945298/status.json?api_key=************"


#Configuracion MQTT
broker_address = "test.mosquitto.org"
topic_tx = "/smartETSIDI/ascensor"
topic_rx_temperatura = "/smartETSIDI/aula"

# Funciones ############################################################


def getInfoMeteo():
	global terminar
	global temperatura_actual
	global temperatura_minima
	global temperatura_maxima
	global tiempo_actualizacion
	global prevision_lluvia
	global prevision_minima
	global prevision_maxima
	global idema
	global ubicacion
	while terminar == False:
		#SUSTITUIDO POR DATOS DEL PROYECTO DE JORGE
		#Realizar la solicitud de informacion sobre la temperatura actual
		#response = requests.request("GET", url_aemet_1, headers=headers_aemet, params=querystring_aemet)
		#data = response.json()
		## print(json.dumps(data, indent = 3, ensure_ascii=False))
		#url_datos = data.get('datos')
		#if url_datos:
			#datos_response = requests.get(url_datos)
			#if datos_response.status_code != 200:
			#	print("Error en la solicitud de la temperatura actual. Codigo de estado HTML: ", datos_response.status_code)
			#datos_json = datos_response.json()
			##print(json.dumps(datos_json, indent = 3, ensure_ascii=False))
			#
			##Ordenar los datos recibidos por fecha y hora para obtener solo los mas recientes
			#observaciones_retiro = [item for item in datos_json if item.get("idema") == idema]
			#observaciones_retiro.sort(key=lambda x: x.get("fint"), reverse = True)
			#ultima_observacion = observaciones_retiro[0] if observaciones_retiro else None
			#
			#if ultima_observacion:
			#	temperatura_actual = ultima_observacion.get("ta")
			#	hora_lectura = ultima_observacion.get("fint")
			#	ubicacion = ultima_observacion.get("ubi")
			#else:
			#	temperatura_actual = -99
			#	
			#	
		tiempo_proxima_actualizacion1 = tiempo_actualizacion
		#else:
		#	print "Error obteniendo la URL de AEMET con las observaciones actuales de" + ubicacion
		#	tiempo_proxima_actualizacion1 = 65 #Algo mas de un minuto que es cuando AEMET deja pedir mas datos si se excede limite solicitudes
			
		#Realizar la solicitud de informacion sobre la prevision del tiempo
		response = requests.request("GET", url_aemet_2, headers=headers_aemet, params=querystring_aemet)
		data = response.json()
		url_datos = data.get('datos')
		

		if url_datos:
			datos_response = requests.get(url_datos)
			if datos_response.status_code != 200:
				print("Error en la solicitud de las previsiones a AEMET. Codigo estado HTML: ", datos_response.status_code)
			datos_json = datos_response.json()
			# print(json.dumps(datos_json, indent = 3, ensure_ascii=False))
			
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
			
			# Si todo ha ido bien no se vuelven a solicitar datos hasta dentro de media hora
			tiempo_proxima_actualizacion2 = tiempo_actualizacion
		else:
			print("Error obteniendo la URL para el acceso a los datos de previsiones meteorologicas de AEMET")
			tiempo_proxima_actualizacion2 = 65
			
		sleep(min(tiempo_proxima_actualizacion1, tiempo_proxima_actualizacion2))
		
		
		
def mostrarDatosMeteo():
	global terminar
	global temperatura_actual
	global prevision_minima
	global prevision_maxima
	global prevision_lluvia
	global tiempo_mostrar_datos
	while terminar == False:
		print "Temperatura maxima prevista para hoy: " + str(prevision_maxima)
		print "Temperatura minima prevista para hoy: " + str(prevision_minima)
		print "Probabilidad de precipitacion: " + str(prevision_lluvia)
		print "Temperatura actual en " + ubicacion + ": " + str(temperatura_actual)
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
	
	# Botonera
	boton_frame = tk.Frame(root, bg="black")
	boton_frame.place(relx =0.75, rely=0.5, relwidth=0.9)
	
	for i in range(5):
		btn = tk.Button(boton_frame, text= str(i), width=10, height=2, command=lambda i=i: sendCmd(str(i)))
		btn.grid(row=i, column=0, padx=5)
		
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
		
		if terminar == False:
			root.after(tiempo_refrescar_interfaz, actualizarInterfazGrafica)
		
		
		
def serialRX():
	global planta
	global estado
	global mensajes_estados
	global mensaje_estado
	global terminar
	ultimo_estado = -100
	ultima_planta = -100
	while terminar == False:
		cadena=puerto_serie.readline().decode('utf-8').strip()
		valores = cadena.split(';')
		estado = int(valores[0])
		planta = int(valores[1])
		try:
			mensaje_estado = mensajes_estados[estado]
		except IndexError: #Por eco en las comunicaciones se recibe el comando enviado en la entrada del pto. serie
			print("Error index mensajes pantalla. Se deberia solucionar en unos instantes automaticamente")
		if estado != ultimo_estado or planta != ultima_planta: #Cuando cambie el estado
			ultimo_estado = estado
			ultima_planta = planta
			print "Planta: " + str(planta) + " | " + mensaje_estado
		
def publicarDatosREST():
	global terminar
	global planta
	global estado
	global mensaje_estado
	global terminar
	ultimo_estado = -100 #Inicializacion
	ultima_planta = -100
	while terminar == False:
		#print("Debug publicar datos:")
		#debug_test = "Planta: " + str(planta) + " | ultima_planta: " + str(ultima_planta) + " | Estado: " + str(estado) + " | ultimo_estado: " + str(ultimo_estado)
		#print(debug_test)
		if estado != ultimo_estado or planta != ultima_planta: #Cuando cambie el estado
			print("Actualizando datos en ThingSpeak...")
			ultimo_estado = estado
			ultima_planta = planta
			#Publicar los datos
			params = {"field1": planta, "field2": estado, "key":api_key_write_thignspeak}
			try:
				response = requests.post(url_thingspeak, params=params, headers = headers_thingspeak)
				if response.status_code == 200:
					print("Datos publicados")
				else:
					print(json.dumps(response, indent = 3, ensure_ascii=False))
			except requests.exceptions.Timeout:
				print("Timeout. Agotado tiempo de espera del servidor de Thingspeak")
				ultimo_estado = -1 # Fuerza volver a intentar publicar
			except requests.exceptions.ConnectionError:
				print("No se pudo conectar con el servidor de Thingspeak")
				ultimo_estado = -1 # Fuerza volver a intentar publicar
			except requests.exceptions.HTTPError as err:
				temp_mensaje = "Error HTTP: " + err.response.status_code + " - " + err.response.reason
				print(temp_mensaje)
				ultimo_estado = -1 # Fuerza volver a intentar publicar
			except requests.exceptions.RequestException as e:
				temp_mensaje = "Error en la solicitud: " + e
				print(temp_mensaje)
				ultimo_estado = -1 # Fuerza volver a intentar publicar
			except Exception as e:
				print "Error inesperado: %s: %s" % (type(e).__name__, str(e))
				ultimo_estado = -1 # Fuerza volver a intentar publicar
		sleep(15) #No se permite subir datos con mas frecuencia que 15 segundos
		# Codigo relacionado con subir el estado del ascensor a Thingspeak
		

def publicarDatosMQTT(client):
	global terminar
	global planta
	global estado
	global mensaje_estado
	global terminar
	ultimo_estado = -100 #Inicializacion
	ultima_planta = -100
	ultima_vez = time.time()
	
	# Crear el diccionario para las comunicaciones
	diccionario_tx = {"fecha":1741707436.7012782, "planta":-100,"estado_int":-100, "estado_str":"Nd"}
	
	while terminar == False:

		#Si se detectan cambios o ha pasado mucho tiempo, se publica informacion
		if estado != ultimo_estado or planta != ultima_planta or time.time() - ultima_vez > 30: #Cuando cambie el estado o pase tiempo
			print("Actualizando datos por MQTT...")
			ultimo_estado = estado
			ultima_planta = planta
			
			#Publicar los datos
			
			diccionario_tx["fecha"] = datetime.now().isoformat()
			diccionario_tx["planta"] = planta
			diccionario_tx["estado_int"] = estado
			diccionario_tx["estado_str"] = mensaje_estado
			
			diccionario_json = json.dumps(diccionario_tx, indent=3, sort_keys=True)
			client.publish(topic_tx, diccionario_json, retain=True)
            
			ultima_vez = time.time()
            

def sendCmd(msg):
	mensaje = msg + "\n"
	try:
		puerto_serie.reset_input_buffer()
		puerto_serie.write(mensaje.encode())
		sleep(.05)
		puerto_serie.flush()
		puerto_serie.reset_output_buffer()
		puerto_serie.reset_input_buffer()
		print("Comando enviado al ascensor")
	except Exception as e:
		print "No se pudo enviar el comando por el puerto serie: " + str(e)
		
		
	
#MQTT RX	
def on_connect_rx(client, userdata, flags, rc):
	print("Connected with result code " + str(rc))
	client.subscribe(topic_rx_temperatura)
    
#MQTT RX    
def on_message_rx(client1, userdata, message):
	global temperatura_actual
	print("Message received")
	mensaje = str(message.payload.decode("utf-8"))
	data = json.loads(mensaje)
	temperatura_actual = data.get("Temperatura")
	
		
#TX MQTT
def on_connect_tx(client, userdata, flags, rc):
	print("MQTT TX conectado al servidor con codigo " + str(rc))
    
#TX MQTT
def on_publish_tx(client, userdata, mid):
	print("Datos publicados por MQTT")

	
# Programa principal ###################################################

print("BIENVENIDO")
print("Puede finalizar el programa cuando lo desee con ctrl+C")

# Configuraciones iniciales

# Configuracion del puerto serie
puerto_serie = serial.Serial('/dev/ttyACM0', 115200)

# Configuracion cliente MQTT para TX
client2 = mqtt.Client()

	#Definir funciones tratamiento callbacks
client2.on_connect = on_connect_tx
client2.on_publish = on_publish_tx

client2.connect(broker_address, 1883, 60)

# Configuracion cliente MQTT para RX
client = mqtt.Client()

	#Definir funciones tratamiento callbacks
client.on_connect = on_connect_rx
client.on_message = on_message_rx

client.connect(broker_address, 1883, 60)

client.loop_start()

# Creacion de hilos
publicar_datos_REST = threading.Thread(target = publicarDatosREST, name = "Publicar REST")
publicar_datos_MQTT = threading.Thread(target = publicarDatosMQTT, args=(client2,), name = "Publicar MQTT")
actualizacion_meteo = threading.Thread(target = getInfoMeteo, name = "Info meteo")
mostrar_datos_meteo = threading.Thread(target = mostrarDatosMeteo, name = "Mostrar datos meteo")
interfaz_grafica = threading.Thread(target = interfazGrafica, name = "GUI")
RX_puerto_serie = threading.Thread(target = serialRX, name = "Puerto serie")


# Lanzamiento de los hilos
actualizacion_meteo.start()
mostrar_datos_meteo.start()
interfaz_grafica.start()
RX_puerto_serie.start()
publicar_datos_REST.start()
publicar_datos_MQTT.start()


try:
	while True:
		time.sleep(1)
except KeyboardInterrupt:
	print("Finalizando el programa...")
	terminar = True

print("Deteniendo RX MQTT")
client.loop_stop()
client.disconnect()
print("Finalizando hilos...")
RX_puerto_serie.join()
print("Fin conexion UART PSoC")
publicar_datos_REST.join()
print("Fin conexion Thingspeak")
publicar_datos_MQTT.join()
print("Fin MQTT")


root.quit()
root.destroy()
os._exit(0)
