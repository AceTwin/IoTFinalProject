from picamera import PiCamera
import paho.mqtt.client as mqtt
import requests
import json
import datetime

uri = 'http://192.168.3.1:3000/hooks/r9CNJs8hpYKxe53np/yNvsbryGk4YJGfcptALtHAj6aJKofXz9Z5MdhH2g7cdYQSnw'

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("pressure/sensor")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(str(msg.payload)[2])
    if str(msg.payload)[2]=="y":
        #The door said something
        print("The Door!!!")
        imagepost()
    else:
        #And it's gone
        print("I release you!")

def imagepost():
    camera = PiCamera()
    filename = "image"+str(datetime.datetime.now().strftime('%Y_%m_%d_%H_%M_%S'))+".jpg"
    print(filename)
    camera.capture(filename)
    camera.close()
    print("Image saved as: "+filename)
    data={}
    data = {
        "attachments": [
            {
                "text":"Hello",
                "image_url":"http://192.168.3.44:8000/"+filename,
                "color":"#764FA5"
            }
        ]
    }

    r = requests.post(uri, json.dumps(data)).content


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.3.1", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()

