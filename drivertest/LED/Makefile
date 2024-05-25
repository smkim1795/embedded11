all: ledtest.c libMyPeri.a
	arm-linux-gnueabihf-gcc -o ledtest ledtest.c -lMyPeri -L.

libMyPeri.a: led.o
	arm-linux-gnueabihf-ar rc libMyPeri.a led.o

led.o: led.h led.c
	arm-linux-gnueabihf-gcc -c led.c -o led.o
