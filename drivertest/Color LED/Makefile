CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a colorledtest

colorledtest: colorledtest.c libMyPeri.a
                $(CC) colorledtest.c -lMyPeri -L. -o colorledtest


libMyPeri.a: colorled.o
        $(AR) rc libMyPeri.a colorled.o

colorled.o: colorled.h colorled.c
                $(CC) -c colorled.c -o colorled.o
