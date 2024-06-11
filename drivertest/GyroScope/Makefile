CC = arm-linux-gnueabihf-gcc
AR = arm-linux-gnueabihf-ar

all: libMyPeri.a gyrotest

gyrotest: gyrotest.c libMyPeri.a
        $(CC) gyrotest.c -lMyPeri -L. -o gyrotest

libMyPeri.a: gyro.o
        $(AR) rc libMyPeri.a gyro.o

gyro.o: gyro.h gyro.c
        $(CC) -c gyro.c -o gyro.o
