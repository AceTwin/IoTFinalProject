#!/usr/bin/python
from picamera import PiCamera

camera = PiCamera()
camera.capture('image.jpg')
