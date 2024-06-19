CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all : libMyPeri.a projectcode

projectcode : projectcode.c libMyPeri.a libBitmap.h bitmapFileHeader.h
        $(CC) projectcode.c -L. -lMyPeri -lpthread -o projectcode

libMyPeri.a: libBitmap.o gyro.o buzzer.o textlcd.o fnd.o colorled.o button.o led.o
        $(AR) rc libMyPeri.a libBitmap.o gyro.o buzzer.o textlcd.o fnd.o colorled.o button.o led.o

gyro.o: gyro.h gyro.c
        $(CC) -c gyro.c -o gyro.o
buzzer.o : buzzer.h buzzer.c
        $(CC) -c buzzer.c -o buzzer.o
textlcd.o : textlcddrv.h textlcd.c
        $(CC) -c textlcd.c -o textlcd.o
fnd.o : fnd.h fnd.c
        $(CC) -c fnd.c -o fnd.o
colorled.o : colorled.h colorled.c
        $(CC) -c colorled.c -o colorled.o
button.o : button.h button.c
        $(CC) -c button.c -o button.o
led.o : led.h led.c
        $(CC) -c led.c -o led.o
libBitmap.o: libBitmap.h bitmapFileHeader.h libBitmap.c
        $(CC) -c libBitmap.c -o libBitmap.o

clean : 
        rm -f libMyPeri.a
        rm *.o -rf
        rm -f projectcode
