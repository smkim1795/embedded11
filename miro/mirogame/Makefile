C = arm-linux-gnueabihf-gcc
AR = arm-linux-gnueabihf-ar

all: libMyPeri.a final

final: final.c libMyPeri.a libBitmap.h bitmapFileHeader.h gyro.h fnd.h
        $(CC) final.c -lMyPeri -L. -lpthread -o final

libMyPeri.a: libBitmap.o gyro.o fnd.o
        $(AR) rc libMyPeri.a libBitmap.o gyro.o fnd.o

fnd.o: fnd.h fnd.c
        $(CC) -c fnd.c -o fnd.o

gyro.o: gyro.h gyro.c
        $(CC) -c gyro.c -o gyro.o

libBitmap.o: libBitmap.h bitmapFileHeader.h libBitmap.c
        $(CC) -c libBitmap.c -o libBitmap.o

clean:
        rm *.o -rf
        rm *.a -rf
        rm libMyPeri.a
