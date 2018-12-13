#!/usr/bin/python
import rainbowhat as rh
import colorsys
import time
import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
#    client.subscribe("$SYS/#")
    client.subscribe("pressure/sensor")

def ledalert(str):
    rh.display.print_str(str)
    rh.display.show()


def temp():
    t = rh.weather.temperature()
    t = (t * (9/5))+32
    rh.display.clear()
    rh.display.print_float(t)
    rh.display.show()

    i = 1

def alert():
    while True:
        h = i / 100.0
        r, g, b = [int(c * 255) for c in colorsys.hsv_to_rgb(h, 1.0, 1.0)]
        for pixel in range(7):
            rh.rainbow.clear()
            rh.rainbow.set_pixel(pixel, r,g,b)
            rh.rainbow.show()
            time.sleep(0.05)
        rh.lights.rgb(0, 0, 0)
        i=i+10
        r, g, b = [int(c * 255) for c in colorsys.hsv_to_rgb(h, 1.0, 1.0)]
        for pixel in range(6,1,-1):
            rh.rainbow.clear()
            rh.rainbow.set_pixel(pixel, r,g,b)
            rh.rainbow.show()
            time.sleep(0.05)
        temp()
        if i == 100:
            i = 1

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if str(msg.payload)=="y":
        ledalert('Door')
        alert()
    else:
        ledalert('    ')

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.3.1", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()


