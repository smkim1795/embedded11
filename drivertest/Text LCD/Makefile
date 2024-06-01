CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a ledtest buttontest buzzertest textlcdtest

libMyPeri.a: button.o led.o buzzer.o textlcd.o
        $(AR) rc libMyPeri.a led.o button.o buzzer.o textlcd.o

textlcd.o: textlcd.h textlcddrv.h textlcd.c
        $(CC) -c textlcd.c -o textlcd.o 

buzzer.o: buzzer.h buzzer.c
        $(CC) -c buzzer.c -o buzzer.o

button.o: button.h button.c
        $(CC) -c button.c -o button.o

led.o: led.h led.c
        $(CC) -c led.c -o led.o

ledtest: ledtest.c libMyPeri.a
        $(CC) ledtest.c -lMyPeri -L. -o ledtest

buttontest: buttontest.c libMyPeri.a
        $(CC) buttontest.c -lMyPeri -L. -lpthread -o buttontest

buzzertest: buzzertest.c libMyPeri.a
        $(CC) buzzertest.c -lMyPeri -L. -o buzzertest

textlcdtest: textlcdtest.c textlcd.h libMyPeri.a
        $(CC) textlcdtest.c -lMyPeri -L. -o textlcdtest

clean:
        rm *.o -rf
        rm *.a -rf
