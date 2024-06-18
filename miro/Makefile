CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all : libMyPeri.a mazetest

mazetest : mazetest.c libMyPeri.a libBitmap.h bitmapFileHeader.h gyro.h fnd.h
        $(CC) mazetest.c -L. -lMyPeri -lpthread -o mazetest

libMyPeri.a: libBitmap.o gyro.o buzzer.o textlcd.o fnd.o
        $(AR) rc libMyPeri.a libBitmap.o gyro.o buzzer.o textlcd.o fnd.o

gyro.o: gyro.h gyro.c
        $(CC) -c gyro.c -o gyro.o
buzzer.o : buzzer.h buzzer.c
        $(CC) -c buzzer.c -o buzzer.o
textlcd.o : textlcddrv.h textlcd.c
        $(CC) -c textlcd.c -o textlcd.o
fnd.o : fnd.h fnd.c
        $(CC) -c fnd.c -o fnd.o
libBitmap.o: libBitmap.h bitmapFileHeader.h libBitmap.c
        $(CC) -c libBitmap.c -o libBitmap.o

clean : 
        rm -f libMyPeri.a
        rm -f mazetest

