
all: install build upload-web upload-mcu console
	@echo "done"



upload-mcu: build-mcu
	pio run -t upload

upload-web: build-web
	pio run -t uploadfs


build: build-mcu build-web

build-mcu: install
	pio run

build-web:
	make -C web build	


console:
	pio serialports monitor -b 115200


install: install-mcu install-web

install-mcu:
	pio lib install ESPAsyncWebServer
	pio lib install WiFiManager

install-web: 
	make -C web install



clean:
	rm -rf .pioenvs .piolibdeps
	make -C web clean