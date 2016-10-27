
all: upload-web upload-mcu console
	@echo "done"



upload-mcu: build-mcu
	pio run -t upload

upload-web: build-web
	pio run -t uploadfs



build-mcu: install
	pio run

build-web:
	make -C web build	



console:
	pio serialports monitor -b 115200


install:
	pio lib install ESPAsyncWebServer
	pio lib install WiFiManager


clean:
	rm -rf .pioenvs .piolibdeps
	make -C web clean