
all: install build 
	make -C mcu upload
	@echo "done"



build: build-mcu build-web

build-mcu:
	make -C mcu build	

build-web:
	make -C web build	




install: install-mcu install-web

install-mcu:
	make -C mcu install

install-web: 
	make -C web install



clean:
	make -C mcu clean
	make -C web clean