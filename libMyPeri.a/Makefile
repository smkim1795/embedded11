CC = arm-linux-gnueabihf-gcc
AR = arm-linux-gnueabihf-ar

all: libMyPeri.a final

final: final.c libMyPeri.a libBitmap.h bitmapFileHeader.h gyro.h
        $(CC) final.c -lMyPeri -L. -lpthread -o final

libMyPeri.a: libBitmap.o gyro.o
        $(AR) rc libMyPeri.a libBitmap.o gyro.o 

gyro.o: gyro.h gyro.c
        $(CC) -c gyro.c -o gyro.o

libBitmap.o: libBitmap.h bitmapFileHeader.h libBitmap.c
        $(CC) -c libBitmap.c -o libBitmap.o
