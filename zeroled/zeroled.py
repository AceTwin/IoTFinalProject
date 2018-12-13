#!/usr/bin/python
import rainbowhat as rh
import colorsys
import time
import paho.mqtt.client as mqtt

status="n"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("pressure/sensor")

def ledalert(str):
    rh.display.clear()
    rh.display.print_str(str)
    rh.display.show()

def temp():
    t = rh.weather.temperature()
    t = (t * (9/5))+32
    ledalert(str(int(t))+'F')

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if str(msg.payload)=="y":
        ledalert('Door')
        rh.rainbow.set_all(255,0,0)
        rh.rainbow.show()
        status="y"
    else:
        temp()
        rh.rainbow.set_all(0,0,0)
        rh.rainbow.show()
        status="n"

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.3.1", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
if status=="n":
    temp()
client.loop_forever()


