# MUST have downloaded the arduino cli before using. Download here:
# https://arduino.github.io/arduino-cli/0.21/installation/

upload:
	arduino-cli compile ./ --port /dev/cu.usbmodem101 --fqbn "arduino:avr:uno"
	arduino-cli upload ./ --port /dev/cu.usbmodem101 --fqbn "arduino:avr:uno"

listen:
	arduino-cli monitor --port /dev/cu.usbmodem101

compile:
	arduino-cli compile ./ --port /dev/cu.usbmodem101 --fqbn "arduino:avr:uno"