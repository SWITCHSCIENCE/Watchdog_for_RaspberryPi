#!/usr/bin/env python

import RPi.GPIO as GPIO
import time

LEDPIN = 17

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)
GPIO.setup(LEDPIN, GPIO.OUT)

while True:
	LEDon = GPIO.output(LEDPIN, 0)
	time.sleep(0.5)
	LEDoff = GPIO.output(LEDPIN, 1)
	time.sleep(0.5)

