
all: upload-mcu

upload: upload-mcu upload-web

upload-mcu: build
	pio run -t upload

upload-web:
	pio run -t uploadfs


build: install
	pio run

console:
	pio serialports monitor -b 115200

install:
	pio lib install ESPAsyncWebServer
	pio lib install WiFiManager

clean:
	rm -rf .pioenvs .piolibdeps
