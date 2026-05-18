import sys
import time
import serial
from Adafruit_IO import MQTTClient
from Adafruit_IO import Client, Feed

ADAFRUIT_IO_USERNAME = "Ludwin21"
ADAFRUIT_IO_KEY = "aio_sbHw60ohBFLnSOti9juplLoJv9r8"

# Feeds
FEED_BotonManual         = "botonmanual"
FEED_BotonEEPROM         = "botoneeprom"
FEED_BotonUart           = "botonuart"
FEED_EEPROM_POSE         = "eeprom-pose"
FEED_Cantidad_EEPROM     = "cantidad-eeprom"
FEED_IndicadorPoseEEPROM = "indicadorposeeeprom"
FEED_Servo1	             = "servo1"
FEED_Servo2	             = "servo2"
FEED_Servo3	             = "servo3"
FEED_Servo4	             = "servo4"

ser = serial.Serial('COM3', 4800, timeout=0.1)

def send(msg):
    ser.write((msg + '\n').encode())
    print(f"Enviado: {msg}")

def connected(client):
    client.subscribe(FEED_BotonManual)
    client.subscribe(FEED_BotonEEPROM)
    client.subscribe(FEED_BotonUart)
    client.subscribe(FEED_EEPROM_POSE)
    client.subscribe(FEED_Servo1)
    client.subscribe(FEED_Servo2)
    client.subscribe(FEED_Servo3)
    client.subscribe(FEED_Servo4)
    print("Conectado y suscrito a todos los feeds")

def disconnected(client):
    print("Desconectado")
    sys.exit(1)

def message(client, feed_id, payload):
    print(f"Feed: {feed_id} -> {payload}")

    if   feed_id == FEED_BotonManual:
        if payload == "1":
            send("B1:000")

    elif feed_id == FEED_BotonEEPROM:
        if payload == "1":
            send("B2:000")

    elif feed_id == FEED_BotonUart:
        if payload == "1":
            send("B3:000")

    elif feed_id == FEED_EEPROM_POSE:
        if payload == "1":
            send("B2:001")

    elif feed_id == FEED_Servo1:
        send(f"S1:{int(payload):03d}") 

    elif feed_id == FEED_Servo2:
        send(f"S2:{int(payload):03d}")

    elif feed_id == FEED_Servo3:
        send(f"S3:{int(payload):03d}")

    elif feed_id == FEED_Servo4:
        send(f"S4:{int(payload):03d}")

client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
client.on_connect    = connected
client.on_disconnect = disconnected
client.on_message    = message

client.connect()
client.loop_background()


while True:
    if ser.in_waiting > 0:
        linea = ser.readline().decode('utf-8').strip()
        print(f"ATmega: {linea}")

        if linea.startswith("C:"):
            cantidad = int(linea[2:])
            client.publish(FEED_Cantidad_EEPROM, cantidad)
            print(f"Publicado cantidad EEPROM: {cantidad}")

        elif linea.startswith("P:"):
            posicion = int(linea[2:])
            client.publish(FEED_IndicadorPoseEEPROM, posicion)
            print(f"Publicado posición EEPROM: {posicion}")
    time.sleep(0.1)
