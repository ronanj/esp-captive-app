
This is a captive application demo for the ESP8266 / Gizwits module

# Captive Application

The idea is you can use an ESP8266 as an access point, to which, when logged in, 
the user can start a standalone web application and interact (via web-sockets) with the ESP as well
as all other devices conencted to the ESP.

By nature, a captive application is a web application which runs in the captive portal login.

# Installation

	make install
	make build
	make upload

or just `make`

# Customization

Once the application MCU code is uploaded to the ESP, 
you can just build you application and only upload the web part to the device.

	cd web
	make build
	make upload
