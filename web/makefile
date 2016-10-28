
all: build
	@echo "done"

build: install
	@webpack

upload:
	make -C ../mcu upload-web

install: node_modules
	@echo "Install ok"

clean:
	rm -rf node_modules

node_modules:
	npm install

