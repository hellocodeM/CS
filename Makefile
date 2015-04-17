all: 
	cd src/ && make

test: all
	cd src/ && make test

clean: 
	cd src/ && make clean
